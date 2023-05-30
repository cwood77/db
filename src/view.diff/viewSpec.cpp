#define WIN32_LEAN_AND_MEAN
#include "../command/viewSpec.hpp"
#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include "load.hpp"
#include <memory>
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

   virtual model::viewSpec& createViewSpec() const
   {
      std::unique_ptr<model::viewSpec> pVs(new model::viewSpec());
      pVs->type = "diff";
      pVs->parserTypeName = typeid(parser).name();
      return *pVs.release();
   }

private:
   tcat::typePtr<cmd::iViewSpecLineParser> pBase;
};

tcatExposeTypeAs(viewSpecParser,cmd::iViewSpecParser);

} // namespace view_diff

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
