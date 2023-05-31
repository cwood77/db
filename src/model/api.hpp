#ifndef ___model_api___
#define ___model_api___

#include <list>
#include <map>
#include <string>

namespace model {

class view;

class record {
public:
   std::map<std::string,std::string> fields;
};

class model {
public:
   std::list<record> records;
};

class iErrorReport {
public:
   virtual std::istream& create() = 0;
};

class iRule {
public:
   virtual ~iRule() {}

   virtual void initialize(const std::string& string) = 0;

   virtual void unmap(view& v) = 0;
   virtual void map(view& v) = 0;
   virtual void check(view& v, iErrorReport& err) = 0;
};

class viewSpec {
public:
   virtual ~viewSpec()
   {
      for(auto *pRule : rules)
         delete pRule;
   }

   std::string type;
   std::list<iRule*> rules;
   std::string filter;
   std::string sort;
   std::string fields;

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

} // namespace model

#endif // ___model_api___
