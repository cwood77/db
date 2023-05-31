#ifndef ___viewdiff_save_hpp___
#define ___viewdiff_save_hpp___

#include "../command/saveload.hpp"

namespace model { class record; }

namespace view_diff {

class formatter : public cmd::iViewFormatter {
public:
   virtual void format(model::view& v, std::ostream& s);
};

} // namespace view_diff

#endif // ___viewdiff_save_hpp___
