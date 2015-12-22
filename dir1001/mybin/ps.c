#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<dirent.h>
#include<sys/stat.h>
#include<time.h>
#include<linux/fs.h>
#include<termios.h>
#include<sys/ioctl.h>
#include"file1.h"
int hz = 0;

//coredump file 
//gdb core.dump
typedef struct _NODEF
{
    char uid[200];
    char zt[30];
    char tty[20];
    char name[1024];
    char mname[256];
    char pri[10];
    char nice[10];
    int UID;
    int pid;
    int ppid;
    int cpu;
    int usetime;
    int startime;
    int utime[3];
    int stime[2];
}NODEF;

typedef struct _TIME
{
    int hour;
    int min;
    int sec;
}TIME;

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

int my_pow(int m, int n)
{
    if (n < 0)
    {
	return -1;
    }
    else if (n == 0)
    {
	return 1;
    }
    else
    {
	while (--n)
	{
	    m *= 10;
	}
    }
    return m;
}
int my_atoi(char *p)
{
    int sum = 0;
    p = strtok(p,".");
    int k = strlen(p)-1;
    while (*p)
    {
	if(strncmp(p,".",1) == 0)
	{
	    return sum+1;
	}
	int j = (*p - '0')* my_pow(10,k--);
	sum += j;
	p++;
    }
    return sum;
}

void my_name1(NODE *phead,NODEF *pk)
{
    FILE *fd;
    NODEF *pkk = pk;
    NODE *p = phead->next;
    char buff[256] = {0};
    char buff1[256] = {0};
    char buff3[256] = {0};
    int sign = 1;
    int count = 0;
    while(p != NULL)
    {
	    memset(buff3,0,strlen(buff3));
	    strcpy(buff3,p->d_name);
	    strcat(buff3,"cmdline");
	    memset(buff,0,strlen(buff));
	    memset(buff1,0,strlen(buff1));
	    sign = 1;
        fd = fopen(buff3,"r");
        count = 0;
        while(sign)
        {
	        memset(buff1,0,strlen(buff1));
	        fgets(buff1,255,fd);
	        sign = strlen(buff1);
	        strcat(buff1," ");
	        count = count + sign + 1;
	        strcat(buff,buff1);
	        fseek(fd,count,SEEK_SET);
        }
        strcpy(pkk->name,buff);
	    p = p->next;
	    pkk++;
	    fclose(fd);
    }
}

void my_name2(NODE *phead,NODEF *pk)
{
    FILE *fd;
    NODEF *pkk = pk;
    NODE *p = phead->next;
    char buff[256] = {0};
    char buff1[256] = {0};
    char *p1 = NULL;
    while(p != NULL)
    {
	    memset(buff,0,strlen(buff));
	    memset(buff1,0,strlen(buff1));
	    strcpy(buff,p->d_name);
	    strcat(buff,"stat");
	    fd = fopen(buff,"r");
	    fgets(buff1,255,fd);
	    p1 = strtok(buff1," ()");
	    p1 = strtok(NULL," ()");
	    strcpy(pkk->mname,p1);
	    if(strlen(pkk->name) < strlen(p1))
	    {
	        strcpy(pkk->name,"[");
	        strcat(pkk->name,p1);
	        strcat(pkk->name,"]");
	    }
	    pkk++;
	    p = p->next;
        fclose(fd);
    }
}

void my_ppid(NODE *phead,NODEF *pk)
{
    FILE *fd;
    NODEF *pkk = pk;
    NODE *p = phead->next;
    char buff[256] = {0};
    char buff1[256] = {0};
    char *p1 = NULL;
    int i = 0;
    while(p != NULL)
    {
        memset(buff,0,strlen(buff));
        memset(buff1,0,strlen(buff1));
        strcpy(buff,p->d_name);
        strcat(buff,"stat");
        fd = fopen(buff,"r");
        fgets(buff1,255,fd);
        p1 = strtok(buff1," ");
        p1 = strtok(NULL," ");
        p1 = strtok(NULL," ");
	    strcpy(pkk->zt,p1);
	    p1 = strtok(NULL," ");
	    pkk->ppid = my_atoi(p1);
        pkk++;
        p = p->next;
        fclose(fd);
    }
}

void my_prid(NODE *phead,NODEF *pk)
{
    FILE *fd;
    NODEF *pkk = pk;
    NODE *p = phead->next;
    char buff[256] = {0};
    char buff1[256] = {0};
    int i = 0;
    while(p != NULL)
    {
        memset(buff,0,strlen(buff));
        memset(buff1,0,strlen(buff1));
        strcpy(buff,p->d_name);
        strcat(buff,"stat");
        fd = fopen(buff,"r");
        fgets(buff1,255,fd);
        char *p1 = strtok(buff1," ");
	i = 17;
	while(i--)
	{
            p1 = strtok(NULL," ");
	}
        strcpy(pkk->pri,p1);
	    p1 = strtok(NULL,p1);
	    strcpy(pkk->nice,p1);
	    pkk->pid = p->size;
        pkk++;
        p = p->next;
        fclose(fd);
    }
}

void my_zhongduan(NODE *phead,NODEF *pk)
{
    FILE *fd;
    NODEF *pkk = pk;
    NODE *p = phead->next;
    char buff[256] = {0};
    char buff1[256] = {0};
    char buff2[256] = {0};
    int i = 0;
    int j = 0;
    int k = 0;
    while(p != NULL)
    {
        memset(buff,0,strlen(buff));
        memset(buff1,0,strlen(buff1));
        memset(buff2,0,strlen(buff2));
        strcpy(buff,p->d_name);
        strcat(buff,"stat");
        fd = fopen(buff,"r");
        fgets(buff1,255,fd);
        char *p1 = strtok(buff1," ");
	    k = 6;
	    while(k--)
	    {
            p1 = strtok(NULL," ");
	    }
	    i = my_atoi(p1);
        j = 0;
        if(i == 0)
        {
            strcpy(pkk->tty,"?");
        }
        else if(i < 34816)
        {
            j = i - 1024;
            strcpy(pkk->tty,"tty");
            char ch = j + '0';
            buff2[0] = ch;
            strcat(pkk->tty,buff2);
        }
        else
        {
            j = i - 34816;
            strcpy(pkk->tty,"pts/");
            char ch = j + '0';
            buff2[0] = ch;
            strcat(pkk->tty,buff2);
        }
        pkk++;
        p = p->next;
	    fclose(fd);
    }
}

void my_uid(NODE *phead,NODEF *pk)
{
    FILE *fd;
    NODEF *pkk = pk;
    NODE *p = phead->next;
    char buff[256] = {0};
    char buff1[256] = {0};
    char *p1 = NULL;
    int i;
    while(p != NULL)
    {
	i = 8;
        memset(buff,0,strlen(buff));
        memset(buff1,0,strlen(buff1));
        strcpy(buff,p->d_name);
        strcat(buff,"status");
        fd = fopen(buff,"r");
        if(fd == NULL)
        {
            printf("pppppp\n");
        }
	while(i--)
	{
            fgets(buff1,40,fd);
	}
        p1 = strtok(buff1,":     ");
        p1 = strtok(NULL,"      ");
        printf("lllllllll %s\n",p1);
	pkk->UID = my_atoi(p1);
        printf("oooo\n");
        switch(pkk->UID)
	    {
	    case 0:strcpy(pkk->uid,"root");
		    break;
	    case 29:strcpy(pkk->uid,"rpcuser");
		    break;
	    case 32:strcpy(pkk->uid,"rpc");
		    break;
	    case 42:strcpy(pkk->uid,"gdm");
		    break;
	    case 68:strcpy(pkk->uid,"68");
		    break;
	    case 70:strcpy(pkk->uid,"avahi");
		    break;
	    case 81:strcpy(pkk->uid,"dbus");
		    break;
	    case 89:strcpy(pkk->uid,"postfix");
		    break;
	    case 499:strcpy(pkk->uid,"rtkit");
		    break;
	    case 500:strcpy(pkk->uid,"wengdou");
		    break;
	    }
	    pkk++;	    
        p = p->next;
	    fclose(fd);
    }
}

void my_stime(NODE *phead,TIME *ptr)
{
    FILE *fd;
    fd = fopen("/proc/uptime","r");
    char buff[256] = {0};
    fgets(buff,255,fd);
    char *p1 = strtok(buff," ");
    int i = my_atoi(p1);
    ptr->hour -= i/3600;
    ptr->min -= (i%3600)/60;
    ptr->sec -= (i%3600)%60;
    if(ptr->sec < 0)
    {
	    ptr->sec += 60;
	    ptr->min -= 1;
    }
    if(ptr->min < 0)
    {
	    ptr->min += 60;
	    ptr->hour -= 1;
    }
    if(ptr->hour < 0)
    {
	    ptr->hour += 24;
    }
}

void my_stime1(NODE *phead,NODEF *pk,TIME *ptr)
{
    FILE *fd;
    NODE *p = phead->next;
    NODEF *pkk = pk;
    char buff[256] = {0};
    char buff1[256] = {0};
    int i = 0;
    int j = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;
    while(p != NULL)
    {
	    memset(buff,0,strlen(buff));
	    memset(buff1,0,strlen(buff1));
	    strcpy(buff,p->d_name);
	    strcat(buff,"stat");
	    fd = fopen(buff,"r");
	    fgets(buff1,255,fd);
	    char *p1 = strtok(buff1," ");
	    i = 21;
	    while(i--)
	    {
	        p1 = strtok(NULL," ");
	    }
	    j = my_atoi(p1);
	    pkk->startime = j;
	    j = j/hz;
	    hour = ptr->hour + j/3600;
        min = ptr->min + (j%3600)/60;
        sec = ptr->sec + (j%3600)%60;
	    if(sec >= 60)
	    {
	        sec -= 60;
	        min += 1;
	    }
	    if(min >= 60)
	    {
	        min -= 60;
	        hour +=1;
	    }
	    if(ptr->hour >= 24)
	    {
	        hour -= 24;
	    }
	    pkk->stime[0] = hour;
	    pkk->stime[1] = min;
	    pkk++;
	    p = p->next;
	    fclose(fd);
    }
}

void my_usetime(NODE *phead,NODEF *pk)
{
    FILE *fd;
    NODE *p = phead->next;
    NODEF *pkk = pk;
    char buff[256] = {0};
    char buff1[256] = {0};
    int i = 0;
    int j = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;
    while(p != NULL)
    {
        memset(buff,0,strlen(buff));
        memset(buff1,0,strlen(buff1));
        strcpy(buff,p->d_name);
        strcat(buff,"stat");
        fd = fopen(buff,"r");
        fgets(buff1,255,fd);
        i = 13;
        char *p1 = strtok(buff1," ");
        while(i--)
        {
            p1 = strtok(NULL," ");
        }
        char *s = p1;
	    p1 = strtok(NULL," ");
	    j = my_atoi(p1) + my_atoi(s);
	    pkk->usetime = j;
        j = j/hz;
        hour = j/3600;
        min = (j%3600)/60;
        sec = (j%3600)%60;
        if(sec >= 60)
        {
            sec -= 60;
            min += 1;
        }
	    if(min >= 60)
        {
            min -= 60;
            hour += 1;
        }
        pkk->utime[0] = hour;
        pkk->utime[1] = min;
	    pkk->utime[2] = sec;
        pkk++;
        p = p->next;
	    fclose(fd);
    }
}

void my_cpu(int count,NODEF *pk)
{
    FILE *fd;
    NODEF *pkk = pk;
    char buff[256] = {0};
    int i = 0;
    while(count)
    {
	    memset(buff,0,strlen(buff));
        fd = fopen("/proc/uptime","r");
        fgets(buff,255,fd);
        char *p1 = strtok(buff,". ");
        char *s = p1;
        p1 = strtok(NULL,". ");
        i = my_atoi(s)*100 + my_atoi(p1);
	    pkk->cpu = ((pkk->usetime)*100)/(i-(pkk->startime)+900);
	    pkk++;
	    count--;
	    fclose(fd);
    }
}
	
	
int main(int argc,char **argv)
{
    DIR *dp;
    NODE *phead;
    list_init(&phead);
    TIME *ptr = (TIME *)malloc(sizeof(TIME));
    hz = sysconf(_SC_CLK_TCK);
    time_t val_time;
    (void) time(&val_time);
    struct tm *get_r; 
    get_r = localtime(&val_time);
    ptr->hour = get_r->tm_hour;
    ptr->min = get_r->tm_min;
    ptr->sec = get_r->tm_sec;
    int width = 0;
    get(&width);
    struct stat st;
    struct dirent *p;
    int count = 0;
    char buff[256] = "/proc";
    char buff1[256] = {0};
    chdir(buff);
    dp = opendir(buff);
    strcat(buff,"/");
    while((p = readdir(dp)) != NULL)
    {
        strcpy(buff1,buff);
        stat(p->d_name,&st);
        if((strncmp(p->d_name,".",1) == 0) || (!S_ISDIR(st.st_mode))||('a'<= p->d_name[0] && p->d_name[0] <= 'z'))
        {
            continue;
        }
        strcat(buff1,p->d_name);
        strcat(buff1,"/");
        list_add_b(phead,buff1,my_atoi(p->d_name));
        count++;
    }
    NODEF *pk = (NODEF *)malloc(sizeof(NODEF)*count);
    NODEF *pk1 = pk;
    //printf("sncac\n");
    my_uid(phead,pk);
    my_name1(phead,pk);
    my_name2(phead,pk);
    my_prid(phead,pk);
    my_zhongduan(phead,pk);
    my_stime(phead,ptr);
    my_stime1(phead,pk,ptr);
    my_usetime(phead,pk);
    my_cpu(count,pk);
    my_ppid(phead,pk);
    if(argc == 1)
    {
	    printf(" PID    TTY	 TIME	CMD\n");
	    while(count--)
	    {
	        if(strncmp(pk->tty,"pts/",4) == 0)
	        {
		        printf(" %-5d  %-5s  %02d:%02d:%02d  %s\n",pk->pid,pk->tty,pk->utime[0],pk->utime[1],pk->utime[2],pk->mname);
	        }
	        pk++;
	    }
    }
    else
    {
	    if(argc == 2 && strncmp(argv[1],"-ef",3) == 0)
	    {
	        int k = width - 51;
	        if(k < 0)
	        {
	            k = 0;
	        }
	        printf("UID      PID   PPID   C  STIME TTY   TIME      CMD\n");
	        while(count--)
	        {
	            if(strlen(pk->name) > k)
	            {
		            pk->name[k] = 0;
	            }
	            printf("%-7s  %-5d  %-5d %d  %02d:%02d %-5s %02d:%02d:%02d %s\n",pk->uid,pk->pid,pk->ppid,pk->cpu,
		        pk->stime[0],pk->stime[1],pk->tty,pk->utime[0],pk->utime[1],pk->utime[2],pk->name);
	            pk++;
	        }
	    }
	    else if(argc ==2 && strcmp(argv[1],"-a") == 0)
	    {
	        printf(" PID    TTY        TIME  CMD\n");
	        while(count--)
	        {
		        if(strncmp(pk->tty,"pts",3) == 0 && strncmp(pk->mname,"bash",4) != 0)
		        {
		            printf(" %-5d  %-5s  %02d:%02d:%02d  %s\n",pk->pid,pk->tty,pk->utime[0],pk->utime[1],pk->utime[2],pk->mname);
		            break;
	    	    }
		        pk++;
	        }
	    }
	    else if(argc ==2 && strncmp(argv[1],"-al",3) == 0)
	    {
	        printf("S UID PID   PPID  C PRI NI TTY   TIME     CMD\n");
	        while(count--)
	        {
		        if(strncmp(pk->tty,"pts",3) == 0 && strncmp(pk->mname,"bash",4) != 0)
		        {
		            printf("%s %-3d %-5d %-5d %d %s %s %-5s %02d:%02d:%02d %s\n",pk->zt,pk->UID,pk->pid,pk->ppid,pk->cpu,pk->pri,pk->nice,pk->tty,
			        pk->utime[0],pk->utime[1],pk->utime[2],pk->mname);
		        }
		        pk++;
	        }
	    }
    }
    free(pk1);
    free(ptr);
    exit(0);
}
                                                                         
