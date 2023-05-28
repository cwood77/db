#include "../cmn/string.hpp"
#include "api.hpp"
#include "viewSpec.hpp"
#include <fstream>

namespace cmd {
namespace {

class commonViewSpecParser : public iCommonViewSpecParser {
public:
   virtual void parse(model::viewSpec& vs, const std::string& line)
   {
      throw std::runtime_error(std::string("unrecognized viewSpec line: ") + line);
   }
};

class topViewSpecParser : public iTopViewSpecParser {
public:
   topViewSpecParser() : m_pMatch(NULL) {}

   virtual void parse(model::viewSpec& vs, std::istream& s)
   {
      while(true)
      {
         if(!s.good())
            break;

         std::string line;
         std::getline(s,line);
         if(line.empty())
            continue;

         handleLine(vs,line);
      }

      if(vs.type.empty())
         throw std::runtime_error("viewSpec indicates no type?");
   }

private:
   void handleLine(model::viewSpec& vs, const std::string& line)
   {
      // comments
      const char *pThumb = line.c_str();
      cmn::eatWhitespace(pThumb);
      if(*pThumb == '#')
         return;

      if(m_pMatch)
         m_pMatch->parse(vs,line);
      else
         handleLineUntyped(vs,line);
   }

   void handleLineUntyped(model::viewSpec& vs, const std::string& line)
   {
      const char *pThumb = line.c_str();
      if(cmn::startsWithAndAdvance(pThumb,"type: "))
      {
         vs.type = pThumb;
         matchSpecificParser(pThumb);
      }
      else
         throw std::runtime_error(std::string("unknown viewSpec line: ") + line);
   }

   void matchSpecificParser(const std::string& type)
   {
      for(size_t i=0;i<m_parserSet.size()&&!m_pMatch;i++)
         if(m_parserSet[i]->handlesType(type))
            m_pMatch = m_parserSet[i];

      if(!m_pMatch)
         throw std::runtime_error(std::string("no viewSpec parser knows type: ") + type);
   }

   tcat::typeSet<iViewSpecParser> m_parserSet;
   iViewSpecParser *m_pMatch;
};

class parseViewSpecCommandImpl : public iParseViewSpecCommandImpl {
public:
   parseViewSpecCommandImpl() : m_pOut(NULL) {}

   virtual void execute()
   {
      std::ifstream in(m_path.c_str());
      if(!in.good())
         throw std::runtime_error(std::string("can't open file:") + m_path);

      tcat::typePtr<iTopViewSpecParser> pParser;
      pParser->parse(*m_pOut,in);
   }

   virtual void configure(const std::string& path, model::viewSpec& out)
   {
      m_path = path;
      m_pOut = &out;
   }

private:
   std::string m_path;
   model::viewSpec *m_pOut;
};

tcatExposeTypeAs(commonViewSpecParser,iCommonViewSpecParser);
tcatExposeTypeAs(topViewSpecParser,iTopViewSpecParser);
tcatExposeTypeAs(parseViewSpecCommandImpl,iParseViewSpecCommandImpl);

} // anonymous namespace
} // namespace cmd
