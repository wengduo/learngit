#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>


int main(int argc,char **argv)
{
    if(argc<2)
    {
	printf("touch error\n");
    }
    else
    {
	int i = 1;
	while(argv[i] != NULL)
	{
	    open(argv[i++],O_CREAT,0664);
	}
    }
    exit(0);
}
