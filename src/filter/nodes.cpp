#include "../tcatlib/api.hpp"
#include "nodes.hpp"
#include "parser.hpp"

namespace filter {

filterNode& filterNode::tail()
{
   if(m_pNext)
      return m_pNext->tail();
   else
      return *this;
}

std::string filterStack::popString()
{
   std::string v = m_stack.front();
   m_stack.pop_front();
   return v;
}

void filterStack::pushString(const std::string& v)
{
   m_stack.push_front(v);
}

bool filterStack::popBool()
{
   auto x = popString();
   if(x == "true")
      return true;
   else if(x == "false")
      return false;
   else
      throw std::runtime_error("stack expected bool but got: " + x);
}

void filterStack::pushBool(bool v)
{
   if(v)
      pushString("true");
   else
      pushString("false");
}

void nodeFactory::chain(std::unique_ptr<filterNode>& pRoot, types t, const std::string& arg)
{
   switch(t)
   {
      case kStringLiteral:
         demandNonEmpty(arg);
         chain(pRoot,new literalNode(arg));
         break;
      case kFieldName:
         demandNonEmpty(arg);
         chain(pRoot,new fieldNode(arg));
         break;
      case kEquals:
         demandEmpty(arg);
         chain(pRoot,new equalsNode);
         break;
      case kNot:
         demandEmpty(arg);
         chain(pRoot,new notNode);
         break;
      case kAnd:
         demandEmpty(arg);
         chain(pRoot,new andNode);
         break;
      case kOr:
         demandEmpty(arg);
         chain(pRoot,new orNode);
         break;
      default:
         throw std::runtime_error("unimplemented filter node type");
   }
}

void nodeFactory::chain(std::unique_ptr<filterNode>& pRoot, filterNode *pNoob)
{
   if(pRoot.get() == NULL)
      pRoot.reset(pNoob);
   else
      pRoot->chainNext(*pNoob);
}

void nodeFactory::demandEmpty(const std::string& arg)
{
   if(!arg.empty())
      throw std::runtime_error("filter node factory expected no arg for this node type");
}

void nodeFactory::demandNonEmpty(const std::string& arg)
{
   if(arg.empty())
      throw std::runtime_error("filter node factory expected an arg for this node type");
}

tcatExposeTypeAs(filterStack,model::iFilterStack);

} // namespace filter
