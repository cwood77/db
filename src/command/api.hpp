#ifndef ___cmd_api___
#define ___cmd_api___

#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include <string>

//namespace model { class view; }
//namespace model { class viewSpec; }

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

class iParseViewSpecCommandImpl : public iCommandImpl {
public:
   virtual void configure(const std::string& path, model::viewSpec& out) = 0;
};

class parseViewSpecCommand : public iCommand {
public:
   parseViewSpecCommand(const std::string& path, model::viewSpec& out)
   { m_pImpl->configure(path,out); }

   virtual void queue(iProgram& p) { p.add(*m_pImpl); }

private:
   tcat::typePtr<iParseViewSpecCommandImpl> m_pImpl;
};

class loadViewCommand : public iCommand {
public:
   loadViewCommand(const std::string& path, model::view& out) : m_path(path), m_out(out) {}

   virtual void queue(iProgram& p)
   {
      p.add(*new parseViewSpecCommand(m_path,m_out.s));
   }

private:
   std::string m_path;
   model::view& m_out;
};

} // namespace cmd

#endif // ___cmd_api___
