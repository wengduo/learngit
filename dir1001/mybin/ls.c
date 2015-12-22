#include<stdio.h>
#include<stdlib.h>
#include<stdlib.h>
#include<assert.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<termios.h>
#include"file1.h"
#include<sys/ioctl.h>
#include<linux/fs.h>
int height= 0;//能输出几行
int count1 = 0;//格式化控制输出
int arr[200] = {0};
int brr[2000] = {0};

void get(int *w)
{
    struct termios ts;
    tcgetattr(STDIN_FILENO,&ts);
    struct winsize *ws;
    ws=(struct winsize*)malloc(sizeof(struct winsize));
    memset(ws,0x00,sizeof(struct winsize));
    ioctl(STDIN_FILENO,TIOCGWINSZ,ws);
    *w = ws->ws_col;
}

void my_printf2(char *p,int k)
{
    struct stat st;
    lstat(p,&st);
    if(S_ISUID & st.st_mode)
    {
        printf("\033[41;37m%s\033[0m  ",p);
        if(k == 1)
        {
            printf("\n");
            k--;
        }
	else
	{
	    int p2 = arr[count1] - strlen(p);
	    while(p2--)
	    {
		printf(" ");
	    }
	    count1++;
	}
    }
    else if(S_ISLNK(st.st_mode))
    {
	if(k == 0)
	{
	    printf("\033[1;36m%-*s\033[0m  ",arr[count1++],p);
	}
	else
	{
	    char buff[256] = {0};
	    printf("\033[1;36m%s\033[0m ",p);
	    if(readlink(p,buff,256) == -1)
            {
                 printf("error\n");
            }
	    arr[count1] = strlen(buff);
            printf("-> ");
	    k--;
	    my_printf2(buff,k);
	    printf("\n");
	}
    }
    else if(S_ISDIR(st.st_mode))
    {
        printf("\033[1;34m%-*s\033[0m  ",arr[count1++],p);
        if(k == 1)
        {
             printf("\n");
             k--;
        }
    }
    else if(S_ISCHR(st.st_mode)||S_ISBLK(st.st_mode))
    {
	printf("\033[40;33m%s\033[0m  ",p);
        if(k == 1)
        {
            printf("\n");
            k--;
        }
        else
        {
            int p2 = arr[count1] - strlen(p);
            while(p2--)
            {
                printf(" ");
            }
            count1++;
        }
    }
    else if(S_ISSOCK(st.st_mode))
    {
	printf("\033[1;35m%-*s\033[0m  ",arr[count1++],p);
        if(k == 1)
        {
            printf("\n");
            k--;
        }
    }
    else if(S_ISFIFO(st.st_mode))
    {
	printf("\033[40;33m%-*s\033[0m  ",arr[count1++],p);
	if(k == 1)
	{
	    printf("\n");
	    k--;
	}
    }
    else
    {
        if(!access(p,X_OK))
        {
             printf("\033[1;32m%-*s\033[0m  ",arr[count1++],p);
             if(k == 1)
             {
                 printf("\n");
                 k--;
             }
        }
        else
        {
             printf("%-*s  ",arr[count1++],p);
             if(k == 1)
             {
                 printf("\n");
                 k--;
 	     }
	}
    }
}

void my_ll(char *p,int k)
{
    struct tm *t = NULL;
    struct stat st;
    lstat(p,&st);
    char buff[256] = {0};
    int m = 0;
    char *p1 = ctime(&st.st_mtime);
    strtok(p1," ");
    p1 = strtok(NULL," ");
    switch(st.st_mode & S_IFMT)
    {
        case S_IFLNK:buff[m++] = 'l';break;
        case S_IFSOCK:buff[m++] = 's';break;
        case S_IFREG:buff[m++] = '-';break;
        case S_IFBLK:buff[m++] = 'b';break;
        case S_IFCHR:buff[m++] = 'c';break;
        case S_IFIFO:buff[m++] = 'p';break;
        case S_IFDIR:buff[m++] = 'd';break;
    }
    int i = 8;
    while(i >= 0)
    {
        if((st.st_mode) & 1<<i)
        {
            switch(i%3)
            {
                case 2:buff[m++] = 'r';break;
                case 1:buff[m++] = 'w';break;
                case 0:buff[m++] = 'x';break;
            }
        }
        else
        {
            buff[m++] = '-';
        }
        i--;
    }
    buff[m++] = '.';
    if(S_ISUID & st.st_mode)
    {
        if(strncmp(buff+3,"x",1) == 0)
	{
	    buff[3] = 's';
	}
	else
	{
	    buff[3] = 'S';
	}
    }
    if(S_ISGID & st.st_mode)
    {
	if(strncmp(buff+6,"x",1) == 0)
        {
            buff[6] = 's';
        }
        else
        {
            buff[6] = 'S';
        }
    }
    if(S_ISVTX & st.st_mode)
    {
	if(strncmp(buff+9,"x",1) == 0)
        {
            buff[9] = 't';
        }
        else
        {
            buff[9] = 'T';
        }

    }
    printf("%s",buff);
    printf(" %2ld ",(long)st.st_nlink);
    printf("%-7s %-7s ",getpwuid(st.st_uid)->pw_name,getgrgid(st.st_gid)->gr_name);
    printf("%6ld ",(long)st.st_size);
    t = localtime(&st.st_mtime);
    printf("%s %2d %02d:%02d ",p1,t->tm_mday,t->tm_hour,t->tm_min);
    k++;
    height = 0;
    my_printf2(p,k);
}

void paiban(NODE*phead, int maxsize,int count)
{
    int width = 0;
    int i = 0;
    int yushu = 0;
    int length1 = 0;
    get(&width);
    int kk = width/(maxsize+2);
    if(count < kk)
    {
	memcpy(arr,brr,sizeof(arr));
	return ;
    }
    else
    {
	if(count % kk == 0)
	{
	    yushu = count%kk;
	    height = count/kk;
	}
	else
	{
	    height = count/kk + 1;
	}
    }
    
    int kp = height;
    int kpp = 0;
    while(i < count)
    {
	int j = kp - 1;
	if(yushu > 0)
	{
	    yushu--;
	    kpp++;
	}
	if((yushu == 0)&&(kpp > 0))
	{
	    kp--;
	    yushu--;
	}	    
	int max = brr[i];
	while(j--)
	{
	    i++;
	    if(max < brr[i])
	    {
		max = brr[i];
	    }
	}
	arr[length1++] = max;
	i++;
    }
    return ;
}

void my_printf1(NODE *ph,int k)
{
    int i = height;
    while(i--)
    {
	NODE *ph1 = ph->next;
        while(ph != NULL)
	{
	    my_printf2(ph->d_name,k);
	    int j = height;
	    while(j--)
	    {
		ph = ph->next;
		if(ph == NULL)
		{
		    break;
		}
	    }
	}
	if(i>=1)
	{
	    printf("\n");
	}
	count1 = 0;
	ph = ph1;
    }
}
int main(int argc,char *argv[])
{
    struct stat st;
    struct inode *t;
    struct dirent *pp1 = NULL;
    DIR *dp;
    int k = 0;
    char path[256];
    getcwd(path,256);
    if(argc > 2)
    {
	if(strncmp(argv[2],"/",1) == 0)
	{
	    if((dp = opendir(argv[2])) != NULL)
	    {
	        chdir(argv[2]);
	    }
	    else
	    {
		chdir(argv[2]);
		my_ll(argv[2],k);
		exit(0);
	    }
	}
	else
	{
	    my_ll(argv[2],k);
	    exit(0);
	}
    }
    else if(argc == 2)
    {
	if(strncmp(argv[1],"/",1) == 0)
	{
	    if((dp = opendir(argv[1])) != NULL)
            {
                chdir(argv[1]);
            }
	}
	else
	{
	    dp = opendir(path);
	}
    }
    else
    {
        dp = opendir(path);
    }
    NODE *phead;
    list_init(&phead);
    char *buff[100] = {0};
    int count = 0;
    int k1 = 0;//文件大小
    int k2 = 0;
    int k3 = 0;
    int MAXSIZE = 0;
    while((pp1 = readdir(dp)) != NULL)
    {
	if((argc > 1)&&(strncmp(argv[1],"-a",2) == 0 || strcmp(argv[1],"-al") == 0))
	{
	    list_add_a(phead,pp1->d_name,0);
	    stat(pp1->d_name,&st);
            k3 += st.st_blocks;
	}
	else
	{
	    if(strncmp(pp1->d_name,".",1) == 0)
	    {
		continue;
	    }
	    list_add_a(phead,pp1->d_name,0);
	    stat(pp1->d_name,&st);
	    k3 += st.st_blocks;
	}
    }
    NODE *p1 = phead->next;
    while(p1 != NULL)
    {
	if(strlen(p1->d_name) > MAXSIZE)
	{
	   MAXSIZE = strlen(p1->d_name);
	}
	brr[count++] = strlen(p1->d_name);
	p1 = p1->next;
    }
    paiban(phead,MAXSIZE,count);
    if(argc > 1 && (strncmp(argv[1],"-l",2) == 0 || strcmp(argv[1],"-al") == 0))
    {
	printf("total %lld\n",(long long)k3/2);
    }
    NODE *pp = phead->next;
    while(pp != NULL)
    {
	if((argc > 1)&&(strncmp(argv[1],"-l",2) == 0 || strcmp(argv[1],"-al") == 0))
	{
	    my_ll(pp->d_name,k);
	}
	else
	{
	    if(height > 1)
	    {
	        my_printf1(pp,k);
	        break;
	    }
	    my_printf2(pp->d_name,k);
	}
	pp = pp->next;
    }
    if(argc<2 || (strcmp(argv[1],"-a") == 0 || strncmp(argv[1],"/",1) == 0))
    {
        printf("\n");
    }
    list_destory(phead);
    closedir(dp);
    exit(0);
}
