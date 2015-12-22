#include<stdio.h>
#include<time.h>
#include<stdlib.h>


int main()
{
    time_t timeval;
    (void)time(&timeval);
    printf("%s",ctime(&timeval));
    exit(0);
}
