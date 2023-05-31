#include "api.hpp"

namespace cmd {
namespace {

class unmapViewCommandImpl : public iUnmapViewCommandImpl {
public:
   unmapViewCommandImpl() : m_pView(NULL) {}

   virtual void execute()
   {
      for(auto *pRule : m_pView->pSpec->rules)
         pRule->unmap(*m_pView);
   }

   virtual void configure(model::view& v)
   {
      m_pView = &v;
   }

private:
   model::view *m_pView;
};

class mapViewCommandImpl : public iMapViewCommandImpl {
public:
   mapViewCommandImpl() : m_pView(NULL) {}

   virtual void execute()
   {
      for(auto *pRule : m_pView->pSpec->rules)
         pRule->map(*m_pView);
   }

   virtual void configure(model::view& v)
   {
      m_pView = &v;
   }

private:
   model::view *m_pView;
};

tcatExposeTypeAs(unmapViewCommandImpl,iUnmapViewCommandImpl);
tcatExposeTypeAs(mapViewCommandImpl,iMapViewCommandImpl);

} // anonymous namespace
} // namespace cmd
