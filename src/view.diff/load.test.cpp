#define WIN32_LEAN_AND_MEAN
#include "../tcatlib/api.hpp"
#include <windows.h>

#ifdef cdwTest
#include "../command/api.hpp"
#include "../command/viewSpec.hpp"
#include "../file/manager.hpp"
#include "../model/api.hpp"
#include "../test/api.hpp"
#include <sstream>

namespace std {

bool operator==(const model::record& lhs, const model::record& rhs)
{
#if 0
   auto jit = rhs.fields.begin();
   for(auto it=lhs.fields.begin();it!=lhs.fields.end();++it,++jit)
   {
      ::printf("%s[%s]<?>%s[%s]\n",
         it->first.c_str(),it->second.c_str(),
         jit->first.c_str(),jit->second.c_str());
   }
#endif

   return lhs.fields == rhs.fields;
}

}

bool operator==(const model::model& lhs, const model::model& rhs)
{
   return lhs.records == rhs.records;
}

namespace {

testDefineTest(diff_view_load)
{
   tcat::typePtr<file::iFileManager> fMan;

   std::stringstream viewSpecContents;
   viewSpecContents
      << "type: diff"
   ;
   fMan->fakeStream("|dummy|.viewSpec",viewSpecContents);

   std::stringstream viewContents;
   viewContents
      << "---------------------------" << std::endl
      << "a: b"
   ;
   fMan->fakeStream("|dummy|",viewContents);

   model::model expected;
   expected.records.push_back(model::record());
   auto& r = *(--expected.records.end());
   r.fields["a"] = "b";

   tcat::typePtr<cmd::iProgram> pProg;
   model::view v;
   pProg->add(*new cmd::loadViewCommand("|dummy|",v));
   pProg->execute();

   a.assertTrue(expected == v.model);
}

} // namespace anonymous

#endif // cdwTest

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
