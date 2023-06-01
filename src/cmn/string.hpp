#ifndef ___cmn_string___
#define ___cmn_string___

#define WIN32_LEAN_AND_MEAN
#include <functional>
#include <list>
#include <set>
#include <string>
#include <windows.h>

namespace cmn {

inline std::wstring widen(const std::string& nstr)
{
   int len = nstr.size();
   std::wstring wstr(len + 1, 0);
   mbstowcs(&wstr[0], nstr.c_str(), len);
   return wstr.c_str();
}

typedef const char *parseThumbT;

inline bool startsWithAndAdvance(parseThumbT& pThumb, const std::string& pattern)
{
   if(::strncmp(pThumb,pattern.c_str(),pattern.length())==0)
   {
      pThumb += pattern.length();
      return true;
   }

   return false;
}

inline void eatWhitespace(parseThumbT& pThumb)
{
   for(;*pThumb==' ';++pThumb);
}

inline void splitCommaFunc(parseThumbT& pThumb, std::function<void(const std::string&)> f)
{
   while(true)
   {
      const char *pStart = pThumb;
      for(;*pThumb!=0&&*pThumb!=',';++pThumb);
      if(pStart!=pThumb)
      {
         std::string word(pStart,pThumb-pStart);
         f(word);
         pThumb++;
      }
      else
         break;
   }
}

inline void splitCommaSet(parseThumbT& pThumb, std::set<std::string>& words)
{
   splitCommaFunc(pThumb,[&](auto& w){ words.insert(w); });
}

inline void splitCommaList(parseThumbT& pThumb, std::list<std::string>& words)
{
   splitCommaFunc(pThumb,[&](auto& w){ words.push_back(w); });
}

} // namespace cmn

#endif // ___cmn_string___
