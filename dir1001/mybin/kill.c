#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>  
#include <signal.h>  
#include <sys/types.h>  
#include <sys/wait.h>
int my_pow(int m, int n)
{
    if (n < 0)
    {
        return -1;
    }
    else if (n == 0)
    {
        return 1;
    }
    else
    {
        while (--n)
        {
            m *= 10;
        }
    }
    return m;
}
int my_atoi(char *p)
{
    int sum = 0;
    int k = strlen(p)-1;
    while (*p)
    {
        int j = (*p - '0')* my_pow(10,k--);
        sum += j;
        p++;
    }
    return sum;
}



int main(int argc,char **argv)
{
    if(argc == 2)
    {
	pid_t pid = atoi(argv[1]);
        if(kill(pid,SIGTERM) == -1)
        {
	    printf("kill error\n");
        }
    }
    else if(argc == 3)
    {
	int k = atoi(argv[1]);
	if(k == 9)
	{
	    pid_t pid = atoi(argv[2]);
            if(kill(pid,SIGKILL) == -1)
            {
                printf("kill error\n");
            }
	}

    }
    exit(0);
} 

