#include "csapp.h"

typedef int item_t; /* the type of buffer item. */

/* The data structure of the SHARED BUFFER used in the P-C Model, in which it manages a queue that represents the available slots. */
struct {
  item_t *buf;     /* Buffer array */
  int n;	    /* Maximum number of slots */
  int front;    /* buf[(front+1)%n] is first item */
  int rear;	    /* buf[rear%n] is last item */
  sem_t mutex;  /* protects accesses to buf */
  sem_t slots;  /* Counts available slots */
  sem_t items;  /* Counts available items */
} sbuf_t;

void sbuf_init(sbuf_t* sp, int n)
{
	sp->buf = (item_t*)malloc(n*sizeof(item_t));
	sp->n = n;
	sp->front = sp->rear = 0;
	Sem_init(&sp->mutex,0,1);
	Sem_init(&sp->slots,0,n);
	Sem_init(&sp->items,0,0);
}

void sbuf_insert(sbuf_t* sp,item_t value)
{
    P(&sp->slots);
    P(&sp->mutex);

    sp->buf[sp->rear] = value;
    sp->rear = (sp->rear+1)%sp->n;

    V(&sp->mutex);
    V(&sp->items);
}

item_t sbuf_remove(sbuf* sp)
{
    item_t item;
    P(&sp->items);
    P(&sp->mutex);

    item = sp->buf[sp->front];
    sp->front = (sp->front+1)%sp->n;

    V(&sp->mutex);
    V(&sp->slots);
    return item;
}

