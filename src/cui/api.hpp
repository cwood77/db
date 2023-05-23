#ifndef ___cui_api___
#define ___cui_api___

#include <cstdlib>

namespace cui { 

// --------------- simple atomics

class pnt {
public:
   pnt(size_t x, size_t y) : x(x), y(y) {}

   size_t x;
   size_t y;
};

} // namespace cui

#endif // ___cui_api___
