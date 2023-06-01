#include "../cmn/string.hpp"
#include "../cmn/talias.hpp"
#include "../command/viewSpec.hpp"
#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include <memory>
#include <set>

namespace rules {

class enumRule : public model::iRule {
public:
   explicit enumRule(const std::string& fieldName) : m_fieldName(fieldName) {}

   virtual void check(model::view& v, model::iErrorReport& err)
   {
      for(auto& r : v.model.records)
      {
         auto v = r.fields[m_fieldName];
         if(legalWords.find(v)==legalWords.end())
         {
            auto& rpt = err.create();
            rpt << "'" << m_fieldName << "' has illegal value '" << v << "'" << std::endl;
         }
      }
   }

   std::set<std::string> legalWords;

private:
   std::string m_fieldName;
};

class enumRuleFactory : public cmd::iRuleFactory {
public:
   virtual model::iRule& create(const std::string& string)
   {
      if(string.empty())
         throw std::runtime_error("usage: 'rule: enum:<field>=<value-list>");

      const char *pThumb = string.c_str();
      for(;*pThumb!=0&&*pThumb!='=';++pThumb);
      if(*pThumb!='=')
         throw std::runtime_error(std::string("can't find = in enum rule: ") + string);
      std::string field(string.c_str(),pThumb-string.c_str());
      std::unique_ptr<enumRule> pRule(new enumRule(field));

      cmn::splitCommaSet(++pThumb,pRule->legalWords);

      if(pRule->legalWords.size() == 0)
         throw std::runtime_error("no values are indicated in enum rule");

      return *pRule.release();
   }
};

tcatExposeTypeAs(enumRuleFactory,enumRuleFactory);
publishTypeAlias(rules,enum,enumRuleFactory);

} // namespace rules
