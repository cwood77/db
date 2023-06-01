#include "api.hpp"
#include <sstream>

namespace cmd {
namespace {

class errorReport : public model::iErrorReport {
public:
   virtual std::ostream& create()
   {
      m_errors.push_back(std::stringstream());
      return *(--m_errors.end());
   }

   void compileAndThrow()
   {
      if(m_errors.size() == 0)
         return;

      std::stringstream overall;
      overall << "database has " << m_errors.size() << " consistency error(s)" << std::endl;
      for(auto& err : m_errors)
         overall << err.str();

      throw std::runtime_error(overall.str());
   }

private:
   std::list<std::stringstream> m_errors;
};

class checkViewCommandImpl : public iCheckViewCommandImpl {
public:
   checkViewCommandImpl() : m_pView(NULL) {}

   virtual void execute()
   {
      errorReport err;

      for(auto *pRule : m_pView->pSpec->rules)
         pRule->check(*m_pView,err);

      err.compileAndThrow();
   }

   virtual void configure(model::view& v)
   {
      m_pView = &v;
   }

private:
   model::view *m_pView;
};

tcatExposeTypeAs(checkViewCommandImpl,iCheckViewCommandImpl);

} // anonymous namespace
} // namespace cmd
