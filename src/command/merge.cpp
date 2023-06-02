#include "api.hpp"
#include <functional>
#include <stdexcept>

namespace cmd {
namespace {

class mergeViewCommandImpl : public iMergeViewCommandImpl {
public:
   mergeViewCommandImpl() : m_pView(NULL), m_pInto(NULL) {}

   virtual void execute()
   {
      // ---merge existing records and add new ones
      compareEachRightToLeft(m_pInto->records,m_pView->model.records,
      [&](auto *pInto, auto& in)
      {
         if(pInto)
            // if both views have a record, merge it
            merge(*pInto,in);
         else
            // if only in view has it, add it
            add(in);
      });

      // ---consider deleting some stuff
      compareEachRightToLeft(m_pView->model.records,m_pInto->records,
      [&](auto *pIn, auto& into)
      {
         if(!pIn)
            // if only the into (old) view has it, consider whether
            // it's absence indicates a delete or not
            considerDelete(into);
      });
   }

   virtual void configure(model::view& v, model::model& into)
   {
      m_pView = &v;
      m_pInto = &into;
   }

private:
   void compareEachRightToLeft(
      std::list<model::record>& lookup,
      std::list<model::record>& list,
      std::function<void(model::record*,model::record&)> compare)
   {
      // sweep lookup records and build a map based on the key
      std::map<std::string,model::record*> map;
      for(auto& r : lookup)
      {
         auto kvalue = r.fields[m_pView->pSpec->keyField];
         map[kvalue] = &r;
      }

      // sweep list records and look for matches in the map
      for(auto& r : list)
      {
         auto kvalue = r.fields[m_pView->pSpec->keyField];

         auto it = map.find(kvalue);
         model::record *pLhs = NULL;
         if(it != map.end())
            pLhs = it->second;

         compare(pLhs,r);
      }
   }

   void merge(model::record& into, model::record& in)
   {
      // in wins all contests
      for(auto it=in.fields.begin();it!=in.fields.end();++it)
         into.fields[it->first] = it->second;
   }

   void add(model::record& r)
   {
      model::record copy;
      copy.fields = r.fields;
      m_pInto->records.push_back(copy);
   }

   void considerDelete(model::record& r)
   {
      bool wouldBeInView = m_pView->pSpec->pFilter->isPass(r);
      if(!wouldBeInView)
         return;

      // records that were in the view when it was mapped, but are no longer,
      // were deleted by the user

      auto markedForDeath = r.fields[m_pView->pSpec->keyField];
      for(auto it=m_pInto->records.begin();it!=m_pInto->records.end();++it)
      {
         auto kvalue = it->fields[m_pView->pSpec->keyField];
         if(markedForDeath == kvalue)
         {
            m_pInto->records.erase(it);
            return;
         }
      }

      throw std::runtime_error(std::string("ISE trying to delete record that should exist: " + markedForDeath));
   }

   model::view *m_pView;
   model::model *m_pInto;
};

tcatExposeTypeAs(mergeViewCommandImpl,iMergeViewCommandImpl);

} // anonymous namespace
} // namespace cmd
