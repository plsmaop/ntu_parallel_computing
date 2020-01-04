#include "VSQRT.h"
 
#include <math.h>
 
void sqrt2(float *begin, float *end) {
     for (; begin != end; begin++)
         *begin = sqrt(*begin);
}
