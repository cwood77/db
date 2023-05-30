#include "../cmn/string.hpp"
#include "api.hpp"
#include "saveload.hpp"
#include <fstream>
#include <memory>

namespace cmd {
namespace {

class parseViewCommandImpl : public iParseViewCommandImpl {
public:
   parseViewCommandImpl() : m_pView(NULL) {}

   virtual void execute()
   {
      std::ifstream in(m_path.c_str());
      if(!in.good())
         throw std::runtime_error(std::string("can't open file:") + m_path);

      tcat::typePtr<iViewParser> pParser(m_pView->pSpec->parserTypeName);
      pParser->parse(*m_pView,in);
   }

   virtual void configure(const std::string& path, model::view& v)
   {
      m_path = path;
      m_pView = &v;
   }

private:
   std::string m_path;
   model::view *m_pView;
};

tcatExposeTypeAs(parseViewCommandImpl,iParseViewCommandImpl);

} // anonymous namespace
} // namespace cmd

