#ifndef ___model_api___
#define ___model_api___

#include <list>
#include <map>
#include <string>

namespace model {

class view;

class record {
public:
   std::string getIf(const std::string& name) const
   {
      auto it = fields.find(name);
      if(it == fields.end())
         return "";
      else
         return it->second;
   }

   std::map<std::string,std::string> fields;
};

class model {
public:
   std::list<record> records;
};

class iErrorReport {
public:
   virtual std::ostream& create() = 0;
};

class iRule {
public:
   virtual ~iRule() {}

   virtual void unmap(view& v) {}
   virtual void map(view& v) {}
   virtual void check(view& v, iErrorReport& err) {}
};

class iFilter {
public:
   virtual ~iFilter() {}

   virtual bool isPass(const record& r) const = 0;
};

class viewSpec {
public:
   viewSpec() : pFilter(NULL) {}

   virtual ~viewSpec()
   {
      for(auto *pRule : rules)
         delete pRule;
      delete pFilter;
   }

   std::string type;
   std::list<iRule*> rules;
   iFilter *pFilter;
   std::list<std::string> cols;

   std::string keyField;

   std::string parserTypeName;
   std::string formatterTypeName;
};

class view {
public:
   view() : pSpec(NULL) {}
   virtual ~view() { delete pSpec; }

   viewSpec *pSpec;
   model model;
};

// ============================= filter nodes =============================
// ========================================================================

namespace filter {

class always : public iFilter {
public:
   virtual bool isPass(const record& r) const { return true; }
};

} // namespace filter
} // namespace model

#endif // ___model_api___
