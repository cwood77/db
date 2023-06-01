#include "../cmn/string.hpp"
#include "../cmn/talias.hpp"
#include "../command/viewSpec.hpp"
#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include <memory>
#include <set>

namespace rules {

class sorter {
public:
   explicit sorter(const std::list<std::string>& fields) : m_fields(fields) {}

   bool operator()(const model::record& lhs, const model::record& rhs)
   {
      for(auto& fname : m_fields)
      {
         bool isRev = (fname.c_str()[0] == '-');
         if(isRev)
            fname = fname.c_str()+1;

         auto lhsF = lhs.getIf(fname);
         auto rhsF = rhs.getIf(fname);
         if(lhsF != rhsF)
            return isRev ? (lhsF > rhsF) : (lhsF < rhsF);
      }

      return &lhs < &rhs;
   }

private:
   std::list<std::string> m_fields;
};

class sortRule : public model::iRule {
public:
   virtual void map(model::view& v)
   {
      sorter s(fields);
      v.model.records.sort(s);
   }

   std::list<std::string> fields;
};

class sortRuleFactory : public cmd::iRuleFactory {
public:
   virtual model::iRule& create(const std::string& string)
   {
      if(string.empty())
         throw std::runtime_error("usage: 'rule: sort:<field-list>");

      std::unique_ptr<sortRule> pRule(new sortRule);

      const char *pThumb = string.c_str();
      cmn::splitCommaList(pThumb,pRule->fields);
      if(pRule->fields.size() == 0)
         throw std::runtime_error("no fields are indicated in sort rule");

      return *pRule.release();
   }
};

tcatExposeTypeAs(sortRuleFactory,sortRuleFactory);
publishTypeAlias(rules,sort,sortRuleFactory);

} // namespace rules
