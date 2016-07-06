#include<stdlib.h>
#include<stdio.h>
struct heapnode
{
	int data;
	struct heapnode * l, * r, * p;
};
typedef struct heapnode node;
struct qnode /*this added level of abstraction may be unsightly but it's very effective.  That's C I guess*/
{
    node * dis;
    struct qnode * next;
    unsigned char mark;/*acts as boolean to prevent creating and enqueueing duplicate empty leaf nodes*/
};
typedef struct qnode qnode;
struct heaphead
{
    unsigned char abnormality;
    node * realhead;
    qnode * head,* tail,* stack;
};
typedef struct heaphead heaphead;
void addLeaves(heaphead * jackson)/* constant time operation. does not link the parent to the new leaves*/
{
    node *left = (node *)malloc(sizeof(node));
    node *right = (node *)malloc(sizeof(node));
    qnode *one = (qnode *)malloc(sizeof(qnode));
    qnode *two = (qnode *)malloc(sizeof(qnode));
    one->mark=two->mark=0;
    left->p=right->p=jackson->head->dis;
    left->l=left->r=right->l=right->r=NULL;
    two->dis=right;
    one->dis=left;
    one->next=two;
    jackson->tail->next = one;
    jackson->tail=two;
    jackson->head->mark=1;
    return;
}
node * pop(heaphead * jackson)/*running time not truly "constant" but is dependent on current state, not input size*/
{
    qnode * ret = jackson->head;
    if (ret->mark==0) addLeaves(jackson);
    jackson->head = ret->next;
    ret->next=jackson->stack;
    jackson->stack=ret;
    if (ret->dis->p != NULL)
    {
        if (ret->dis->p->l == NULL) ret->dis->p->l = ret->dis;
        else if (ret->dis->p->r == NULL) ret->dis->p->r = ret->dis;
    }
    return ret->dis;
}
void maxBubble(int val,node * to_check)
{
    if (to_check->p == NULL) 
    {
        to_check->data=val;
    }    
    else if (to_check->p->data<val)
    {
        to_check->data = to_check->p->data;
        maxBubble(val,to_check->p);        
    }
    else to_check->data=val;
    return;
}
void minBubble(int val,node * to_check)
{
    if (to_check->p == NULL) 
    {
        to_check->data=val;
    }    
    else if (to_check->p->data>val)
    {
        to_check->data = to_check->p->data;
        minBubble(val,to_check->p);        
    }
    else to_check->data=val;
    return;
}
void startheap(heaphead * jackson,int val)/*initializes a heap- sets up a new root and two empty leaves*/
{
    node *top = (node *)malloc(sizeof(node));
    node *left = (node *)malloc(sizeof(node));
    node *right = (node *)malloc(sizeof(node));
    qnode * q = (qnode *)malloc(sizeof(qnode));
    qnode *one = (qnode *)malloc(sizeof(qnode));
    qnode *two = (qnode *)malloc(sizeof(qnode));
    top->p=top->l=top->r=NULL;
    top->data=val;
    jackson->realhead = top;
    q->mark=1;
    jackson->stack = q;
    one->mark=two->mark=0;
    left->p=right->p=top;
    left->l=left->r=right->l=right->r=NULL;
    two->dis=right;
    one->dis=left;
    one->next=two;
    jackson->head = one;
    jackson->head->next=jackson->tail=two;
}
void insert(heaphead * jackson, int val)
{
	if (jackson->realhead==NULL)
    {
        startheap(jackson,val);
        return;
    }
    node *temp = pop(jackson);
    if (jackson->abnormality==0)minBubble(val,temp);
    else maxBubble(val,temp);
    temp->l=temp->r=NULL;
    return;
}
void maxBubbleDown(int val,node * to_check)
{
    if (to_check->r == NULL) 
    {
        if (to_check->l==NULL)
        {
            to_check->data=val;    
        }
        else if (to_check->l->data>val)    
        {
            to_check->data=to_check->l->data;
            maxBubbleDown(val,to_check->l);   
        }
        else to_check->data=val;
    }    
    else if (to_check->r->data>val)
    {
        if (to_check->l->data>to_check->r->data)
        {
            to_check->data=to_check->l->data;
            maxBubbleDown(val,to_check->l);  
        }
        else
        {
            to_check->data=to_check->r->data;
            maxBubbleDown(val,to_check->r);  
        }   
    }
    else if (to_check->l->data>val)    
    {
        to_check->data=to_check->l->data;
        maxBubbleDown(val,to_check->l);   
    }
    else to_check->data=val;
    return;
}
void minBubbleDown(int val,node * to_check)
{
    if (to_check->r == NULL) 
    {
        if (to_check->l==NULL)
        {
            to_check->data=val;    
        }
        else if (to_check->l->data<val)    
        {
            to_check->data=to_check->l->data;
            minBubbleDown(val,to_check->l);   
        }
        else to_check->data=val;
    }    
    else if (to_check->r->data<val)
    {
        if (to_check->l->data<to_check->r->data)
        {
            to_check->data=to_check->l->data;
            minBubbleDown(val,to_check->l);  
        }
        else
        {
            to_check->data=to_check->r->data;
            minBubbleDown(val,to_check->r);  
        }   
    }
    else if (to_check->l->data<val)    
    {
        to_check->data=to_check->l->data;
        minBubbleDown(val,to_check->l);   
    }
    else to_check->data=val;
    return;
}
int extract(heaphead * jackson)
{
    if (jackson->realhead == NULL)
    {
        printf("Can't extract from an empty heap\n");
        return -1;
    }
    int r = jackson->realhead->data;
    if (jackson->realhead->l == NULL)
    {
        free(jackson->realhead);
        free(jackson->tail);
        free(jackson->head);
        jackson->realhead=NULL;
        return r;
    }
    qnode *last = jackson->stack;
    jackson->stack=jackson->stack->next;
    if (jackson->abnormality==1) maxBubbleDown(last->dis->data,jackson->realhead);
    else minBubbleDown(last->dis->data,jackson->realhead);
    if (last->dis->p->r == NULL) last->dis->p->l=NULL;
    else last->dis->p->r=NULL;
    last->next=jackson->head;
    jackson->head=last;
    return r;
}
