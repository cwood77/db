#include "../cmn/autoPtr.hpp"
#include "../cmn/string.hpp"
#include "../cmn/talias.hpp"
#include "../file/manager.hpp"
#include "api.hpp"
#include "viewSpec.hpp"
#include <fstream>
#include <memory>

namespace cmd {
namespace {

class commonViewSpecParser : public iCommonViewSpecParser {
public:
   virtual void parse(model::viewSpec& vs, const std::string& line)
   {
      const char *pThumb = line.c_str();
      if(cmn::startsWithAndAdvance(pThumb,"rule: "))
      {
         tcat::typePtr<cmn::typeAliasTable> talias;
         talias->demand("rules",pThumb);
      }

      throw std::runtime_error(std::string("unrecognized viewSpec line: ") + line);
   }
};

class topViewSpecParser : public iTopViewSpecParser {
public:
   virtual model::viewSpec& parse(std::istream& s)
   {
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

      if(!m_pSpec.get() || m_pSpec->type.empty())
         throw std::runtime_error("viewSpec indicates no type?");

      return *m_pSpec.release();
   }

private:
   void handleLine(const std::string& line)
   {
      // comments
      const char *pThumb = line.c_str();
      cmn::eatWhitespace(pThumb);
      if(*pThumb == '#')
         return;

      if(m_pMatch)
         (*m_pMatch)->parse(*m_pSpec,line);
      else
         handleLineUntyped(line);
   }

   void handleLineUntyped(const std::string& line)
   {
      const char *pThumb = line.c_str();
      if(cmn::startsWithAndAdvance(pThumb,"type: "))
      {
         matchSpecificParser(pThumb);
         m_pSpec.reset(&(*m_pMatch)->createViewSpec());
      }
      else
         throw std::runtime_error(std::string("unknown viewSpec line: ") + line);
   }

   void matchSpecificParser(const std::string& type)
   {
      tcat::typePtr<cmn::typeAliasTable> talias;
      auto fullTypeName = talias->demand("views",type);
      m_pMatch.reset(new tcat::typePtr<iViewSpecParser>(fullTypeName));
      return;
   }

   std::unique_ptr<tcat::typePtr<iViewSpecParser> > m_pMatch;
   std::unique_ptr<model::viewSpec> m_pSpec;
};

class parseViewSpecCommandImpl : public iParseViewSpecCommandImpl {
public:
   parseViewSpecCommandImpl() : m_ppSpec(NULL) {}

   virtual void execute()
   {
      tcat::typePtr<file::iFileManager> fMan;
      cmn::autoReleasePtr<file::iFileInStream> fStream(&fMan->demandReadStream(m_path));

      tcat::typePtr<iTopViewSpecParser> pParser;
      *m_ppSpec = &pParser->parse(fStream->stream());
   }

   virtual void configure(const std::string& path, model::viewSpec **ppSpec)
   {
      m_path = path;
      m_ppSpec = ppSpec;
   }

private:
   std::string m_path;
   model::viewSpec **m_ppSpec;
};

tcatExposeTypeAs(commonViewSpecParser,iCommonViewSpecParser);
tcatExposeTypeAs(topViewSpecParser,iTopViewSpecParser);
tcatExposeTypeAs(parseViewSpecCommandImpl,iParseViewSpecCommandImpl);

} // anonymous namespace
} // namespace cmd
