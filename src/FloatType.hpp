#ifndef INC_FLOATTYPE_hpp
#define INC_FLOATTYPE_hpp

#include <cfloat>  // DBL_MIN, DBL_MAX

#ifdef FLOATTYPE_SINGLE
typedef float  fdouble;
const char floatingpoint_type[] = "float";
#else
typedef double fdouble;
const char floatingpoint_type[] = "double";
#endif


#endif // INC_FLOATTYPE_hpp

// ********** End of file ***************************************
