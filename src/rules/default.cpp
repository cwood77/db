#define WIN32_LEAN_AND_MEAN
#include "../cmn/talias.hpp"
#include "../command/viewSpec.hpp"
#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include <windows.h>

namespace rules {

class defaultGuidRule {
};

class defaultGuidRuleFactory : public cmd::iRuleFactory {
public:
   virtual model::iRule& create(const std::string& string)
   {
      throw std::runtime_error(std::string("bad args: ") + string);
   }
};

tcatExposeTypeAs(defaultGuidRuleFactory,defaultGuidRuleFactory);
publishTypeAlias(rules,default,defaultGuidRuleFactory);

} // namespace rules

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
