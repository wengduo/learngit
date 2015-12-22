#include<stdio.h>
#include<stdlib.h>



int main()
{
    FILE *fd;
    fd = fopen("/home/wengdou/lianxi/myshell/dir1001/mybin/history1.c","r");
    int count = 1;
    char buff[100] = {0};
    while(fgets(buff,100,fd) != NULL)
    {
	    printf(" %d  %s",count,buff);
	    count++;
    }
    exit(0);
} 
