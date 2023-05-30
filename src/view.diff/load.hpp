#ifndef ___viewdiff_load_hpp___
#define ___viewdiff_load_hpp___

#include "../command/saveload.hpp"

namespace model { class record; }

namespace view_diff {

class parser : public cmd::iViewParser {
public:
   parser() : m_pCurrRec(NULL) {}

   virtual void parse(model::view& v, std::istream& s);

private:
   void handleLine(model::view& v, const std::string& line);
   void handleField(model::view& v, const std::string& line);
   void handleGlobal(model::view& v, const std::string& line);
   size_t measureRule(const std::string& line);

   model::record *m_pCurrRec;
};

} // namespace view_diff

#endif // ___viewdiff_load_hpp___
