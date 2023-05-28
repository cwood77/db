#ifndef ___cmn_string___
#define ___cmn_string___

#define WIN32_LEAN_AND_MEAN
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

} // namespace cmn

#endif // ___cmn_string___
