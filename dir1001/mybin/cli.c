#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <termios.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include "file1.h"

#define        LS        1
#define        CP        2
#define        RM        3
#define        MV        4
#define        MKDIR     5
#define        GET       6
#define        HELP      8
#define        ERR       7
int height = 0;
int count1 = 0;
int arr[100] = {0};
int brr[1000] = {0};

int get()
{
    struct termios ts;
    tcgetattr(STDIN_FILENO,&ts);
    struct winsize *ws;
    ws=(struct winsize*)malloc(sizeof(struct winsize));
    memset(ws,0x00,sizeof(struct winsize));
    ioctl(STDIN_FILENO,TIOCGWINSZ,ws);
    int w = ws->ws_col; 
    return w;
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
 
int get_cmd(char* s )
{
    if ( s == NULL )
    {
        return ERR;
    }

    if ( strcmp(s,"ls") == 0 )
    {
        return LS;
    }
    else if ( strcmp(s,"cp") == 0 )
    {
        return CP;
    }
    else if ( strcmp(s,"rm") == 0 )
    {
        return RM;
    }
    else if ( strcmp(s,"mv") == 0 )
    {
        return MV;
    }
    else if ( strcmp(s,"mkdir") == 0 )
    {
        return MKDIR;
    }
    else if ( strcmp(s,"help") == 0 )
    {
        return HELP;
    }
    else if ( strcmp(s,"get") == 0 )
    {
        return GET;
    }

    return ERR;
}

void my_printf2(NODE *p)
{
    if(p->size == 1)
    {
        printf("\033[1;34m%-*s\033[0m  ",arr[count1++],p->d_name);
    }
    else if(p->size == 2)
    {
        printf("\033[1;32m%-*s\033[0m  ",arr[count1++],p->d_name);
    }
    else if(p->size == 3)
    {
        printf("%-*s  ",arr[count1++],p->d_name);
    }
}

void my_printf1(NODE *ph)
{
    int i = height;
    while(i--)
    {
        NODE *ph1 = ph->next;
        while(ph != NULL)
        {

            my_printf2(ph);
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

void paiban(NODE *phead,int maxsize,int count)
{
    int i = 0;
    int yushu = 0;
    int length1 = 0;
    int width = get();
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

void my_rm(int sockfd,char *myargv[])
{
    char buff[1024] = {0};

    strcpy(buff,myargv[0]);

    int i = 1;
    while( myargv[i] != NULL )
    {
        strcat(buff,"#");
        strcat(buff,myargv[i]);
        i++;
    }

    send(sockfd,buff,strlen(buff),0);

    memset(buff,0,1000);

    int n = recv(sockfd,buff,1000,0);
    if ( n == 0 )
    {
        printf("ser close\n");
    }
    char *p = strtok(buff,"#");
    if(strcmp(p,"0") == 0)
    {
        printf("success\n");
    }
    else
    {
        printf("failue\n");
    }

}

void my_get(int sockfd,char *myargv[])
{
    char buff[1024] = {0};

    strcpy(buff,myargv[0]);

    int i = 1;
    while( myargv[i] != NULL )
    {
        strcat(buff,"#");
        strcat(buff,myargv[i]);
        i++;
    }

    send(sockfd,buff,strlen(buff),0);
    FILE *in = fopen(myargv[1],"a+b");
    while (1)
    {
        memset(buff,0,1000);

        int n = recv(sockfd,buff,1000,0);
        if ( n == 0 )
        {
            printf("ser close\n");
            break;
        }
        if(strcmp(buff,"over") == 0 )
        {
            printf("get success\n");
            break;
        }
        char *p = strtok(buff,"#");
        if(strcmp(p,"0") == 0)
        {
            //open(myargv[1],0644);
            fwrite(buff,sizeof(char),1000,in);
            //close(in);
        }
        else
        {
            printf("get failue\n");
            break;
        }
    }
}

void my_mkdir(int sockfd,char *myargv[])
{
    char buff[1024] = {0};

    strcpy(buff,myargv[0]);

    int i = 1;
    while( myargv[i] != NULL )
    {
        strcat(buff,"#");
        strcat(buff,myargv[i]);
        i++;
    }

    send(sockfd,buff,strlen(buff),0);

    memset(buff,0,1000);

    int n = recv(sockfd,buff,1000,0);
    if ( n == 0 )
    {
        printf("ser close\n");
    }
    char *p = strtok(buff,"#");
    if(strcmp(p,"0") == 0)
    {
        printf("success\n");
    }
    else
    {
        printf("failue\n");
    }
}
void send_cmd_ls(int sockfd,char *myargv[])
{
    NODE *phead;
    list_init(&phead);

    char buff[1024] = {0};

    strcpy(buff,myargv[0]);

    int i = 1;
    while( myargv[i] != NULL )
    {
        strcat(buff,"#");
        strcat(buff,myargv[i]);
        i++;
    }

    send(sockfd,buff,strlen(buff),0);

    memset(buff,0,1000);

    int n = recv(sockfd,buff,1000,0);
    if ( n == 0 )
    {
        printf("ser close\n");
        return;
    }
  
    char buff1[512] = {0};
    
    char *p = strtok(buff,"#");

    if ( strcmp(p,"0") == 0 )
    {
        int max = 0;
        int count = 0;
        if( i == 1 || (i==2&&strcmp(myargv[1],"-a") == 0))
        {
            while((p = strtok(NULL,"#")) != NULL)
            {
                char buff2[256] = {0};
                strcpy(buff2,p);
                char ch = buff2[strlen(buff2)-1];
                int k = ch -48;
                buff2[strlen(buff2)-2] = 0;
                list_add_a(phead,buff2,k);
                brr[count++] = strlen(buff2);
                if(strlen(buff2) > max)
                {
                    max = strlen(buff2);
                }
            }
            paiban(phead,max,count);
            if(height > 1)
            {
                my_printf1(phead);
            }
            else
            {
                NODE *p = phead->next;
                while(p != NULL)
                {
                    my_printf2(p);
                    p = p->next;
                }
            }
        }
        else
        {
            NODE *phead;
            list_init(&phead);
            p = strtok(NULL,"#");
            printf("%s\n",p);
            while(p = strtok(NULL,"#"))
            {
                list_add_a(phead,p,0);
            }
            NODE *pp = phead->next;
            while(pp != NULL)
            {
                char buff2[512] = {0};
                char buff3[512] = {0};
                char *s = strtok(pp->d_name,"$!");
                printf("%s ",s);
                s = strtok(NULL,"$!");
                strcpy(buff3,s);
                s = strtok(NULL,"$!");
                if(my_atoi(s) == 1)
                {
                    printf("\033[1;34m%s\033[0m\n",buff3);
                }
                else if(my_atoi(s) == 2)
                {
                    printf("\033[1;32m%s\033[0m\n",buff3);
                }
                else
                {
                    printf("%s\n",buff3);
                }
                pp = pp->next;
            }
        }
        if(i == 1)
        {
            printf("\n");
        }
    }
}

int main(int argc,char *argv[])
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    assert( sockfd != -1 );

    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6500);
    saddr.sin_addr.s_addr = inet_addr(argv[1]);

    int res = connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if( res == -1 )
    {
        printf("ip error\n");
        exit(0);
    }

    while( 1 )
    {
        printf("Connect %s>> ",argv[1]);
        fflush(stdout);

        char buff[128] = {0};
        fgets(buff,128,stdin);
        
        if ( strncmp(buff,"exit",4) == 0 )
        {
            break;
        }

        buff[strlen(buff)-1] = 0;

        char * myargv[10] = {0};
        
        char * p = strtok(buff," ");
        if ( p == NULL )
        {
            continue;
        }

        myargv[0] = p;

        int i = 1;
        for( ; i < 10; i++)
        {
            p = strtok(NULL," ");
            if ( p == NULL )
            {
                break;
            }
            myargv[i] = p;
        }

        int val = get_cmd(myargv[0]);
        switch( val)
        {
            case LS :
                send_cmd_ls(sockfd,myargv); 
                break;
            case RM :
                my_rm(sockfd,myargv);
                break;
            case CP :
            case MV :
            case MKDIR :
                my_mkdir(sockfd,myargv);
                break;

            case GET :
                my_get(sockfd,myargv);
                break;

            case HELP :
                break;
            case ERR :
                printf("run help\n");
                break;

        } 
    }

    close(sockfd);
}
