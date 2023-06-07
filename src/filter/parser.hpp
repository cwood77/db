#ifndef ___filter_parser_h___
#define ___filter_parser_h___

#include "nodes.hpp"
#include <memory>

namespace filter {

class parser {
public:
   explicit parser(iNodeFactory& f) : m_nf(f) {}

   model::iFilterNode& parse(const std::string& x);

private:
   iNodeFactory& m_nf;
   std::unique_ptr<filterNode> m_pRoot;
};

} // namespace filter

#endif // ___filter_parser_h___
