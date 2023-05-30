#include "../cmn/string.hpp"
#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include "load.hpp"

namespace view_diff {

void parser::parse(model::view& v, std::istream& s)
{
   while(true)
   {
      if(!s.good())
         break;

      std::string line;
      std::getline(s,line);
      if(line.empty())
         continue;

      handleLine(v,line);
   }
}

void parser::handleLine(model::view& v, const std::string& line)
{
   // comments
   const char *pThumb = line.c_str();
   cmn::eatWhitespace(pThumb);
   if(*pThumb == '#')
      return;

   if(m_pCurrRec)
      handleField(v,line);
   else
      handleGlobal(v,line);
}

void parser::handleField(model::view& v, const std::string& line)
{
   auto N = measureRule(line);
   if(N > 3)
   {
      v.model.records.push_back(model::record());
      m_pCurrRec = &*(--v.model.records.end());
      return;
   }

   const char *pThumb = line.c_str();
   for(;*pThumb!=0&&*pThumb!=':';++pThumb);
   if(*pThumb==0)
      throw std::runtime_error(std::string("failed to split string during view parse: ") + line);

   if(pThumb[1] != ' ')
      throw std::runtime_error(std::string("unknown field syntax during view parse: ") + line);

   std::string name(line.c_str(),pThumb-line.c_str());
   std::string value(pThumb+2);
   if(m_pCurrRec->fields.find(name)!=m_pCurrRec->fields.end())
      throw std::runtime_error(std::string("duplicate field on view parse line: ") + line);

   m_pCurrRec->fields[name] = value;
}

void parser::handleGlobal(model::view& v, const std::string& line)
{
   auto N = measureRule(line);
   if(N > 3)
   {
      v.model.records.push_back(model::record());
      m_pCurrRec = &*(--v.model.records.end());
   }
   else
      throw std::runtime_error(std::string("unrecognized line in view parse: ") + line);
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
