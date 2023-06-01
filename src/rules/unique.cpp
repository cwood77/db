#include "../cmn/talias.hpp"
#include "../command/viewSpec.hpp"
#include "../model/api.hpp"
#include "../tcatlib/api.hpp"

namespace rules {

class uniqueRule : public model::iRule {
public:
   explicit uniqueRule(const std::string& fieldName) : m_fieldName(fieldName) {}

   virtual void check(model::view& v, model::iErrorReport& err)
   {
      std::map<std::string,size_t> counts;

      for(auto& r : v.model.records)
         counts[r.fields[m_fieldName]]++;

      for(auto it=counts.begin();it!=counts.end();++it)
      {
         if(it->second > 1)
         {
            auto& rpt = err.create();
            rpt << "'" << m_fieldName << "' has value '" << it->first << "' " << it->second << " times which is more than 1" << std::endl;
         }
      }
   }

private:
   std::string m_fieldName;
};

class uniqueRuleFactory : public cmd::iRuleFactory {
public:
   virtual model::iRule& create(const std::string& string)
   {
      if(string.empty())
         throw std::runtime_error("usage: 'rule: unique:<field>");
      return *new uniqueRule(string);
   }
};

tcatExposeTypeAs(uniqueRuleFactory,uniqueRuleFactory);
publishTypeAlias(rules,unique,uniqueRuleFactory);

} // namespace rules
