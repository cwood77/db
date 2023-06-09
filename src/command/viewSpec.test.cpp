#define WIN32_LEAN_AND_MEAN
#include "../tcatlib/api.hpp"
#include <windows.h>

#ifdef cdwTest
#include "../file/manager.hpp"
#include "../model/api.hpp"
#include "../test/api.hpp"
#include "viewSpec.hpp"
#include <memory>
#include <sstream>

namespace {

testDefineTest(diff_viewSpec_parse_emptyThrows)
{
   tcat::typePtr<file::iFileManager> fMan;
   std::stringstream viewSpecContents;
   fMan->fakeStream("|dummy|.viewSpec",viewSpecContents);

   bool threw = false;
   try
   {
      tcat::typePtr<cmd::iTopViewSpecParser> pParser;
      std::unique_ptr<model::viewSpec> pVs(&pParser->parse("|dummy|.viewSpec"));
   }
   catch(std::exception&)
   {
      threw = true;
   }
   a.assertTrue(threw);
}

testDefineTest(diff_viewSpec_parse_diffOkWithComments)
{
   tcat::typePtr<file::iFileManager> fMan;
   std::stringstream viewSpecContents;
   viewSpecContents
      << "# this line is discarded as a comment" << std::endl
      << "   # this one too" << std::endl
      << std::endl
      << std::endl
      << "type: diff" << std::endl
      << "   # this one three" << std::endl
      << std::endl
      << "key: a" << std::endl
      << std::endl
   ;
   fMan->fakeStream("|dummy|.viewSpec",viewSpecContents);

   tcat::typePtr<cmd::iTopViewSpecParser> pParser;
   std::unique_ptr<model::viewSpec> pVs(&pParser->parse("|dummy|.viewSpec"));

   a.assertTrue(pVs->type == "diff");
   a.assertTrue(!pVs->parserTypeName.empty());
   a.assertTrue(!pVs->formatterTypeName.empty());
}

} // namespace anonymous

#endif // cdwTest

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
