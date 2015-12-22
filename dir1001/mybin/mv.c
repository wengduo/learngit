#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<stdlib.h>
#include<dirent.h>
#include<fcntl.h>
#include<sys/stat.h>
void my_mv2(char *p,char *s);
void my_strrev(char *arr, int m)
{
    char *p = arr;
    while (*p != 0)
    {
        p++;
    }
    p--;
    int i = 0;
    while (i<m / 2)
    {
        char tmp;
        tmp = *arr;
        *arr++ = *p;
        *p-- = tmp;
         i++;
    }
}
void my_mv(char *p,char *s)
{
    char c;
    int in;
    int out;
    struct stat st;
    char path[256] = {0};
    char path1[256] = {0};
    if(strncmp(s,"..",2) == 0)
    {
	getcwd(path1,256);
	chdir("..");
	getcwd(path,256);
	strcat(path,"/");
	strcat(path,p);
	s = path;
	chdir(path1);
    }
    stat(p,&st);
    if(link(p,s) == 0)
    {
	unlink(p);
    }
    else
    {
	printf("error\n");
    }
}	

void my_mv1(char *p,char *s)
{
    char path[256] = {0};
    char path1[256] = {0};
    getcwd(path1,256);
    if(strncmp(s,"..",2) == 0)
    {
        chdir("..");
	mkdir(p,00775);
        getcwd(path,256);
        strcat(path,"/");
        strcat(path,p);
        s = path;
        chdir(path1);
    }
    else
    {
	if(strncmp(s,"/",1) != 0)
	{
            if(mkdir(s,00775) == -1)
	    {
		printf("mkdir error\n");
		exit(0);
	    }
	    strcat(path1,"/");
	    strcat(path1,s);
	    s = path1;
	}
	else if(strncmp(s,"/home/wenfdou",13) == 0)
	{
	    if(mkdir(s,00775) == -1)
	    {
		printf("mkdir error\n");
		exit(0);
	    }
	}
	else
	{
	    printf("cp: cannot create directory `%s': Permission denied\n",s);
	    exit(0);
	}
    }
    my_mv2(p,s);
    getcwd(path1,256);
    strcat(path1,"/");
    strcat(path1,p);
    rmdir(path1);
}
void my_mv2(char *p,char *s)
{
    DIR *pp1;
    struct stat st;
    struct dirent *pp;
    pp1 = opendir(p);
    chdir(p);
    while((pp = readdir(pp1)) != NULL)
    {
	stat(pp->d_name,&st);
	if(strncmp(pp->d_name,".",1) == 0)
	{
	    continue;
	}
	char *s1 =s;
	strcat(s,"/");
	strcat(s,pp->d_name);
	if(S_ISDIR(st.st_mode))
	{
	    mkdir(s,00775);
	    my_mv2(pp->d_name,s);
	    rmdir(pp->d_name);
	}
	else
	{
	    my_mv(pp->d_name,s);
	}
	s[strlen(s)-strlen(pp->d_name)-1]=0;
    }
    chdir("..");
    closedir(pp1);
}
	
int main(int argc,char **argv)
{
    DIR *dp;
    struct stat st;
    struct dirent *p;
    if(argc < 2)
    {
	printf("error\n");
    }
    else if((dp = opendir(argv[1])) == NULL)
    {
	my_mv(argv[1],argv[2]);
    }
    else 
    {
        stat(argv[1],&st);
	if(S_ISDIR(st.st_mode))
	{
	    my_mv1(argv[1],argv[2]);
	}
    }		
    exit(0);
}
