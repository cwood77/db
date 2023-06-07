#ifdef cdwTest
#include "../test/api.hpp"
#include "nodes.hpp"
#include "parser.hpp"

namespace filter {
namespace {

class fakeNodeFactory : public iNodeFactory {
public:
   virtual void chain(std::unique_ptr<filterNode>& pRoot, types t, const std::string& arg = "")
   {
      actualTypes.push_back(t);

      // make the parser think I'm doing something
      if(pRoot.get() == 0)
         pRoot.reset(new equalsNode());
   }

   std::list<types> actualTypes;
};

testDefineTest(filter_parser_emptyStringThrows)
{
   fakeNodeFactory nf;
   try
   {
      delete &parser(nf).parse("");
      a.assertTrue(false);
   }
   catch(std::exception&)
   {
   }
   a.assertTrue(nf.actualTypes.size() == 0);
}

testDefineTest(filter_parser_simpleHappy)
{
   fakeNodeFactory nf;
   delete &parser(nf).parse(".someField \"word with space\" ==");

   std::list<iNodeFactory::types> expected;
   expected.push_back(iNodeFactory::kFieldName);
   expected.push_back(iNodeFactory::kStringLiteral);
   expected.push_back(iNodeFactory::kEquals);
   a.assertTrue(nf.actualTypes == expected);
}

} // anonymous namespace
} // namespace filter

#endif // cdwTest

