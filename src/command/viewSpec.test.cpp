#define WIN32_LEAN_AND_MEAN
#include "../tcatlib/api.hpp"
#include <windows.h>

#ifdef cdwTest
#include "../model/api.hpp"
#include "../test/api.hpp"
#include "viewSpec.hpp"
#include <memory>
#include <sstream>

namespace {

testDefineTest(diff_viewSpec_parse_emptyThrows)
{
   std::stringstream in;

   bool threw = false;
   try
   {
      tcat::typePtr<cmd::iTopViewSpecParser> pParser;
      std::unique_ptr<model::viewSpec> pVs(&pParser->parse(in));
   }
   catch(std::exception&)
   {
      threw = true;
   }
   a.assertTrue(threw);
}

testDefineTest(diff_viewSpec_parse_diffOkWithComments)
{
   std::stringstream in;
   in
      << "# this line is discarded as a comment" << std::endl
      << "   # this one too" << std::endl
      << std::endl
      << std::endl
      << "type: diff" << std::endl
      << "   # this one three" << std::endl
      << std::endl
   ;

   tcat::typePtr<cmd::iTopViewSpecParser> pParser;
   std::unique_ptr<model::viewSpec> pVs(&pParser->parse(in));

   a.assertTrue(pVs->type == "diff");
   a.assertTrue(!pVs->parserTypeName.empty());
}

} // namespace anonymous

#endif // cdwTest

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
