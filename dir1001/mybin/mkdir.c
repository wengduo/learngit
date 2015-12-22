#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<assert.h>
#include<string.h>

void my_mkdir(char *p)
{
    mkdir(p,00775);
}
int main(int argc,char **argv)
{
    struct stat st;
    struct dirent *pdp;
    DIR *p;
    char buff[256] = {0};
    char *p1 = getcwd(buff,256);
    if(argc < 2)
    {
	printf("error\n");
    }
    else
    {
	*argv++;
	p = opendir(p1);
	while(*argv != NULL)
	{
	    int k = 0;
	    while((pdp = readdir(p)) != NULL)
	    {
		stat(pdp->d_name,&st);
		if(S_ISDIR(st.st_mode))
		{
		    if(strcmp(*argv,pdp->d_name) == 0)
		    {
			printf("mkdir: cannot create directory %s : File exists\n",*argv);
			k++;
		    }
		}
	    }
	    if(k == 0)
	    {
	        my_mkdir(*argv);
	    }
	    *argv++;
	}
    }
    closedir(p);
    exit(0);
}
