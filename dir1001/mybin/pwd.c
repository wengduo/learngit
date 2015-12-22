#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<dirent.h>
#include<sys/types.h>

int main(int argc,char **argv,char **envp)
{
    if(argc == 2)
    {
	printf("%s\n",argv[1]);
    }
    else
    {
	printf("error\n");
    }
    exit(0);
}
