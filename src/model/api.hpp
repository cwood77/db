#ifndef ___model_api___
#define ___model_api___

#include <list>
#include <map>
#include <string>

namespace model {

class record {
public:
   std::map<std::string,std::string> fields;
};

class model {
public:
   std::list<record> records;
};

class iRule {};

class viewSpec {
public:
   std::string type;
   std::list<iRule*> rules;
   std::string filter;
   std::string sort;
   std::string fields;
};

class view { public: viewSpec s; model m; };

} // namespace model

#endif // ___model_api___
