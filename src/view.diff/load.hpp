#ifndef ___viewdiff_load_hpp___
#define ___viewdiff_load_hpp___

#include "../command/saveload.hpp"

namespace model { class record; }

namespace view_diff {

class parser : public cmd::iViewParser {
public:
   parser() : m_pView(NULL), m_pCurrRec(NULL) {}

   virtual void parse(model::view& v, std::istream& s);

private:
   void handleLine(const std::string& line);
   void handleMultilineField(const std::string& line);
   void handleField(const std::string& line);
   void handleGlobal(const std::string& line);
   bool tryHandleRule(const std::string& line);
   size_t measureRule(const std::string& line);

   model::view *m_pView;
   model::record *m_pCurrRec;
   std::string m_currField;
};

} // namespace view_diff

#endif // ___viewdiff_load_hpp___
