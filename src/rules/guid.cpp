#include "../cmn/guid.hpp"
#include "../cmn/talias.hpp"
#include "../command/viewSpec.hpp"
#include "../model/api.hpp"
#include "../tcatlib/api.hpp"

namespace rules {

class defaultGuidRule : public model::iRule {
public:
   explicit defaultGuidRule(const std::string& fieldName)
   : m_fieldName(fieldName) {}

   virtual void unmap(model::view& v)
   {
      for(auto& r : v.model.records)
      {
         if(r.getIf(m_fieldName).empty())
         {
            r.fields[m_fieldName] = cmn::guid::create();
         }
      }
   }

private:
   std::string m_fieldName;
};

class defaultGuidRuleFactory : public cmd::iRuleFactory {
public:
   virtual model::iRule& create(const std::string& string)
   {
      if(string.empty())
         throw std::runtime_error("usage: 'rule: default<guid>:<field>");

      return *new defaultGuidRule(string);
   }
};

tcatExposeTypeAs(defaultGuidRuleFactory,defaultGuidRuleFactory);
publishTypeAliasComplex(rules,defaultGuid,default<guid>,defaultGuidRuleFactory);

} // namespace rules
