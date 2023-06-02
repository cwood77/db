#define WIN32_LEAN_AND_MEAN
#include "../cmn/talias.hpp"
#include "../command/viewSpec.hpp"
#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include "load.hpp"
#include "save.hpp"
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

   virtual model::viewSpec& createViewSpec() const
   {
      std::unique_ptr<model::viewSpec> pVs(new model::viewSpec());
      pVs->type = "diff";
      pVs->pFilter = new model::filter::always();
      pVs->parserTypeName = typeid(parser).name();
      pVs->formatterTypeName = typeid(formatter).name();
      return *pVs.release();
   }

private:
   tcat::typePtr<cmd::iViewSpecLineParser> pBase;
};

tcatExposeTypeAs(viewSpecParser,viewSpecParser);
publishTypeAlias(views,diff,viewSpecParser);

} // namespace view_diff

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
