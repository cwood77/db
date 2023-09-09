#include "../model/api.hpp"
#include "../tcatlib/api.hpp"
#include "save.hpp"
#include <cstring>

namespace view_diff {

void formatter::format(model::view& v, std::ostream& s)
{
   for(auto& r : v.model.records)
   {
      s << "---------------------------" << std::endl;

      for(auto fname : v.pSpec->cols)
      {
         s << fname;

         auto& value = r.fields[fname];
         if(::strchr(value.c_str(),'\n'))
         {
            s << ":<" << r.fields[fname] << std::endl << ">" << std::endl;
         }
         else
            s << ": " << r.fields[fname] << std::endl;
      }

      s << std::endl;
   }
}

tcatExposeTypeAs(formatter,formatter);

} // namespace view_diff
