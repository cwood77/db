#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include "save.hpp"

namespace view_diff {

void formatter::format(model::view& v, std::ostream& s)
{
   for(auto& r : v.model.records)
   {
      s << "---------------------------" << std::endl;

      for(auto fname : v.pSpec->cols)
         s << fname << ": " << r.fields[fname] << std::endl;

      s << std::endl;
   }
}

tcatExposeTypeAs(formatter,formatter);

} // namespace view_diff
