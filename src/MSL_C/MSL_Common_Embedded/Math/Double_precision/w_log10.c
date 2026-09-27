#include "cmath"

// These SDK entry points are retained in the retail binary for the product.sel export symbol table.
#pragma push
#pragma force_active on
double log10(double x) {
    return __ieee754_log10(x);
}
#pragma pop
