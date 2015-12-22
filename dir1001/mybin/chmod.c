#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<math.h>

int my_pow(int p,int s)
{
    if(s == 0)
    {
	return 1;
    }
    int sum = 1;
    while(s--)
    {
	sum *= p;
    }
    return sum;
}
int main(int argc,char **argv)
{
    if(argc < 3)
    {
	printf("Try `chmod --help' for more information.\n");
	exit(0);
    }
    if(strlen(argv[1]) < 5)
    {
	int k = strlen(argv[1]);
        int sum = 0;
	int i= 0;
	while(k)
	{
	    sum = sum + (argv[1][k-1]-'0')*(my_pow(8,i++));
	    k--;
	}
	printf("%d\n",sum);
	if(chmod(argv[2],sum) == -1)
	{
	    printf("chmod error\n");
	}
    }
    exit(0);
}
