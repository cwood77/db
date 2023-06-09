#include "../cmn/autoPtr.hpp"
#include "../cmn/string.hpp"
#include "../cmn/talias.hpp"
#include "../file/manager.hpp"
#include "../filter/api.hpp"
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
         const char *pColon = pThumb;
         for(;*pColon!=0&&*pColon!=':';++pColon);
         std::string alias(pThumb,pColon-pThumb);

         tcat::typePtr<cmn::typeAliasTable> talias;
         auto fullTypeName = talias->demand("rules",alias);
         tcat::typePtr<cmd::iRuleFactory> pRFac(fullTypeName);

         if(pColon[0] == ':') pColon++;
         vs.rules.push_back(&pRFac->create(pColon));
      }
      else if(cmn::startsWithAndAdvance(pThumb,"filter: "))
      {
         tcat::typePtr<filter::iExpert> fEx;
         fEx->configure(vs,pThumb);
      }
      else if(cmn::startsWithAndAdvance(pThumb,"cols: "))
         cmn::splitCommaList(pThumb,vs.cols);
      else if(cmn::startsWithAndAdvance(pThumb,"key: "))
         vs.keyField = pThumb;
      else
         throw std::runtime_error(std::string("unrecognized viewSpec line: ") + line);
   }
};

class topViewSpecParser : public iTopViewSpecParser {
public:
   virtual model::viewSpec& parse(const std::string& filePath)
   {
      handleFile(filePath);

      if(!m_pSpec.get() || m_pSpec->type.empty())
         throw std::runtime_error("viewSpec indicates no type?");

      if(!m_pSpec->pFilter)
         throw std::runtime_error("viewSpec indicates no filter?");

      if(m_pSpec->keyField.empty())
         throw std::runtime_error("viewSpec indicates no key?");

      return *m_pSpec.release();
   }

private:
   void handleFile(const std::string& path)
   {
      std::string adjustedPath = path;
      if(!m_filePath.empty())
         adjustedPath = m_filePath + "\\..\\" + path;
      else
         m_filePath = path;

      tcat::typePtr<file::iFileManager> fMan;
      cmn::autoReleasePtr<file::iFileInStream> fStream(&fMan->demandReadStream(adjustedPath));
      auto& s = fStream->stream();

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

   void handleLine(const std::string& line)
   {
      // comments
      const char *pThumb = line.c_str();
      cmn::eatWhitespace(pThumb);
      if(*pThumb == '#')
         return;

      // inherit
      if(cmn::startsWithAndAdvance(pThumb,"inherit: "))
      {
         std::string path = pThumb;
         handleFile(path);
         return;
      }

      if(m_pMatch)
         (*m_pMatch)->parse(*m_pSpec,line);
      else
         handleLineUntyped(line);
   }

   void handleLineUntyped(const std::string& line)
   {
      const char *pThumb = line.c_str();
      if(cmn::startsWithAndAdvance(pThumb,"type: "))
         matchSpecificParser(pThumb);
      else
         throw std::runtime_error(std::string("unknown viewSpec line: ") + line);
   }

   void matchSpecificParser(const std::string& type)
   {
      tcat::typePtr<cmn::typeAliasTable> talias;
      auto fullTypeName = talias->demand("views",type);
      m_pMatch.reset(new tcat::typePtr<iViewSpecParser>(fullTypeName));
      m_pSpec.reset(&(*m_pMatch)->createViewSpec());
   }

   std::string m_filePath;
   std::unique_ptr<tcat::typePtr<iViewSpecParser> > m_pMatch;
   std::unique_ptr<model::viewSpec> m_pSpec;
};

class parseViewSpecCommandImpl : public iParseViewSpecCommandImpl {
public:
   parseViewSpecCommandImpl() : m_ppSpec(NULL) {}

   virtual void execute()
   {
      tcat::typePtr<iTopViewSpecParser> pParser;
      *m_ppSpec = &pParser->parse(m_path);
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
