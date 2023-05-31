#ifndef ___cmd_api___
#define ___cmd_api___

#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include <string>

namespace cmd {

class iProgram;

class iCommand {
public:
   virtual ~iCommand() {}
   virtual void queue(iProgram& p) = 0;
};

class iCommandImpl {
public:
   virtual ~iCommandImpl() {}
   virtual void execute() = 0;
};

class iProgram {
public:
   virtual ~iProgram() {}
   virtual void add(iCommand& c) = 0;
   virtual void add(iCommandImpl& c) = 0;
   virtual void execute() = 0;
};

// =============================== COMMANDS ===============================
// ========================================================================

// --------------- parse viewSpec
class iParseViewSpecCommandImpl : public iCommandImpl {
public:
   virtual void configure(const std::string& path, model::viewSpec **ppSpec) = 0;
};

class parseViewSpecCommand : public iCommand {
public:
   parseViewSpecCommand(const std::string& path, model::viewSpec **ppSpec)
   { m_pImpl->configure(path,ppSpec); }

   virtual void queue(iProgram& p) { p.add(*m_pImpl); }

private:
   tcat::typePtr<iParseViewSpecCommandImpl> m_pImpl;
};

// --------------- parse view
class iParseViewCommandImpl : public iCommandImpl {
public:
   virtual void configure(const std::string& path, model::view& v) = 0;
};

class parseViewCommand : public iCommand {
public:
   parseViewCommand(const std::string& path, model::view &v)
   { m_pImpl->configure(path,v); }

   virtual void queue(iProgram& p) { p.add(*m_pImpl); }

private:
   tcat::typePtr<iParseViewCommandImpl> m_pImpl;
};

// --------------- load view
class loadViewCommand : public iCommand {
public:
   loadViewCommand(const std::string& path, model::view& out) : m_path(path), m_out(out) {}

   virtual void queue(iProgram& p)
   {
      p.add(*new parseViewSpecCommand(m_path + ".viewSpec",&m_out.pSpec));
      p.add(*new parseViewCommand(m_path,m_out));
   }

private:
   std::string m_path;
   model::view& m_out;
};

// --------------- unmap view
class iUnmapViewCommandImpl : public iCommandImpl {
public:
   virtual void configure(model::view& v) = 0;
};

class unmapViewCommand : public iCommand {
public:
   explicit unmapViewCommand(model::view &v)
   { m_pImpl->configure(v); }

   virtual void queue(iProgram& p) { p.add(*m_pImpl); }

private:
   tcat::typePtr<iUnmapViewCommandImpl> m_pImpl;
};

// --------------- map view
class iMapViewCommandImpl : public iCommandImpl {
public:
   virtual void configure(model::view& v) = 0;
};

class mapViewCommand : public iCommand {
public:
   explicit mapViewCommand(model::view &v)
   { m_pImpl->configure(v); }

   virtual void queue(iProgram& p) { p.add(*m_pImpl); }

private:
   tcat::typePtr<iMapViewCommandImpl> m_pImpl;
};

// --------------- save view
class iSaveViewCommandImpl : public iCommandImpl {
public:
   virtual void configure(const std::string& path, model::view& v) = 0;
};

class saveViewCommand : public iCommand {
public:
   saveViewCommand(const std::string& path, model::view &v)
   { m_pImpl->configure(path,v); }

   virtual void queue(iProgram& p) { p.add(*m_pImpl); }

private:
   tcat::typePtr<iSaveViewCommandImpl> m_pImpl;
};

} // namespace cmd

#endif // ___cmd_api___
