//g++ printf_macro.cpp -o printf_macro

#include <cstdio>   // printf

#define FIBER_LOG(GLOBAL_LEVEL, LEVEL, ...)		\
		do {										\
			if (LEVEL <= GLOBAL_LEVEL)				\
				printf (__VA_ARGS__);						\
		} while (0)
	

int main()
{
	FIBER_LOG(10, 5, "some msg, num = %d, float = %f \n\n\n", 10, 88.8);
	return 0;
}