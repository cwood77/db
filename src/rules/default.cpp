#define WIN32_LEAN_AND_MEAN
#include "../cmn/string.hpp"
#include "../cmn/talias.hpp"
#include "../command/viewSpec.hpp"
#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include <windows.h>

namespace rules {

class defaultRule : public model::iRule {
public:
   defaultRule(const std::string& fieldName, const std::string& defValue)
   : m_fieldName(fieldName), m_defValue(defValue) {}

   virtual void unmap(model::view& v)
   {
      for(auto& r : v.model.records)
         if(r.getIf(m_fieldName).empty())
            r.fields[m_fieldName] = m_defValue;
   }

private:
   std::string m_fieldName;
   std::string m_defValue;
};

class defaultRuleFactory : public cmd::iRuleFactory {
public:
   virtual model::iRule& create(const std::string& string)
   {
      if(string.empty())
         throw std::runtime_error("usage: 'rule: default:<field>=<value>");

      std::vector<std::string> words;
      const char *pThumb = string.c_str();
      cmn::splitDelimVector(pThumb,'=',words);
      if(words.size() != 2)
         throw std::runtime_error("usage: 'rule: default:<field>=<value> [2]");

      return *new defaultRule(words[0],words[1]);
   }
};

tcatExposeTypeAs(defaultRuleFactory,defaultRuleFactory);
publishTypeAlias(rules,default,defaultRuleFactory);

} // namespace rules

tcatImplServer();

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
