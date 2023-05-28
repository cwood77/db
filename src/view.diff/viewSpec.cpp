#define WIN32_LEAN_AND_MEAN
#include "../command/viewSpec.hpp"
#include "../tcatlib/api.hpp"
#include <windows.h>

namespace view_diff {

class viewSpecParser : public cmd::iViewSpecParser {
public:
   viewSpecParser()
   : pBase(typeid(cmd::iCommonViewSpecParser).name())
   {
   }

   virtual void parse(model::viewSpec& vs, const std::string& line)
   {
      pBase->parse(vs,line);
   }

   virtual bool handlesType(const std::string& type) const { return type == "diff"; }

private:
   tcat::typePtr<cmd::iViewSpecLineParser> pBase;
};

tcatExposeTypeAs(viewSpecParser,cmd::iViewSpecParser);

} // namespace view_diff

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
