#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include "save.hpp"

namespace view_diff {

void formatter::format(model::view& v, std::ostream& s)
{
   for(auto& r : v.model.records)
   {
      s << "---------------------------" << std::endl;

      for(auto it=r.fields.begin();it!=r.fields.end();++it)
         s << it->first << ": " << it->second << std::endl;

      s << std::endl;
   }
}

tcatExposeTypeAs(formatter,formatter);

} // namespace view_diff
