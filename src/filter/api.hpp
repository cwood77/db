#ifndef ___filter_api_h___
#define ___filter_api_h___

#include <string>

namespace model { class viewSpec; }

namespace filter {

class iExpert {
public:
   virtual ~iExpert() {}
   virtual void configure(model::viewSpec& vs, const std::string& expr) = 0;
};

} // namespace filter

#endif // ___filter_api_h___
