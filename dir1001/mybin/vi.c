#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include "file1.h"
#define RESET() printf("\033c")
#define CLEAR() printf("\033[1;1H\033[2J")

int sign = 0;
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

void show(NODE *phead,int n,int m)
{
    RESET();
    NODE *p = phead->next;
    int i = 0;
    int ha = list_length(phead) - 1;
    while(p != NULL)
    {
	if(i == ha)
	{
	    if(n == 0)
	    {
		if(m != 0)
		{
		    printf("%s\r\033[%dA",p->buff,m);
		}
		else
		{
		    printf("%s\r",p->buff);
		}
		return;
	    }
	    else if(m == 0)
	    {
		if(n == strlen(p->buff))
		{
		    printf("%s",p->buff);
		}
		else
		{
		    printf("%s\r\033[%dC",p->buff,n);
		}
	    }
	    else
	    {
	        printf("%s\r\033[%dA\033[%dC",p->buff,m,n);
	    }
	}
	else
	{
	    printf("%s",p->buff);
	}
	i++;
	p = p->next;
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
    while (i <= k)
    {
        p[i-1] =p[i];
        i++;
    }
    p[i] = 0;
}

my_copy(char *p,char *s,int n)
{
    int i = 0;
    int h = strlen(p);
    while(n < h-1)
    {
	s[i++] = p[n];
	p[n++] = 0;
    }
}

my_strcat(char *p,char *s,int n)
{
    int h = strlen(s)+1;
    int i = 0;
    while(h--)
    {
	p[n++] = s[i++];
    }
}

void show_buff(char *p ,int n)
{
    if(n >= strlen(p))
    {
	printf("\n\n\n\r%s",p);
    }
    else
    {
	printf("\n\n\n\r%s\033[%dD",p,(int)strlen(p)-n);
    }
}

void my_esc(NODE *phead,char *s)
{
    int i = 1;
    char ch;
    while(1)
    {
	ch = getchar();
	if(ch == 27)
	{
	    if((ch = getchar()) == 91)
	    {
		if((ch = getchar()) == 67||ch == 68)
		{
		    if(ch == 67 && i < strlen(s) && strlen(s) != 0)
                    {
                        i++;
                        show(phead,0,0);
			show_buff(s,i);
                    }
                    else if(ch == 68 && i > 0)
                    {
                        i--;
                        show(phead,0,0);
			show_buff(s,i);
                    }
                }
            }
	}
	else if(ch == 126)
	{
	    my_bdel(s,i+1);;
	}
	else if(ch == 127)
	{
	    my_bdel(s,i);
            i--;
	}
	else if(ch == 10)
	{
	    break;
	}
	else
	{
	    if(i < strlen(s) - 1)
	    {
		my_buff(s,strlen(s) - i,ch);
                i++;
	    }
	    else
	    {
	        s[i++] = ch;
	    }
	}
	show(phead,0,0);
	show_buff(s,i);
    }
}

void zhui_jia(NODE *phead,NODE *p,FILE *fw,char *s,int m,int i)
{
    char ch;
    //sleep(3);
    show(phead,i,m);
    while(1)
    {
        ch = getchar();
	if(ch == 27)
	{
	    if((ch = getchar()) == 91)
            {
                if((ch = getchar()) == 65||ch == 66||ch == 67||ch == 68)
                {
                    if(ch == 65 && m < list_length(phead) - 1)
                    {
                        p = p->prev;
                        m++;
                        if(i >= strlen(p->buff))
                        {
                            i = strlen(p->buff) - 1;
                        }
                        show(phead,i,m);
                    }
                    else if(ch == 66 && m > 0)
                    {
                        p = p->next;
                        m--;
                        if(i > strlen(p->buff) - 1)
                        {
                            if(m == 0)
                            {
                                i = strlen(p->buff);
                            }
                            else
                            {
                                i = strlen(p->buff) - 1;
                            }
                        }
                        show(phead,i,m);
                    }
                    else if(ch == 67 && i < strlen(p->buff) && strlen(p->buff) != 0)
                    {
			if(m > 0)
			{
			    if( i >= strlen(p->buff) - 1)
			    {
				i = strlen(p->buff) - 1;
			    }
			    else
			    {
                                i++;
			    }
                            show(phead,i,m);
			}
			else
			{
			    i++;
			    show(phead,i,m);
			}
                     }
                    else if(ch == 68 && i > 0)
                    {
                        i--;
                        show(phead,i,m);
                    }
                }
            }
	    else if(ch == 58)
            {
                show(phead,0,0);
                char buff[256] = {0};
                int j = 0;
		buff[j++] = ch;
		show(phead,0,0);
		printf("\n\n\n\r%s",buff);
		my_esc(phead,buff);
                if(strncmp(buff,":wq",3) == 0 && strlen(buff) == 3)
                {
		    while(p->next != NULL)
                    {
                        p = p->next;
                    }
		    if(strlen(p->buff) == 0)
                    {
                        list_delete(phead,list_length(phead));
                    }
		    else
                    { 
                        p->buff[i++] = '\n';
                    }

                    sign++;
                    NODE *p1 = phead->next;
                    while(p1 != NULL)
                    {
                        fwrite(p1->buff,sizeof(char),strlen(p1->buff),fw);
                        p1 = p1->next;
                    }
		    break;
                }
                else if(strncmp(buff,":q!",3) == 0 && strlen(buff) == 3)
                {
                    unlink(s);
		    break;
		}
		else if(strncmp(buff,":rm",3) == 0)
		{
		    char *p1 = strtok(buff," ");
		    p1 = strtok(NULL," ");
		    if(my_atoi(p1) >= 0 && my_atoi(p1) <= list_length(phead))
		    {
			list_delete(phead,my_atoi(p1));
		    }
		}
		show(phead,i,m);
	    }
	}
	else if(ch == 126)
        {
            if(i < strlen(p->buff)-1)
            {
                my_bdel(p->buff,i+1);
            }
	    else if(i > 0)
	    {
		my_bdel(p->buff,i);
		i--;
	    }
        }
        else if(ch == 127)
        {
            if(i > 0)
            {
                my_bdel(p->buff,i);
                i--;
            }
	    else if(i == 0 && strlen(p->buff) < 2 && m < list_length(phead) - 1)
	    {
		p = p->prev;
		list_delete(phead,list_length(phead) - m -1);
		i = strlen(p->buff) - 1;
	    }
	    else if(i == 0 && strlen(p->buff) > 1 && m < list_length(phead) - 1)
	    {
		p = p->prev;
		int j = strlen(p->buff) - 1;
		my_strcat(p->buff,p->next->buff,j);
		list_delete(phead,list_length(phead) - m -1);
		i = j;
	    }
	    show(phead,i,m);
        }
        else if(ch == 10)
        {
            list_add_c(phead,list_length(phead)-m);
	    my_copy(p->buff,p->next->buff,i);
	    p->buff[i++] = ch;
	    p->buff[i++] = 0;
            p = p -> next;
	    if(m > 0)
	    {
	        p->buff[strlen(p->buff)] = '\n';
	    }
            i = 0;
	    show(phead,i,m);
        }
	else
        {
            if(m > 0 || i < strlen(p->buff) - 1)
            {
                my_buff(p->buff,strlen(p->buff) - i,ch);
                i++;
            }
            else
            {
                p->buff[i++] = ch;
            }
	    show(phead,i,m);
        }
	show(phead,i,m);
    }
}

int main(int argc,char *argv[])
{
    RESET();
    struct termios old_opt,opt;
    tcgetattr(0,&old_opt);
    opt = old_opt;
    opt.c_lflag &= ~ECHO;
    opt.c_lflag &= ~ICANON;//本地模式，c_cflay,控制模式，c_oflag,输出模式c_iflay，输入模式
    tcsetattr(0,TCSANOW,&opt);
    NODE *phead;
    list_init(&phead);
    int m = 0;
    int i = 0;
    FILE *fr;
    FILE *fw;
    list_add_c(phead,0);
    NODE *p = phead->next;
    if((fr = fopen(argv[1],"r")) != NULL)
    {
	while(fgets(p->buff,255,fr) != NULL)
	{
	    list_add_c(phead,list_length(phead)-m);
	    p = p->next;
	}
	//p = p->prev;
	//list_delete(phead,list_length(phead));
	//i = strlen(p->buff) - 1;
	show(phead,i,m);
	//sleep(3);
	char buff[256] = "/home/wenfdou/lianxi/dir1001/mybin/1.c";
	fw = fopen(buff,"w+");
	zhui_jia(phead,p,fw,buff,m,i);
	if(sign == 1)
	{
	    unlink(argv[1]);
	    link(buff,argv[1]);
	    unlink(buff);
	}
    }
    else
    {
        open(argv[1],O_CREAT,0664);
	fw = fopen(argv[1],"wb");
	zhui_jia(phead,p,fw,argv[1],m,i);
    }
    list_destory(phead);
    tcsetattr(0,TCSANOW,&old_opt);
    RESET();
    exit(0);
}
