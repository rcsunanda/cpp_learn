#include <cstdlib>
#include <cstdio>

#ifndef hard_assert
#define hard_assert(c) { if(!(c)) {printf("\nhard_assert: %s:%d\n\n",__FILE__,__LINE__); abort();}}
#endif // hard_assert
