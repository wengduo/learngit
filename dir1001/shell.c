#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#define MAXLEN 128

typedef enum Switch
{
    PWD,
    LS,
    CD,
    CLEAR
}SwitchCmd;

char *CMD[MAXLEN]={NULL};
int cmdnum = 0;

SwitchCmd AnalyseCmd(char *cmd)
{
    if(0 == strcmp(cmd, "pwd"))
        return PWD;
    if(0 == strcmp(cmd, "cd"))
        return CD;
    if(0 == strcmp(cmd, "ls"))
        return LS;
    if(0 == strcmp(cmd, "clear"))
        return CLEAR;
}

void CheckCmd(char *cmd)
{
     int len = 0;
     char *tokencmd=strtok(cmd," ");
     while(tokencmd != NULL)
     {
        CMD[cmdnum++] = tokencmd;
        tokencmd=strtok(NULL," ");
     }   
}
 
void initcmd()
{
    int i = 0;
    for(i = 0; i < cmdnum; ++i)
    {
        CMD[i] = NULL;
    }
    cmdnum = 0;
}

void myFork()
{
    int pid = fork();
    if(0 == pid)
    {
        char arr[128]="./bin/";
        strcat(arr, CMD[0]);
        execv(arr,CMD);
        printf("%s\n", arr);
    }
    else
    {
        waitpid(pid);
    }
}

void main()
{
    char cmd[MAXLEN]={0};
    char tip[MAXLEN]={0};
    while(1)
    {
        strcpy(tip, "wangchen@localhost");
        printf("[%s]$ ", tip);
        initcmd();
        gets(cmd);
        CheckCmd(cmd);
        switch(AnalyseCmd(CMD[0]))
        {
            case PWD:
            {
                 char path[128] = {0};
                 getcwd(path, 127);
                 printf("%s\n", path);
                 break; 
            } 
            case CD:
            {
                chdir(CMD[1]);
                break;
            }
            case LS:
            case CLEAR:
            {
                myFork();
                break;
            }
            default:
            {
                 printf("command was not find\n");
                 break; 
            }
        }
    }
}
