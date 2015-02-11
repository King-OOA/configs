#ifndef	QUEUE_H
#define QUEUE_H

#include "AC.h"

#define EMPTY NULL		

typedef state_t* q_item_t; /*队列中的元素为状态的指针*/

typedef struct node {
	q_item_t value;
	struct node *next;
} qnode_t;

typedef struct queue {
	struct node *head;
	struct node *tail;
	long count;
} queue_t;

void test_queue(queue_t *);
queue_t *cre_queue(void);
void des_queue(queue_t *);
void in_queue(queue_t *, q_item_t);
q_item_t out_queue(queue_t *);
q_item_t get_q_first(queue_t *);
q_item_t get_q_last(queue_t *);
void print_queue(queue_t *);
void clean_queue(queue_t *);
int queue_is_empty(queue_t *);

#endif
