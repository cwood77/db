#ifndef ___command_saveload_hpp___
#define ___command_saveload_hpp___

#include <iostream>

namespace model { class view; }

namespace cmd {

class iViewFormatter {
public:
   virtual ~iViewFormatter() {}
   virtual void format(model::view& v, std::ostream& s) = 0;
};

class iViewParser {
public:
   virtual ~iViewParser() {}
   virtual void parse(model::view& v, std::istream& s) = 0;
};

} // namespace cmd

#endif // ___command_saveload_hpp___
