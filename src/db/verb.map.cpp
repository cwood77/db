#define WIN32_LEAN_AND_MEAN
#include "../cmn/autoPtr.hpp"
#include "../cmn/service.hpp"
#include "../cmn/win32.hpp"
#include "../cmn/wshsubproc.hpp"
#include "../console/arg.hpp"
#include "../console/log.hpp"
#include "../cui/api.hpp"
#include "../cui/pen.hpp"
#include "../file/api.hpp"
#include "../file/manager.hpp"
#include "../tcatlib/api.hpp"
#include <fstream>
#include <memory>

namespace {

class intCommand : public console::iCommand {
public:
   std::string oCannedInputFile;

   virtual void run(console::iLog& l);
};

class myVerb : public console::globalVerb {
protected:
   virtual console::verbBase *inflate()
   {
      std::unique_ptr<console::verbBase> v(
         new console::verb<intCommand>("--int"));

      v->addParameter(
         console::stringParameter::optional(offsetof(intCommand,oCannedInputFile)));

      return v.release();
   }
} gVerb;

void intCommand::run(console::iLog& l)
{
   tcat::typePtr<file::iFileManager> fMan;
   //l.writeLnDebug("loading config settings (optional)");
   cmn::autoReleasePtr<file::iSstFile> pFile(&fMan->bindFile<file::iSstFile>(
      file::iFileManager::kExeAdjacent,
      "config.sst",
      file::iFileManager::kReadOnly
   ));
   pFile->tie(l);

   //l.writeLnDebug("setup the shmem");
   cmn::shmem<cmn::wshSubprocBlock> pShmem(
      cmn::buildWshSubprocShmemName(::GetCurrentProcessId()));
   if(!pShmem.existed())
      throw std::runtime_error("don't appear to be running under wsh?");

   //l.writeLnDebug("compiling services");
   tcat::typePtr<cmn::serviceManager> svcMan;
   pen::object _pen(std::cout);
   cmn::autoService<pen::object> _penSvc(*svcMan,_pen);

   //l.writeLnDebug("loading canned input");

   l.writeLnInfo("wsh " __DATE__ " " __TIME__);
}

} // anonymous namespace
