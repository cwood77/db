#define WIN32_LEAN_AND_MEAN
#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include "nodes.hpp"
#include "parser.hpp"
#include <windows.h>

namespace filter {

class rule : public model::iRule {
public:
   virtual void map(model::view& v)
   {
      std::list<model::record> passes;

      for(auto& r : v.model.records)
      {
         bool pass = v.pSpec->pFilter->isPass(r);
         if(pass)
            passes.push_back(r);
      }

      v.model.records = passes;
   }
};

class expert : public iExpert {
public:
   virtual void configure(model::viewSpec& vs, const std::string& expr)
   {
      // set filter
      {
         delete vs.pFilter;
         vs.pFilter = NULL;

         nodeFactory nf;
         vs.pFilter = new model::filter::when(parser(nf).parse(expr));
      }

      // set rule
      vs.rules.push_back(new rule());
   }
};

tcatExposeTypeAs(expert,iExpert);

} // namespace filter

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
