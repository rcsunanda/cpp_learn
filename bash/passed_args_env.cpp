//g++ passed_args_env.cpp -o passed_args_env

#include <cstdio>

int main(int argc, char* argv[], char* envp[])
{
	printf("argc=%d\n", argc);
	
	printf("-----------\n");
	
	int i = 0;
	while (argv[i])
	{
		printf("argv[%d]=%s\n", i, argv[i]);
		++i;
	}
	
	
	printf("-----------\n");
	
	int j = 0;
	while (envp[j])
	{
		printf("envp[%d]=%s\n", j, envp[j]);
		++j;
	}
	
	
	return 0;
}