#ifndef ___viewdiff_load_hpp___
#define ___viewdiff_load_hpp___

#include "../command/saveload.hpp"

namespace view_diff {

class parser : public cmd::iViewParser {
public:
   virtual void parse(model::view& v, std::istream& s);
};

} // namespace view_diff

#endif // ___viewdiff_load_hpp___
