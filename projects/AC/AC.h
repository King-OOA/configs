#ifndef AC_H
#define AC_H

#include <stdio.h>
#include "make_patset.h"

#define ALPHABET_SIZE 256
#define NOFAIL 0
#define MAX_LIST_LEN 20         /* goto �������������󳤶� */
#define MATCHING 0
#define SHOW_IN_FILE 0          /* Ĭ������Ļ����ʾ��� */
#define RESULTS_FILE "/home/pz/projects/AC/results"

#define DATA_PATH  "/home/pz/data/patterns/"
#define TEXT_FILE DATA_PATH"100MB"
#define PATS_FILE DATA_PATH"/size/100w"

typedef enum {TABLE,LIST} goto_t; /* goto ����ʵ�ַ��� */

/*ÿ��״̬��output����Ľڵ�*/
typedef struct output_node {
    patnode_t *pat; /*ָ��ģʽ���е�ģʽ*/
    struct output_node *next;
} o_node_t;

/*״̬�ṹ*/
typedef struct state {
    void *go_to;   /*���ܹ��������������*/
    struct state *fail; /*����ʱ��Ҫ��ת��״̬*/
    o_node_t *output; /*���Ӹ�״̬�����keys*/
    goto_t g_type; /*����goto���������黹������*/
} state_t;

typedef struct state_node {
    state_t *state;
    struct state_node *next;
} s_node_t;

typedef struct goto_node { /*goto���������ϵĽڵ�*/
    unsigned char input_char;   /* һ�����޷�������ʾ��ǰ�ַ� */
    state_t *next_state;
    struct goto_node *next;
} g_node_t;

/*AC�Զ���*/
struct automata {
    state_t *root; /*ָ��0״̬*/
    s_node_t *state_list; /*link all the states in order to destroy them*/
    long count; /*AC�Զ���������״̬����*/
    long tab_num;
    long list_num;
}; 




void AC(FILE *text, patset_t *pat);

#endif
