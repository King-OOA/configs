#include <stdio.h>
#include <stdlib.h>
#include "textools.h"
#include "queue.h"

/*void test_queue(queue_t *q)
  {
  char op;
  int value;

  for (;;) {
  printf("请输入指令: ");
  scanf(" %c", &op);
  while (getchar() != '\n')
  ;

  switch(op) {
  case 'i':  
  printf("请输入值: ");
  scanf("%d", &value);
  in_queue(q, value);
  break;
  case 'd': out_queue(q);break;
  case 'f': 
  if ((value = get_q_first(q)) != EMPTY)
  printf("第一个元素为: %d", value);
  else
  puts("队列空!");
  break;
  case 'l': if ((value = get_q_last(q)) != EMPTY)
  printf("最后一个元素为: %d", value);
  else
  puts("队列空!");
  break;
  case 'p': print_queue(q);break;
  case 'q': return;break;
  default: puts("不合法指令!");
  }
  printf("\n");
  }

  des_queue(q);
  }*/

queue_t *cre_queue(void)
{
     queue_t *new_queue;

     new_queue = MALLOC(1, queue_t, __FILE__, __LINE__);
     new_queue->count = 0;
     new_queue->head = NULL;
     new_queue->tail = NULL;

     return new_queue;
}

void des_queue(queue_t *q)
{
     clean_queue(q);
     free(q);
}

void clean_queue(queue_t *q) /*清空队列链表，但保留队列结构*/
{
     qnode_t *free_node;

     while (q->head) {
          free_node = q->head;
          q->head = q->head->next;
          free(free_node);
     }
	
     q->tail = NULL;
     q->count = 0;
}

int queue_is_empty(queue_t *q)
{
     return q->count == 0;
}

void in_queue(queue_t *q, q_item_t value) /*插入一个元素*/
{
     qnode_t *new_node =  MALLOC(1, qnode_t, __FILE__, __LINE__);

     new_node->value = value;
     new_node->next = NULL;

     if (queue_is_empty(q)) {/*插入第一个节点*/
          q->head = new_node;
          q->tail = new_node;
     } else {	
          q->tail->next = new_node;
          q->tail = new_node;
     }

     q->count++;
}

q_item_t out_queue(queue_t *q) /*删除一个元素*/
{
     qnode_t* free_node;
     q_item_t value;

     if (queue_is_empty(q)) {
          puts("The queue is empty!");
          return EMPTY;
     }

     value = q->head->value;
     free_node = q->head;
     q->head = q->head->next;
     free(free_node);
     q->count--;

     if (queue_is_empty(q)) /*删除了最后一个节点*/
          q->tail = NULL;

     return value;
}

q_item_t get_q_first(queue_t *q) /*返回队列第一个元素值，并不删除*/
{
     if (queue_is_empty(q))
          return EMPTY;
     else
          return q->head->value;
}

q_item_t get_q_last(queue_t *q)/*返回队列最后一个元素值，并不删除*/
{
     if (queue_is_empty(q))
          return EMPTY;
     else	
          return q->tail->value;
}

#ifdef DEBUG
void print_queue(queue_t *q)
{
     qnode_t *index;
     FILE *fp_result;

     fp_result = fopen("./queue_content", "w");
     
     if (queue_is_empty(q)) {
          printf("空队列!\n");
          fputs("空队列!\n", fp_result);
          return;
     }

     printf("队列中共有 %ld 元素:\n ", q->count);
     fprintf(fp_result, "队列中共有 %ld 元素:\n ", q->count);
     for (index = q->head; index; index = index->next) {
          printf("%ld ", index->value);
          fprintf(fp_result, "%ld ", index->value);
     }

     fclose(fp_result);
}
#endif
