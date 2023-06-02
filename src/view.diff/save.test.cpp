#ifdef cdwTest
#include "../command/api.hpp"
#include "../command/viewSpec.hpp"
#include "../file/manager.hpp"
#include "../model/api.hpp"
#include "../test/api.hpp"
#include <sstream>
#include "save.hpp"

namespace {

testDefineTest(diff_view_save)
{
   tcat::typePtr<file::iFileManager> fMan;

   std::stringstream viewContents;
   fMan->fakeStream("|dummy|",viewContents);

   std::stringstream expected;
   expected
      << "---------------------------" << std::endl
      << "a: b" << std::endl
      << std::endl
   ;

   model::view v;
   v.pSpec = new model::viewSpec();
   v.pSpec->type = "diff";
   v.pSpec->cols.push_back("a");
   v.pSpec->formatterTypeName = typeid(view_diff::formatter).name();
   v.model.records.push_back(model::record());
   auto& r = *(--v.model.records.end());
   r.fields["a"] = "b";

   tcat::typePtr<cmd::iProgram> pProg;
   pProg->add(*new cmd::saveViewCommand("|dummy|",v));
   pProg->execute();

   a.assertTrue(expected.str() == viewContents.str());
}

#if 0
testDefineTest(diff_view_roundtrip)
{
   tcat::typePtr<file::iFileManager> fMan;

   std::stringstream writtenOut;
   fMan->fakeStream("|dummy|",writtenOut);

   model::view v;
   v.pSpec = new model::viewSpec();
   v.pSpec->type = "diff";
   v.pSpec->formatterTypeName = typeid(view_diff::formatter).name();
   v.model.records.push_back(model::record());
   auto& r = *(--v.model.records.end());
   r.fields["a"] = "b";

   {
      tcat::typePtr<cmd::iProgram> pProg;
      pProg->add(*new cmd::saveViewCommand("|dummy|",v));
      pProg->execute();
   }

   std::stringstream viewContents;
   viewContents << writtenOut.str();
   fMan->fakeStream("|dummy|",viewContents);

   model::view actual;
   {
      tcat::typePtr<cmd::iProgram> pProg;
      pProg->add(*new cmd::parseViewCommand("|dummy|",actual));
      pProg->execute();
   }

   a.assertTrue(actual == v);
}
#endif

} // namespace anonymous

#endif // cdwTest
