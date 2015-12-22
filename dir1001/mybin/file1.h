#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>

typedef enum {false=0,true=1} bool;
typedef struct NODE
{
	char *date;
	char d_name[256];
	struct NODE *next;
	struct NODE *prev;
	char buff[256];	
	int size;
}NODE;

bool list_init(NODE **phead);

bool list_add(NODE *phead,char *p);

int list_length(NODE *phead);

char *list_getstr(NODE *phead,int pos);

bool list_clear(NODE *phead);

bool list_destory(NODE *phead);

bool list_add_a(NODE *phead,char *p,int val);

bool list_add_b(NODE *phead,char *p,int val);

bool list_add_c(NODE *phead,int pos);

bool list_delete(NODE *phead,int pos);

bool list_display(NODE *phead);
