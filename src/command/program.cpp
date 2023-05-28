#define WIN32_LEAN_AND_MEAN
#include "../tcatlib/api.hpp"
#include <windows.h>
#include "api.hpp"

namespace cmd {

class program : public iProgram {
public:
   ~program()
   {
      // delete commands, but not commandImpls
      // commands are responsible for cleaning up impls
      for(auto *pC : m_cmds)
         delete pC;
   }

   virtual void add(iCommand& c)
   {
      m_cmds.push_back(&c);
      c.queue(*this);
   }

   virtual void add(iCommandImpl& c)
   {
      m_impls.push_back(&c);
   }

   virtual void execute()
   {
      for(auto *pI : m_impls)
         pI->execute();
   }

private:
   std::list<iCommand*> m_cmds;
   std::list<iCommandImpl*> m_impls;
};

tcatExposeTypeAs(program,iProgram);

} // namespace cmd

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
