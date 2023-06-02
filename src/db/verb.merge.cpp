#define WIN32_LEAN_AND_MEAN
#include "../cmn/autoPtr.hpp"
#include "../cmn/service.hpp"
#include "../cmn/win32.hpp"
#include "../command/api.hpp"
#include "../console/arg.hpp"
#include "../console/log.hpp"
#include "../cui/api.hpp"
#include "../cui/pen.hpp"
#include "../file/api.hpp"
#include "../file/manager.hpp"
#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include <fstream>
#include <memory>

namespace {

class mergeCommand : public console::iCommand {
public:
   std::string oInputView;
   std::string oIntoView;
   std::string oOutputView;

   virtual void run(console::iLog& l);
};

class myVerb : public console::globalVerb {
protected:
   virtual console::verbBase *inflate()
   {
      std::unique_ptr<console::verbBase> v(
         new console::verb<mergeCommand>("--merge"));

      v->addParameter(
         console::stringParameter::required(offsetof(mergeCommand,oIntoView)));
      v->addParameter(
         console::stringParameter::required(offsetof(mergeCommand,oInputView)));
      v->addParameter(
         console::stringParameter::optional(offsetof(mergeCommand,oOutputView)));

      return v.release();
   }
} gVerb;

void mergeCommand::run(console::iLog& l)
{
   tcat::typePtr<file::iFileManager> fMan;
   cmn::autoReleasePtr<file::iSstFile> pFile(&fMan->bindFile<file::iSstFile>(
      file::iFileManager::kExeAdjacent,
      "config.sst",
      file::iFileManager::kReadOnly
   ));
   pFile->tie(l);

   tcat::typePtr<cmn::serviceManager> svcMan;
   cmn::autoService<console::iLog> _logSvc(*svcMan,l);

   l.writeLnInfo("db " __DATE__ " " __TIME__);

   model::view in;
   model::view into;
   tcat::typePtr<cmd::iProgram> pProg;

   pProg->add(*new cmd::loadViewCommand(oInputView,in));
   pProg->add(*new cmd::unmapViewCommand(in));
   pProg->add(*new cmd::checkViewCommand(in));

   pProg->add(*new cmd::loadViewCommand(oIntoView,into));
   pProg->add(*new cmd::unmapViewCommand(into));
   pProg->add(*new cmd::checkViewCommand(into));

   pProg->add(*new cmd::mergeViewCommand(in,into.model));

   pProg->add(*new cmd::checkViewCommand(into));
   pProg->add(*new cmd::mapViewCommand(into));
   pProg->add(*new cmd::saveViewCommand(oOutputView.empty() ? oIntoView : oOutputView,into));

   pProg->execute();
}

} // anonymous namespace
