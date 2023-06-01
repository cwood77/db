// single-schema DB
// native types are (sst):
//   string
//   mint
//   tf
// string-based semantic types are
//   enum
//   date
//   guid
//
// --view syntax--                      check?   mutate?   up?   down?
// type: [sst|compose|table]
// inherit: rel-path
// default<random>: field                                  Y
// default: field=value                                    Y
// now: field=value                                        Y
// on-modify: now: field=value                   Y << CUT THIS! (or at least defer) >>
// unique: field                        Y
// enum<a,b,c>: field                   Y
// checkbox field when value                               Y     Y
// filter: <<RPN>>                                               Y
// sort: a,-b,c                                                  Y
// cols: a,b,c                                                   Y
//


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

class loadViewCommand {};      // path, view
class parseViewSpecCommand {}; // path, viewSpec
class parseViewCommand {};     // path, view

class unmapViewCommand {};     // view

class mapViewCommand {};       // view

class checkViewCommand {};     // view

class mergeViewCommand {};     // view, view

class saveViewCommand {};      // model, path

class mapCommand : public console::iCommand {
public:
   std::string oInputView;
   std::string oOutputView;

   virtual void run(console::iLog& l);
};

class myVerb : public console::globalVerb {
protected:
   virtual console::verbBase *inflate()
   {
      std::unique_ptr<console::verbBase> v(
         new console::verb<mapCommand>("--map"));

      v->addParameter(
         console::stringParameter::required(offsetof(mapCommand,oInputView)));
      v->addParameter(
         console::stringParameter::required(offsetof(mapCommand,oOutputView)));

      return v.release();
   }
} gVerb;

void mapCommand::run(console::iLog& l)
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

   tcat::typePtr<cmd::iProgram> pProg;

   model::view in;

   pProg->add(*new cmd::loadViewCommand(oInputView,in));
   pProg->add(*new cmd::checkViewCommand(in));

   pProg->execute();
}

} // anonymous namespace
