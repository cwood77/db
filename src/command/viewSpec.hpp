#ifndef ___command_viewSpec_hpp___
#define ___command_viewSpec_hpp___

#include <iostream>

namespace model { class viewSpec; }

namespace cmd {

class iViewSpecLineParser {
public:
   virtual ~iViewSpecLineParser() {}
   virtual void parse(model::viewSpec& vs, const std::string& line) = 0;
};

class iViewSpecParser : public iViewSpecLineParser {
public:
   virtual bool handlesType(const std::string& type) const = 0;
};

class iCommonViewSpecParser : public iViewSpecLineParser {};

class iTopViewSpecParser {
public:
   virtual ~iTopViewSpecParser() {}
   virtual void parse(model::viewSpec& vs, std::istream& s) = 0;
};

} // namespace cmd

#endif // ___command_viewSpec_hpp___
