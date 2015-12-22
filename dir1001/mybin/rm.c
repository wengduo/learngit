#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
void my_delete(char *path)
{
    struct stat st;
    DIR *pp1;
    struct dirent *ptr1;
    pp1 = opendir(path);
    chdir(path);
    while((ptr1 = readdir(pp1)) != NULL)
    {
	stat(ptr1->d_name,&st);
	if(strncmp(ptr1->d_name,".",1) == 0)
	{
	    continue;
	}
	if(S_ISDIR(st.st_mode))
	{
	    strcat(path,"/");
	    strcat(path,ptr1->d_name);
	    if(!rmdir(path))
	    {
		path[strlen(path)-strlen(ptr1->d_name)-1] = 0;
	        printf("success2\n");
	    }
	    else
	    {
		my_delete(path);
		path[strlen(path)-strlen(ptr1->d_name)-1] = 0;
	    }
	}
	else
	{ 
	    
	    if(unlink(ptr1->d_name) < 0)
            {
                printf("error\n");
            }
	}
    }
    rmdir(path);
    chdir("..");
    closedir(pp1);
}	
int main(int argc,char **argv)
{
    struct stat st;
    char path[256] = {0};
    getcwd(path,256);
    if(argc < 2)
    {
	printf("11111\n");
	printf("error\n");
    }
    else if(argc == 3)
    {
	if(strncmp(argv[1],"-r",2) == 0)
	{
	    char *p = argv[2];
	    if(strncmp(argv[2],"/",1) != 0)
	    {
		strcat(path,"/");
		strcat(path,argv[2]);
		p = path;
	    }
	    if(!rmdir(p))
            {
                printf("success2\n");
		exit(0);
            }
	    stat(p,&st);
	    if(S_ISDIR(st.st_mode))
	    {
	        my_delete(p);
	    }
	}
    }
    else
    {
	char *p = argv[1];
	if(strncmp(argv[1],"/",1) != 0)
	{
	    strcat(path,"/");
	    strcat(path,argv[1]);
	    p = path;
	}
	if(unlink(p) < 0)
	{
	    printf("error\n");
	}
    }
    exit(0);
}
