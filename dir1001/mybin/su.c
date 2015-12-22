#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>


int main(int argc ,char *argv[])
{
    char *s = "root";
 
    if(argc == 2)
    {
	s = argv[1];
    }
    struct passwd *ptr = getpwnam(s);
    pid_t pid = fork();
    assert(pid != -1);

    if(pid == 0)
    {
	setgid(ptr->pw_gid);
	setuid(ptr->pw_uid);
 
        setenv("HOME",ptr->pw_dir,1);
        execl(ptr->pw_shell,ptr->pw_shell,(char*)0);
        perror("execl error");
    }
    else
    {
	wait(NULL);
    }
 
    exit(0);
}  
