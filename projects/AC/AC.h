#ifndef AC_H
#define AC_H

#include <stdio.h>
#include "make_patset.h"

#define ALPHABET_SIZE 256
#define NOFAIL 0
#define MAX_LIST_LEN 20         /* goto 链表所允许的最大长度 */
#define MATCHING 0
#define SHOW_IN_FILE 0          /* 默认在屏幕上显示结果 */
#define RESULTS_FILE "/home/pz/projects/AC/results"

#define DATA_PATH  "/home/pz/data/patterns/"
#define TEXT_FILE DATA_PATH"100MB"
#define PATS_FILE DATA_PATH"/size/100w"

typedef enum {TABLE,LIST} goto_t; /* goto 函数实现方法 */

/*每个状态上output链表的节点*/
typedef struct output_node {
    patnode_t *pat; /*指向模式集中的模式*/
    struct output_node *next;
} o_node_t;

/*状态结构*/
typedef struct state {
    void *go_to;   /*可能挂数组或有序链表*/
    struct state *fail; /*出错时需要跳转的状态*/
    o_node_t *output; /*连接该状态的输出keys*/
    goto_t g_type; /*标明goto函数是数组还是链表*/
} state_t;

typedef struct state_node {
    state_t *state;
    struct state_node *next;
} s_node_t;

typedef struct goto_node { /*goto函数链表上的节点*/
    unsigned char input_char;   /* 一律用无符号数表示当前字符 */
    state_t *next_state;
    struct goto_node *next;
} g_node_t;

/*AC自动机*/
struct automata {
    state_t *root; /*指向0状态*/
    s_node_t *state_list; /*link all the states in order to destroy them*/
    long count; /*AC自动机包含的状态总数*/
    long tab_num;
    long list_num;
}; 




void AC(FILE *text, patset_t *pat);

#endif
