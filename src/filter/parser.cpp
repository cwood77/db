#include "../cmn/string.hpp"
#include "parser.hpp"

namespace filter {

model::iFilterNode& parser::parse(const std::string& x)
{
   const char *pThumb = x.c_str();
   while(*pThumb != 0)
   {
      if(*pThumb == '"')
      {
         pThumb++;
         const char *pStart = pThumb;
         cmn::eatUntil(pThumb,'"');
         std::string literal(pStart,pThumb-pStart);
         m_nf.chain(m_pRoot,iNodeFactory::kStringLiteral,literal);
         pThumb++;
      }
      else if(*pThumb == '.')
      {
         pThumb++;
         const char *pStart = pThumb;
         cmn::eatUntil(pThumb,' ');
         std::string word(pStart,pThumb-pStart);
         m_nf.chain(m_pRoot,iNodeFactory::kFieldName,word);
      }
      else if(cmn::startsWithAndAdvance(pThumb,"=="))
         m_nf.chain(m_pRoot,iNodeFactory::kEquals);
      else
         throw std::runtime_error(std::string("failed to parse here: ") + pThumb);

      cmn::eatWhitespace(pThumb);
   }

   if(m_pRoot.get() == NULL)
      throw std::runtime_error("usage: filter: <expr>");
   return *m_pRoot.release();
}

} // namespace filter
