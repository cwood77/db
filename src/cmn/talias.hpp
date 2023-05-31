#ifndef ___cmn_talias_h___
#define ___cmn_talias_h___

#include "../tcatlib/api.hpp"
#include <map>
#include <stdexcept>
#include <string>

namespace cmn {

class iTypeAliasProvider {
public:
   virtual ~iTypeAliasProvider() {}
   virtual const std::string& getCategory() const = 0;
   virtual const std::string& getAlias() const = 0;
   virtual const std::string& getValue() const = 0;
};

template<class T>
class typeAliasProvider : public iTypeAliasProvider {
public:
   typeAliasProvider(const std::string& category, const std::string& alias)
   : m_category(category)
   , m_alias(alias)
   , m_value(typeid(T).name())
   {
   }

   virtual const std::string& getCategory() const { return m_category; }
   virtual const std::string& getAlias() const { return m_alias; }
   virtual const std::string& getValue() const { return m_value; }

private:
   std::string m_category;
   std::string m_alias;
   std::string m_value;
};

// args unquoted
#define publishTypeAlias(__cat__,__alias__,__type__) \
   class __alias__##typeAliasProvider : public cmn::typeAliasProvider<__type__> { \
   public: \
      __alias__##typeAliasProvider() \
      : cmn::typeAliasProvider<__type__>(#__cat__,#__alias__) {} \
   }; \
   tcatExposeTypeAs(__alias__##typeAliasProvider,cmn::iTypeAliasProvider)

class typeAliasTable {
public:
   typeAliasTable()
   {
      tcat::typeSet<iTypeAliasProvider> pProvs;
      for(size_t i=0;i<pProvs.size();i++)
      {
         auto *pProv = pProvs[i];
         m_table[pProv->getCategory()][pProv->getAlias()] = pProv->getValue();
      }
   }

   std::string demand(const std::string& category, const std::string& alias)
   {
      auto ans = m_table[category][alias];
      if(ans.empty())
         throw std::runtime_error(std::string("type alias unknown: ") + alias);
      return ans;
   }

private:
   std::map<std::string,std::map<std::string,std::string> > m_table;
};

} // namespace cmn

#endif // ___cmn_talias_h___
