#include "api.hpp"
#include <functional>

namespace cmd {
namespace {

class mergeViewCommandImpl : public iMergeViewCommandImpl {
public:
   mergeViewCommandImpl() : m_pView(NULL), m_pInto(NULL) {}

   virtual void execute()
   {
      // ---merge and/or add
      // sweep into records and build a map based on the key
      // sweep in records and look for matches in the map
      compareEachRightToLeft(m_pInto->records,m_pView->model.records,
      [&](auto *pInto, auto *pIn)
      {
         if(pInto && pIn)
         {
            // if match
            //    merge records with in winning
            merge(*pInto,*pIn);
         }
         else if(pIn)
         {
            // if no match
            //    add record
            add(*pIn);
         }
      });

      return;
      // ---consider deleting some stuff
      // sweep in recods and build a map based on the key
      // sweep into records and look for matches in the map
      compareEachRightToLeft(m_pView->model.records,m_pInto->records,
      [&](auto *pIn, auto *pInto)
      {
         // if match
         //    noop
         if(!pIn && pInto)
         {
            // if no match
            //    see if this record would pass the filter in in's spec
            //       if so: delete this record from into
            //       if not: noop
            considerDelete(*pInto);
         }
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
      std::function<void(model::record*,model::record*)> compare)
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

         compare(pLhs,&r);
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
      // unimplemented
   }

   model::view *m_pView;
   model::model *m_pInto;
};

tcatExposeTypeAs(mergeViewCommandImpl,iMergeViewCommandImpl);

} // anonymous namespace
} // namespace cmd
