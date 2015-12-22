#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>


int main(int argc,char **argv)
{
    if(argc < 3)
    {
	printf("Try `ln --help' for more information.\n");
    }
    if(argc == 3)
    {
	if(link(argv[1],argv[2]) < 0)
	{
	    printf("Try `ln --help' for more information.\n");
        }
    }
    else if(argc == 4)
    {
	if(strncmp(argv[1],"-s",2) == 0)
	{
	    if(symlink(argv[2],argv[3]) < 0)
	    {
		printf("Try `ln --help' for more information.\n");
	    }
	}
    }
    exit(0);
}
