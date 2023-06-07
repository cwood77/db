#ifndef ___model_api___
#define ___model_api___

#include "../tcatlib/api.hpp"
#include <list>
#include <map>
#include <stdexcept>
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

class iFilterStack {
public:
   virtual ~iFilterStack() {}
   virtual std::string popString() = 0;
   virtual void pushString(const std::string& v) = 0;

   virtual bool popBool() = 0;
   virtual void pushBool(bool v) = 0;

   virtual size_t getDepth() const = 0;
};

class iFilterNode {
public:
   virtual ~iFilterNode() {}

   virtual void evaluate(const record& r, iFilterStack& s) const = 0;
};

namespace filter {

class always : public iFilter {
public:
   virtual bool isPass(const record& r) const { return true; }
};

class when : public iFilter {
public:
   explicit when(iFilterNode& root) : m_pRoot(&root) {}
   virtual ~when() { delete m_pRoot; }

   virtual bool isPass(const record& r) const
   {
      tcat::typePtr<iFilterStack> stack;
      m_pRoot->evaluate(r,*stack);
      bool isPass = stack->popBool();
      if(stack->getDepth() != 0)
         throw std::runtime_error("filter evaluation failed to reach stack depth of unity");
      return isPass;
   }

private:
   iFilterNode *m_pRoot;
};

} // namespace filter

} // namespace model

#endif // ___model_api___
