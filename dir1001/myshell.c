#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include"file1.h"
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<termios.h>
#include<fcntl.h>
#define ERR 0
#define CMD 1
#define RUN 2
#define EXIT 3
#define VI 4
#define GCC 5
#define YUNXING 6
#define BUG 7
#define PATH "/home/wengdou/lianxi/myshell/dir1001/mybin/"
#define PATHVI "/usr/bin/vim"
#define PATHGCC "/usr/bin/gcc"
char buff1[256] = "myshell";
char lujing[256] ="/home/wengdou/lianxi/myshell/dir1001/";
char shangjing[256] = {0};
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
int get_cmd(char *str)
{
    if(str == NULL)
    {
	    return ERR;
    }
    else if(strncmp(str,"exit",4)==0)
    {
	    return EXIT;
    }
    else if((strncmp(str,"cd",2) == 0)&&(strlen(str) == 2))
    {
	    return CMD;
    }
    //else if(strncmp(str,"vi",2) == 0)
    //{
	//return VI;
    //}
    else if(strncmp(str,"gcc",3) == 0)
    {
	    return GCC;
    }
    else if(strncmp(str,"./",2) == 0)
    {
	    return YUNXING;
    }
    else 
    {
	    return RUN;
    }
}

void my_cd(NODE *head,int cmd)
{
    struct stat st;
    DIR *dp;
    char *mgargv[10]={0};
    int i=0;
    for( ;i<list_length(head);i++)
    {
        mgargv[i] = list_getstr(head,i);
    }
    if(strncmp(mgargv[1],"..",2) != 0)
    {
	    if(strncmp(mgargv[1],"~",1) == 0 && strlen(mgargv[1]) == 1)
	    {
	        chdir("/home/wengdou/");
	        strncpy(shangjing,lujing,strlen(lujing));
	        strncpy(lujing,"/home/wengdou/",strlen("/home/wengdou/")+1);
	        //printf("%s\n%s\n%s\n",buff11,shangjing,lujing);
	        strncpy(buff1,"~",2);
	    }
	    else if(strncmp(mgargv[1],"-",1) == 0 && strlen(mgargv[1]) == 1)
	    {
	        chdir(shangjing);
	        char tmp[256] = {0};
	        strncpy(tmp,shangjing,strlen(shangjing)+1);
	        strncpy(shangjing,lujing,strlen(lujing)+1);
	        strncpy(lujing,tmp,strlen(tmp)+1);
	        if(strcmp(lujing,"/home/wengdou/") == 0)
	        {
		        strncpy(buff1,"~",2);
	        }
	        else
	        {
		        char tmp1[256] = {0};
		        strncpy(tmp1,lujing,strlen(lujing));
		        my_strrev(tmp1,strlen(tmp1));
		        char *p = strtok(tmp1,"/");
		        if(p == NULL)
		        {
		            strcpy(buff1,"/");
		        }
		        else
		        {
		            my_strrev(p,strlen(p));
		            strncpy(buff1,p,strlen(p)+1);
		        }
	        }
	    }
	    else if(strncmp(mgargv[1],"/",1) == 0&&strlen(mgargv[1]) == 1)
	    {
	        chdir("/");
	        strncpy(shangjing,lujing,strlen(lujing));
	        strcpy(lujing,"/");
	        strcpy(buff1,"/");
	    }
	    else if(strncmp(mgargv[1],"/",1) == 0)
	    {
	        if(opendir(mgargv[1]) == NULL)
	        {
		        printf("sh: cd: %s: Not a directory\n",mgargv[1]);
		        exit(0);
	        }
	        chdir(mgargv[1]);
	        strncpy(shangjing,lujing,strlen(lujing));
	        strncpy(lujing,mgargv[1],strlen(mgargv[1])+1);
	        char tmp1[256] = {0};
	        strncpy(tmp1,lujing,strlen(lujing)+1);
	        my_strrev(tmp1,strlen(tmp1));
	        char *p = strtok(tmp1,"/");
	        my_strrev(p,strlen(p));
	        if(p == NULL)
            {
                strcpy(buff1,"/");
            }
	        else if(strcmp(p,"wengdou") == 0)
	        {
		        strncpy(buff1,"~",2);
	        }
            else
	        {
                strncpy(buff1,p,strlen(p)+1);
            }
	    }
	    else
	    {
	        if(opendir(mgargv[1]) == NULL)
            {
                printf("sh: cd: %s: Not a directory\n",mgargv[1]);
		        exit(0);
            }
            chdir(mgargv[1]);
            strncpy(shangjing,lujing,strlen(lujing));
	        char tmp1[256] = {0};
            if(strcmp(lujing,"/home/wengdou/") == 0)
	        {
		        strcat(lujing,mgargv[1]);
	        }
	        else
	        {
		        strncat(lujing,"/",1);
		        strcat(lujing,mgargv[1]);
	        }
            if(strcmp(mgargv[1],"wengdou") == 0)
	        {
		        strncpy(buff1,"~",1);
	        }
	        else
	        {
		        strncpy(buff1,mgargv[1],strlen(mgargv[1])+1);
	        }
	    }	
    }
    else
    {
	    if(strlen(mgargv[1]) == 2)
	    {
	        strncpy(shangjing,lujing,strlen(lujing)+1);
	        if(strcmp(buff1,"~") == 0)
	        {
		        strcpy(buff1,"wengdou");
	        }
	        lujing[strlen(lujing)-strlen(buff1)-1] = 0;
	        if(strlen(lujing) == 0)
	        {
		        chdir("/");
		        strncpy(buff1,"/",2);
	        }
	        else if(strcmp(lujing,"/") == 0)
	        {
		        ;
	        }
	        else
	        {
	            chdir(lujing);
	            char tmp1[256] = {0};
	            strncpy(tmp1,lujing,strlen(lujing)+1);
	            my_strrev(tmp1,strlen(tmp1));
	            char *p1 = strtok(tmp1,"/");
	            my_strrev(p1,strlen(p1));
	            if(strcmp(p1,"wengdou") == 0)
	            {
		            strncpy(buff1,"~",2);
	            }
	            else
	            {
	                strncpy(buff1,p1,strlen(p1)+1);
	            }
	        }
	    }
    }
}

void my_yun(NODE *head,int cmd)
{
    char path[256] = {0};
    strcpy(path,lujing);
    strncat(path,"/",1);
    char *p = list_getstr(head,0);
    char *s = strtok(p,"/");
    s = strtok(NULL,"/");
    pid_t pid = fork();
    if(pid == 0)
    {
	    char *mgargv[10] = {0};
	    mgargv[0] = s;
        int i = 1;
        for(;i < list_length(head);i++)
        {
            mgargv[i] = list_getstr(head,i);
        }
        strncat(path,s,strlen(s)+1);
        execv(path,mgargv);
        perror("exec error");
        exit(0);
    }
    else
    {
        wait(NULL);
    }
}
void my_gcc(NODE *head,int cmd)
{
    pid_t pid = fork();
    if(pid == 0)
    {
	    char *mgargv[10] = {0};
        int i = 0;
        for(;i < list_length(head);i++)
        {
            mgargv[i] = list_getstr(head,i);
        }
        char path[256] = {0};
        strcpy(path,PATHGCC);
        execv(path,mgargv);
        perror("exec error");
        exit(0);
    }
    else
    {
        wait(NULL);
    }
}

void my_vi(NODE *head,int cmd)
{
    pid_t pid = fork();
    if(pid == 0)
    {
	char *mgargv[10] = {0};
	int i = 0;
	for(;i < list_length(head);i++)
 	{
	    mgargv[i] = list_getstr(head,i);
	}
	char path[256] = {0};
	strcpy(path,PATHVI);
	execv(path,mgargv);
	perror("exec error");
	exit(0);
    }
    else
    {
	    wait(NULL);
    }
}
void do_run(NODE *head,int cmd)
{
    pid_t pid = fork();
    if(pid == 0)
    {
	char *mgargv[10]={0};
	int i=0;
	for( ;i<list_length(head);i++)
	{
	    mgargv[i] = list_getstr(head,i);
	}
	if(strcmp(mgargv[0],"pwd") == 0)
	{
	    mgargv[1] = lujing;
	}
	char path[256] = {0};
	if(cmd == RUN)
	{
	    strcpy(path,PATH);
	}
	strcat(path,list_getstr(head,0));
	execv(path,mgargv);
	perror("exec error");
	exit(0);
    }
    else
    {
	    wait(NULL);
    }
}

void my_buff(char *p, int i,char ch)
{
	int k = strlen(p);
	i++;
	while (i--)
	{
		p[k + 1] = p[k];
		k--;
	}
	p[k+1] = ch;
}

void my_bdel(char *p, int i)
{
    int k = strlen(p);
    int h = k - i;
    while (k--)
    {
	    p[h] = p[h + 1];
	    h++;
    }
    p[h] = 0;
}
void my_zhongduan(char *p,char *s)
{
    char buff[100] = {""};
    FILE *fd;
    fd = fopen("/home/wengdou/lianxi/myshell/dir1001/mybin/history1.c","r");
    int count = 0;
    char tmp1[255] = {0};
    while(fgets(tmp1,100,fd) != NULL)
    {
        memset(tmp1,0,strlen(tmp1));
	    count++;
    }
    fclose(fd);
    char **temp = (char **)malloc(sizeof(char *)*count);
    fd = fopen("/home/wengdou/lianxi/myshell/dir1001/mybin/history1.c","r");
    count = 0;
    while(fgets(tmp1,100,fd) != NULL)
    {
	    char *p1 = (char *)malloc(sizeof(char)*strlen(tmp1)+2);
        tmp1[strlen(tmp1)-1] = 0;
        strcpy(p1,tmp1);
	    temp[count++] = p1;
        memset(tmp1,0,strlen(tmp1));
    }
    int count1 = count;
    fclose(fd);
    struct termios old_opt,opt;
    char ch;
    int i = 0;
    int j = 0;
    tcgetattr(0,&old_opt);
    opt = old_opt;
    opt.c_lflag &= ~ECHO;
    opt.c_lflag &= ~ICANON;//本地模式，c_cflay,控制模式，c_oflag,输出模式c_iflay，输入模式
    tcsetattr(0,TCSANOW,&opt);
    printf("[stu@localhost %s]$ ",p);
    int k = count-1;
    int n = 0;//光标位
    int m = 0;//手输
    while(1)
    {
        ch = getchar();
        if(ch == 27)
        {
            getchar();
	        if((ch = getchar()) == 65||ch == 66||ch == 67||ch == 68)
            {
                if(ch == 65)
                {
                    i = strlen(buff);
                    memset(buff,0,100);
                    strcpy(buff,temp[k]);
                    printf("\r[stu@localhost %s]$ ",p);
                    for(j = 0;j < i;j++)
                    {
                        printf(" ");
                    }
                    printf("\r[stu@localhost %s]$ %s",p,buff);
                    fflush(stdout);
                    k--;
		            n = 0;
		            m = 0;
                }
                else if(ch == 66 && k < count-1)
                {
		            k++;
                    i = strlen(buff);
                    memset(buff,0,100);
                    strcpy(buff,temp[k]);
                    printf("\r[stu@localhost %s]$ ",p);
                    for(j = 0;j < i;j++)
                    {
                        printf(" ");
                    }
                    printf("\r[stu@localhost %s]$ %s",p,buff);
                    fflush(stdout);
		            n = 0;
		            m = 0;
                }
		        else if(ch == 67 && n > 0)
		        {
		            i = strlen(buff);
		            if( n == 0 && m != 5)
		            {
                        memset(buff,0,100);
                        strcpy(buff,temp[k+1]);
		            }
                    printf("\r[stu@localhost %s]$ ",p);
                    for(j = 0;j < i;j++)
                    {
                        printf(" ");
                    }
                    printf("\r[stu@localhost %s]$ %s\033[%dD",p,buff,--n);
                    fflush(stdout);
		        }
		        else if(ch == 67 && n == 0)
		        {
		            printf("\r[stu@localhost %s]$ %s",p,buff);
		        }
		        else if(ch == 68)
		        {
		            i = strlen(buff);
		            if(n < i)
		            {
			            if(n == 0 && m != 5)
			            {
                            memset(buff,0,100);
                            strcpy(buff,temp[k+1]);
			            }
                        printf("\r[stu@localhost %s]$ ",p);
                        for(j = 0;j < i;j++)
                        {
                            printf(" ");
                        }
                        printf("\r[stu@localhost %s]$ %s\033[%dD",p,buff,++n);
                        fflush(stdout);
		            }
                }
            }
        }
	    else if(ch == 126)
	    {
	        i = strlen(buff);
            if(i != 0 && i>n)
            {
                if(n == 0)
                {
		            if( i == 3)
		            {
		                strcat(buff ,"~");
		            }
		            printf("\r[stu@localhost %s]$ %s",p,buff);
                }
                else
                {
                    my_bdel(buff,n);
		            n--;
                    i = strlen(buff);
                    for(j = 0;j < i+1;j++)
                    {
                        printf(" ");
                    }
                    fflush(stdout);
		            if(n == 0)
		            {
			            printf("\r[stu@localhost %s]$ %s",p,buff);
		            }
		            else
		            {
                        printf("\r[stu@localhost %s]$ %s\033[%dD",p,buff,n);
		            }
                }
                fflush(stdout);
                k--;
		        m = 5;
            }
	    /*else if(ch == 126)
	    {
		//buff[i] = "~";
		int i = strlen(buff);
		my_buff(buff,i,ch);
		printf("\r[stu@localhost %s]$ %s\033[%dD",p,buff,n);
		printf("pppp\n");
	    }*/
	    }
        else if(ch == 127)
        {
            i = strlen(buff);
	        if(i != 0 && i>n)
            {
		        if(n == 0)
		        {
                    buff[i-1] = '\0';
                    i--;
                    printf("\r[stu@localhost %s]$ ",p);
                    for(j = 0;j < i+1;j++)
                    {
                        printf(" ");
                    }
                    fflush(stdout);
                    printf("\r[stu@localhost %s]$ %s",p,buff);
		        }
		        else
		        {
		            my_bdel(buff,n+1);
		            i = strlen(buff);
		            for(j = 0;j < i+1;j++)
                    {
                        printf(" ");
                    }
		            fflush(stdout);
		            printf("\r[stu@localhost %s]$ %s\033[%dD",p,buff,n);
		        }
                fflush(stdout);
                k--;
            }
        }
        else if(ch == 10)
        {
            //printf("kcbabciavbic\n");
            break;
        }
	    else if(ch == 15)
	    {
	        printf("pppppppp\n");
	    }
        else
        {
            i = strlen(buff);
		
	        if(n != 0)
	        {
		        int h = n;
		        int j = i - n;
		        my_buff(buff,n,ch);
		        //n++;
		        printf("\r[stu@localhost %s]$ %s\033[%dD",p,buff,n);
	        }
	        else
	        {
                buff[i] = ch;
		        printf("\r[stu@localhost %s]$ %s",p,buff);
            }
	        m = 5;
        }
    }
    strcpy(s,buff);
    tcsetattr(0,TCSANOW,&old_opt);
    printf("\n");
    /*while(count1)
    {
	    free(temp[--count1]);
    }
    free(temp);*/
}

int main(int argc,char *argv[],char *envp[])
{
    NODE *head;
    list_init(&head);
    while(1)
    {
	    FILE *in;
	    char buff[256] = {0};
	    my_zhongduan(buff1,buff);
	    in = fopen("/home/wengdou/lianxi/myshell/dir1001/mybin/history1.c","a+b");
	    if(in == NULL)
        {
            printf("open error\n");
        }
        int i = 0;
	    buff[strlen(buff)] = 0;
	    strncat(buff,"\n",1);
        fwrite(buff,sizeof(char),strlen(buff),in);
	    fclose(in);
	    buff[strlen(buff)-1]=0;
	    char *ptr = strtok(buff," ");
	    if(ptr == NULL)
	    {
	        continue;
	    }
	    else
	    {
	        list_add(head,ptr);
	        while((ptr = strtok(NULL," ")) != NULL)
	        {
		        list_add(head,ptr);
	        }
	        char *s1=list_getstr(head,0);
	        int val = get_cmd(s1);
	        switch(val)
	        {
	        case CMD:
	            my_cd(head,val);
	            break;
	        case RUN:
	            do_run(head,val);
	            break;
	        case EXIT:
	            list_destory(head);
	            exit(0);
	        case VI:
	            my_vi(head,val);
	            break;
	        case GCC:
	            my_gcc(head,val);
	            break;
	        case YUNXING:
	            my_yun(head,val);
	            break;
	        }
	    }
	    list_clear(head);
    }
    exit(0);
}
	
