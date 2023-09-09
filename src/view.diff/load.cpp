#include "../cmn/string.hpp"
#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include "load.hpp"
#include <sstream>

namespace view_diff {

void parser::parse(model::view& v, std::istream& s)
{
   m_pView = &v;

   while(true)
   {
      if(!s.good())
         break;

      std::string line;
      std::getline(s,line);
      if(line.empty())
         continue;

      handleLine(line);
   }
}

void parser::handleLine(const std::string& line)
{
   // comments
   const char *pThumb = line.c_str();
   cmn::eatWhitespace(pThumb);
   if(*pThumb == '#')
      return;

   if(!m_currField.empty())
      handleMultilineField(line);
   else if(m_pCurrRec)
      handleField(line);
   else
      handleGlobal(line);
}

void parser::handleMultilineField(const std::string& line)
{
   if(line == ">")
   {
      m_currField.clear();
      return;
   }

   auto& f = m_pCurrRec->fields[m_currField];
   std::stringstream s;
   s << f << std::endl << line;
   f = s.str();
}

void parser::handleField(const std::string& line)
{
   if(tryHandleRule(line))
      return;

   bool isMultiline = false;
   const char *pThumb = line.c_str();
   for(;*pThumb!=0&&*pThumb!=':';++pThumb);
   if(*pThumb==0)
      throw std::runtime_error(std::string("failed to split string during view parse: ") + line);

   if(pThumb[1] == '<')
      isMultiline = true;
   else if(pThumb[1] != ' ')
      throw std::runtime_error(std::string("unknown field syntax during view parse: ") + line);

   std::string name(line.c_str(),pThumb-line.c_str());
   std::string value(pThumb+2);
   if(m_pCurrRec->fields.find(name)!=m_pCurrRec->fields.end())
      throw std::runtime_error(std::string("duplicate field on view parse line: ") + line);

   m_pCurrRec->fields[name] = value;
   if(isMultiline)
      m_currField = name;
}

void parser::handleGlobal(const std::string& line)
{
   if(!tryHandleRule(line))
      throw std::runtime_error(std::string("unrecognized line in view parse: ") + line);
}

bool parser::tryHandleRule(const std::string& line)
{
   auto N = measureRule(line);
   if(N > 3)
   {
      m_pView->model.records.push_back(model::record());
      m_pCurrRec = &*(--m_pView->model.records.end());
      return true;
   }
   return false;
}

size_t parser::measureRule(const std::string& line)
{
   const char *pThumb = line.c_str();
   for(;*pThumb=='-';++pThumb);
   if(*pThumb==0)
      return pThumb - line.c_str();
   else
      return 0;
}

tcatExposeTypeAs(parser,parser);

} // namespace view_diff
