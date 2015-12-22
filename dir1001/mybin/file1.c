
#include"file1.h"

bool list_init(NODE **phead)
{
    if(phead == NULL)
    {
	return false;
    }

    NODE *tmp = (NODE *)malloc(sizeof(NODE));

    *phead=tmp;
    tmp->next = NULL;
    tmp->prev = NULL;

    return true;
}

bool list_add(NODE *phead,char *p)
{
    if(phead == NULL)
    {
	return false;
    }

    NODE *tmp = (NODE *)malloc(sizeof(NODE));
    

    tmp->date = p;
    tmp->next = NULL;
    tmp->prev = NULL;

    NODE *pt = phead;
    while(pt->next != NULL)
    {
	pt = pt->next;
    }

    pt->next = tmp;
    tmp->prev = pt;

    return true;
}


int list_length(NODE *phead)
{
    NODE *p = phead;
    int count = 0;
    while(p->next != NULL)
    {
	p = p->next;
	count++;
    }

    return count;
}

char *list_getstr(NODE *phead,int pos)
{
    if(phead == NULL || pos < 0 || pos >list_length(phead))
    {
	return NULL;
    }

    NODE *p = phead;
    while(pos >= 0)
    {
	p = p->next;
	pos--;
    }

    return p->date;
}

bool list_clear(NODE *phead)
{
    if(phead->next == NULL)
    {
	return false;
    }

    NODE *p = phead->next;
    while(p->next != NULL)
    {
	p = p->next;
	free(p->prev);
    }
    free(p);
    phead->next = NULL;

    return true;
}

bool list_destory(NODE *phead)
{
    if(phead == NULL)
    {
	return false;
    }

    NODE *p = phead->next;
    while(p->next != NULL)
    {
	NODE *p1 = p->next;
	free(p);
	p = p1;
    }

    free(p);
    free(phead);

    return true;
}


bool list_add_b(NODE *phead,char *p,int val)
{
    if(phead == NULL)
    {
        return false;
    }

    NODE *tmp = (NODE *)malloc(sizeof(NODE));

    tmp->size = val;
    tmp->next = NULL;
    tmp->prev = NULL;
    strncpy(tmp->d_name,p,strlen(p)+1);

    NODE *pt = phead;
    if(pt->next == NULL)
    {
        phead->next = tmp;
        tmp->prev = phead;

        return true;
    }

    while(pt->next != NULL)
    {
        NODE *pt1=pt;
        pt = pt->next;

        if(pt->size > val)
        {
            pt1->next = tmp;
            tmp->prev = pt1;
            tmp->next = pt;
            pt->prev = tmp;

            break;
        }

        if(pt->next == NULL)
        {
            pt->next = tmp;
            tmp->prev = pt1;

            break;
	}
    }

    return true;
}
              
bool list_add_a(NODE *phead,char *p,int val)
{
    if(phead == NULL)
    {
        return false;
    }

    NODE *tmp = (NODE *)malloc(sizeof(NODE));

    tmp->size = val;
    tmp->next = NULL;
    tmp->prev = NULL;
    strncpy(tmp->d_name,p,strlen(p)+1);

    NODE *pt = phead;
    if(pt->next == NULL)
    {
	phead->next = tmp;
	tmp->prev = phead;

	return true;
    }

    while(pt->next != NULL)
    {
	NODE *pt1=pt;
        pt = pt->next;

	/*if(strcmp(pt->d_name,p) > 0)
	{
	    pt1->next = tmp;
	    tmp->prev = pt1;
	    tmp->next = pt;
	    pt->prev = tmp;

	    break;
	}*/

	if(pt->next == NULL)
	{
	    pt->next = tmp;
	    tmp->prev = pt1;

	    break;
	}	
    }

    return true;
}

bool list_add_c(NODE *phead, int pos)
{
    if (phead == NULL ||pos < 0||pos >list_length(phead))
    {
	return false;
    }

    NODE *tmp = (NODE *)malloc(sizeof(NODE));
    memset(tmp->buff,0,255);
    tmp->next = NULL;
    tmp->prev = NULL;
   
    NODE *p = phead;
    if (pos == 0)
    {
	p = phead->next;
	phead->next = tmp;
	tmp->prev = phead;
	tmp->next = p;
	if (p != NULL)
	{
	    p->prev = tmp;
	}
    }
    else if (pos == list_length(phead))
    {
	while (p->next != NULL)
	{
	    p = p->next;
	}

	p->next = tmp;
	tmp->prev = p;
    }
    else
    {
	while (pos--)
	{
	    p = p->next;
	}
	p->next->prev = tmp;
	tmp->next = p->next;
	tmp->prev = p;
	p->next = tmp;
    }

    return true;
}

bool list_delete(NODE *phead,int pos)
{
    if (phead == NULL ||pos < 0 ||pos >= list_length(phead))
    {
	//printf("链表已经为空\n");
	return false;
    }

    NODE *p = phead;
    if (pos == 0)
    {
	p = p->next;
	if (p->next == NULL)
	{
	    phead->next = NULL;
	}
	else
	{
	    phead->next = p->next;
	    p->next->prev = phead;
	}
    }
    else if (pos == list_length(phead)-1)
    {
	while (p->next != NULL)
	{
	    p = p->next;
	}
	p->prev->next = NULL;
    }
    else
    {
	while (pos--)
	{
	    p = p->next;
	}
	p = p->next;
	p->prev->next = p->next;
	p->next->prev = p->prev;
    }
    free(p);

    return true;
}

/*bool list_delete(NODE *phead)
{
    if(phead == NULL)
    {
	return false;
    }

    NODE *p = phead->next;
    while(p != NULL)
    {
	p = p->next;
    }
    p = p->prev;
    free(p->next);
    p->next = NULL;

    return true;
}*/

bool list_display(NODE *phead)
{
    if (phead == NULL)
    {
	return false;
    }

    NODE *p = phead->next;
    while (p != NULL)
    {
	printf("%s\n", p->d_name);
	p = p->next;
    }

    return true;
}
