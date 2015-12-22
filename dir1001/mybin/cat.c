#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>
void file_copy(FILE * file1,FILE * file2)  
{  
    int c;  
  
    while((c = getc(file1)) != EOF)  
    {  
        putc(c,file2);  
    }  
}  
int main(int argc,char *argv[])  
{  
    FILE *fd;
    if(argc > 2 && strncmp(argv[1],"-n",2) == 0)
    {
	fd = fopen(argv[2],"r");
        int count = 1; 
        char buff[256] = {0};
        while(fgets(buff,100,fd) != NULL)
        {
           printf("  %d  %s",count,buff);
	   memset(buff,0,255);
           count++;
        }   
	fclose(fd);
    }	
    else
    {  
        if(argc == 1)
        {  
            printf("cat error\n");
        }  
        else  
        {
            while(-- argc > 0)  
            {  
                if((fd = fopen(*++argv,"r")) == NULL)  
                {  
                    printf("no such file %s\n",*argv);  
                    exit(0);  
                }  
                else  
                {  
                    file_copy(fd,stdout);   
                    fclose(fd);  
                }  
	    }
        } 
    } 
    exit(0);  
}  

