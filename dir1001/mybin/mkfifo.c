#include<stdio.h>
#include<sys/stat.h>
#include<assert.h>
#include<stdlib.h>

int main(int argc,char **argv)
{
    if(argc < 2)
    {
        printf("Try `mkfifo --help' for more information.\n");
	exit(0);
    }
    if(mkfifo(argv[1],0664))
    {
	printf("error\n");
    }
    exit(0);
}
