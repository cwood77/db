#ifndef ___filter_nodes_h___
#define ___filter_nodes_h___

#include "../model/api.hpp"
#include <deque>
#include <memory>

namespace filter {

class filterNode : public model::iFilterNode {
public:
   ~filterNode() { delete m_pNext; }

   virtual void evaluate(const model::record& r, model::iFilterStack& s) const
   {
      if(m_pNext)
         m_pNext->evaluate(r,s);
   }

   void chainNext(filterNode& n)
   { tail().m_pNext = &n; }

protected:
   filterNode() : m_pNext(NULL) {}

private:
   filterNode& tail();

   filterNode *m_pNext;
};

class fieldNode : public filterNode {
public:
   explicit fieldNode(const std::string& fieldName) : m_fieldName(fieldName) {}

   virtual void evaluate(const model::record& r, model::iFilterStack& s) const
   {
      s.pushString(r.getIf(m_fieldName));
      filterNode::evaluate(r,s);
   }

private:
   std::string m_fieldName;
};

class literalNode : public filterNode {
public:
   explicit literalNode(const std::string& value) : m_value(value) {}

   virtual void evaluate(const model::record& r, model::iFilterStack& s) const
   {
      s.pushString(m_value);
      filterNode::evaluate(r,s);
   }

private:
   std::string m_value;
};

class equalsNode : public filterNode {
public:
   virtual void evaluate(const model::record& r, model::iFilterStack& s) const
   {
      auto lhs = s.popString();
      auto rhs = s.popString();
      s.pushBool(lhs == rhs);
      filterNode::evaluate(r,s);
   }
};

class filterStack : public model::iFilterStack {
public:
   virtual std::string popString();
   virtual void pushString(const std::string& v);
   virtual bool popBool();
   virtual void pushBool(bool v);
   virtual size_t getDepth() const { return m_stack.size(); }

private:
   std::deque<std::string> m_stack;
};

class iNodeFactory {
public:
   enum types {
      kStringLiteral,
      kFieldName,
      kEquals
   };

   virtual void chain(std::unique_ptr<filterNode>& pRoot, types t, const std::string& arg = "") = 0;
};

class nodeFactory : public iNodeFactory {
public:
   virtual void chain(std::unique_ptr<filterNode>& pRoot, types t, const std::string& arg = "");

private:
   void chain(std::unique_ptr<filterNode>& pRoot, filterNode *pNoob);
   void demandEmpty(const std::string& arg);
   void demandNonEmpty(const std::string& arg);
};

} // namespace filter

#endif // ___filter_nodes_h___
