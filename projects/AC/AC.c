#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "textools.h"
#include "AC.h"
#include "queue.h"

/*global var*/
struct automata ac_automata;
state_t *state_0; /*point to state 0*/
static unsigned state_out_degree[ALPHABET_SIZE+1];

/*construction functions*/
static void build_goto(const patset_t *);
static void build_fail(void);
static void remove_fail(void);

/*functional functions*/
static void enter(patnode_t *);
static state_t *get_next_state(state_t *, unsigned char);
static void link_states(state_t *, unsigned char, state_t *);
static void ins_output(state_t *, patnode_t *);
static void list_to_table(state_t *);
static int get_out_degree(state_t *);
static void record_hit(o_node_t *, patset_t *);

/*destroy functions*/
static void des_ac_automata(void);
static void des_state(state_t *);
static void des_goto(state_t *);
static void des_output(state_t *);

/*create functions*/
static g_node_t *cre_goto_node(unsigned char, state_t *);
static state_t *cre_state(void);
static s_node_t *cre_state_node(state_t *);


void AC(FILE *fp_text, patset_t *pat_set)
{
    state_t *state; 
    state_t *next_state;
    int ch, outdegree;
    clock_t start, end;
    double build_goto_time = 0, build_fail_time = 0, match_time = 0, remove_time = 0;
    long text_len = 0;
    long trace_back = 0;
	
    /*build goto*/
    printf("building goto.....  ");
    fflush(stdout);
    start = clock();
    build_goto(pat_set);
    end = clock();
    printf("Done!\n");
    build_goto_time = (double) (end - start) / CLOCKS_PER_SEC;

    /*build failure*/
    printf("buliding fail.....  ");
    fflush(stdout);
    start = clock();
    build_fail(); 
    end = clock();
    printf("Done!\n");
    build_fail_time = (double) (end - start) / CLOCKS_PER_SEC;

    /*remove failure*/
#if (NOFAIL)
    printf("removing fail.....  ");
    fflush(stdout);
    start = clock();
    remove_fail();
    end = clock();
    printf("Done!\n");
    remove_time = (double) (end - start) / CLOCKS_PER_SEC;
#endif

#if (MATCHING)
    /*matching*/
    printf("matching....  ");
    fflush(stdout);
    start = clock();
    state = state_0; /*the initial state is state 0*/
    while ((ch = getc(fp_text)) != EOF) {
	text_len++;
	while (!(next_state = get_next_state(state,ch))) {
	    state = state->fail;
	    trace_back++;
	}
	state = next_state;
	if(state->output)
	    record_hit(state->output, pat_set);
    }
    end = clock();
    printf("Done!\n");
    match_time = (double) (end - start) / CLOCKS_PER_SEC;
#endif 
	
    /*destroy AC machine*/
    printf("free memory.....  ");
    fflush(stdout);
    des_ac_automata(); 
    printf("Done!\n");

    if (SHOW_IN_FILE)          /* 在文件中显示实验结果 */
        freopen(RESULTS_FILE, "a", stdout);

    /*print statistics*/
    printf("\n\nResults:\ntext length: %ld    pats num: %ld\n"
           "total states: %ld    tab states: %ld     list states: %ld	max list len: %d\n"
           "bulid goto: %f     build fail: %f     remove fail: %f\n"
           "trace back: %ld    total hits: %ld\n"
           "match time: %f  %s\n", 
           text_len, pat_set->total_pats,
           ac_automata.count, ac_automata.tab_num, ac_automata.list_num, MAX_LIST_LEN, 
           build_goto_time, build_fail_time, remove_time, 
           trace_back, pat_set->total_hits,
           match_time, NOFAIL ? "(no fail)" : "");
    
    for (ch = 0; ch < ALPHABET_SIZE + 1; ch++)
        if (outdegree = state_out_degree[ch])
            printf("%d: %d  %.2f%%\n", ch, outdegree, (double) outdegree/ac_automata.count);
    
}

static void build_goto(const patset_t *pat_set) /*构建goto函数*/
{
    patnode_t *pat; /*指向当前模式串节点*/
    state_0 = cre_state();
    int ch;
 
    /*create state 0*/
    ac_automata.root = state_0;
    list_to_table(state_0);
    ac_automata.state_list = cre_state_node(state_0);
    ac_automata.count++;

    /*依次插入所有模式串*/
    for (pat = pat_set->pat_list; pat; pat = pat->next) 
	enter(pat);

    /*完成0状态处的goto函数*/
    for (ch = 0; ch < ALPHABET_SIZE; ch++)
	if (!((state_t **) state_0->go_to)[ch]) /*0状态中任何没有跳转状态的字符都跳转到自身*/
	    ((state_t **) state_0->go_to)[ch] = state_0; 
}

static void enter(patnode_t *pat)
{
    state_t *cur_state = state_0; /*当前所处状态*/
    state_t *next_state, *new_state; /*下一个状态*/
    s_node_t *state_list_node;
    const char *p = pat->pat_str;

    /*已经存在的状态*/
    while (*p && (next_state = get_next_state(cur_state, *p))) {
        cur_state = next_state;
        p++;
    }

    /*为后面字符新建状态*/
    while (*p) { 
        new_state = cre_state();
        link_states(cur_state, *p++, new_state); /*将新状态插入状态机中*/
        cur_state = new_state;
        
        state_list_node = cre_state_node(new_state);	
        state_list_node->next = ac_automata.state_list;
        ac_automata.state_list = state_list_node;
    
        ac_automata.count++;
    }
    
    /*在状态cur_state的output中插入pattern*/		
    ins_output(cur_state, pat);
}

static state_t *cre_state(void)
{
    state_t *new_state = MALLOC(1, state_t, __FILE__, __LINE__);
	
    new_state->go_to = NULL;
    new_state->fail = NULL;
    new_state->output = NULL;
    new_state->g_type = LIST;	/* 默认为list */

    return new_state;
}

static s_node_t *cre_state_node(state_t *state)
{
    s_node_t *new_node = (s_node_t *) MALLOC(1, s_node_t, __FILE__, __LINE__);
	
    new_node->state = state;
    new_node->next = NULL;

    return new_node;
}

static void link_states(state_t *from_state, unsigned char input_char, state_t *to_state)
{
    g_node_t *new_node, *p, *q; 

    if (from_state->g_type == TABLE) /*goto函数是访问表*/
	((state_t **) from_state->go_to)[input_char] = to_state;
    /*goto函数是有序链表，且节点数不超过临界值，则继续向其中插入新的节点*/
    else if (get_out_degree(from_state) != MAX_LIST_LEN) {
	new_node = cre_goto_node(input_char, to_state);
	for (p = (g_node_t *) from_state->go_to, q = NULL; p && p->input_char < input_char; q = p, p = p->next)
	    ;
	new_node->next = p;
	if (!q)
	    from_state->go_to = new_node;
	else
	    q->next = new_node;

    } else {/*若超过临界值，则先转为表存储,再插入*/
	list_to_table(from_state);
	((state_t **) from_state->go_to)[input_char] = to_state;
    }
}

static void ins_output(state_t *state, patnode_t *pat)
{
    o_node_t *output_node;
	
    if (!state->output) { /*to prevent repeated inserting*/
        output_node  = MALLOC(1, o_node_t, __FILE__, __LINE__);
        /* 插入到表头 */
        output_node->pat = pat;
        output_node->next = state->output;
        state->output = output_node;
    }
}

static state_t *get_next_state(state_t *state, unsigned char input_char)
{
    g_node_t *p;

    if (state->g_type == TABLE) /*如果该状态的goto函数是访问表，则直接读取下一个状态*/
	return ((state_t **) state->go_to)[input_char];

    /*如果该状态的goto函数是链表，则依次读取链表项，看是否有匹配该字符的下一个状态*/
    for (p = (g_node_t *) state->go_to; p && p->input_char < input_char; p = p->next)
	;

    if (p && p->input_char == input_char)
	return p->next_state;
    else
	return NULL; /*没有找到对应于当前输入字符的，匹配失败*/

}

static g_node_t *cre_goto_node(unsigned char input_char, state_t *next_state)
{
    g_node_t *new_node =  MALLOC(1, g_node_t, __FILE__, __LINE__);

    new_node->input_char = input_char;
    new_node->next_state = next_state;
    new_node->next = NULL;

    return new_node;
}

static int get_out_degree(state_t *state) /*计算state的出度*/
{
    int num = 0, i;
    g_node_t *p;
    
    if (state->g_type == LIST)
        for (p = (g_node_t *) state->go_to; p; p = p->next)
            num++;
    else
        for (i = 0; i < ALPHABET_SIZE; i++)
            if (((state_t **) state->go_to)[i] != NULL)
                num++;

    return  num;
}

static void list_to_table(state_t *state)
{
    g_node_t *p;
    state_t **new_table =  MALLOC(ALPHABET_SIZE, state_t *, __FILE__, __LINE__); /*分配一个新的访问表*/
    
    memset(new_table, 0, ALPHABET_SIZE * sizeof(state_t *)); /*初始化新访问表*/

    for (p = (g_node_t *) state->go_to; p; p = p->next) /*将链表中的数据导入到访问表中*/
	new_table[(unsigned) p->input_char] = p->next_state;

    des_goto(state); /*销毁state状态的goto链表*/
    state->go_to = new_table; /*连接上新的访问表*/
    state->g_type = TABLE;
}

static void build_fail(void)
{
    queue_t *state_q = cre_queue(); /*状态队列*/
    int ch;
    state_t *state; /*深度为d的状态*/
    state_t *fail; /*state的fail值*/
    state_t *next_state; /*state的孩子，深度为d+1的状态*/
    state_t *next_fail; /*next_state的fail值*/
    o_node_t *f_output; /*遍历fail状态的output，将其插入next_state的ouput中*/

    /*处理深度为1的状态*/
    for (ch = 0; ch < ALPHABET_SIZE; ch++) /*扫描状态0的goto表*/
	if ((state = ((state_t **) state_0->go_to)[ch]) != state_0) { 
	    state->fail = state_0; /*其错误状态均为0状态*/
	    in_queue(state_q, state); 
	}

    /*广度优先遍历状态机*/
    while (!queue_is_empty(state_q)) {
	state = out_queue(state_q); /*返回深度为d的状态*/
	fail = state->fail;
	for (ch = 0; ch < ALPHABET_SIZE; ch++) /*寻找state的所有下一个有效状态*/
	    if (next_state = get_next_state(state, ch)) {
		/*将该状态插入队列*/
		in_queue(state_q, next_state); 
		while (!(next_fail = get_next_state(fail, ch))) /*一直回溯直到找到合法的fail状态*/
		    fail = fail->fail;
		next_state->fail = next_fail;
		/*并入fail处的output值*/
		for (f_output = next_fail->output; f_output; f_output = f_output->next)
		    ins_output(next_state, f_output->pat);
	    }
    }

    des_queue(state_q); /*销毁队列*/
}

static void record_hit(o_node_t *o_node, patset_t *pat_set)
{
    while (o_node) {
	o_node->pat->hit_times++;
	o_node = o_node->next;
	pat_set->total_hits++;
    }
}

#if (NOFAIL == 1)
static void remove_fail(void)
{
    queue_t *state_q = cre_queue();
    state_t *next_state, *state;
    int ch;

    for (ch = 0; ch < ALPHABET_SIZE; ch++) 
	if ((state = get_next_state(state_0, ch)) != state_0)
	    in_queue(state_q, state);
	
    while (!queue_is_empty(state_q)) {
	state = out_queue(state_q);
	for (ch = 0; ch < ALPHABET_SIZE; ch++)
	    if (next_state = get_next_state(state, ch))
		in_queue(state_q, next_state);
	    else
		link_states(state, ch, get_next_state(state->fail, ch));
    }

    des_queue(state_q);
}
#endif 

/*销毁整个AC状态机*/
static void des_ac_automata(void) 
{
    s_node_t *free_node, *p = ac_automata.state_list;

    while (p) {
	free_node = p;
	p = p->next;
        state_out_degree[get_out_degree(free_node->state)]++;
	if (free_node->state->g_type == TABLE) /*统计goto的tab数和list数*/
	    ac_automata.tab_num++;
	else
	    ac_automata.list_num++;
	des_state(free_node->state); /*销毁每个状态内容*/
	free(free_node);
    }
}

static void des_state(state_t *state)
{
    des_goto(state);
    des_output(state); /*销毁output*/
    free(state);
}

static void des_goto(state_t *state)
{
    g_node_t *p;
    g_node_t *free_node;

    if (state->g_type == TABLE) /*销毁goto*/
	free(state->go_to);
    else {
	p = (g_node_t *) state->go_to;
	while (p) {
	    free_node = p;
	    p = p->next;
	    free(free_node);
	}
    }

    state->go_to = NULL;
}

static void des_output(state_t *state)
{
    o_node_t *p = state->output;
    o_node_t *free_node;

    while (p) {
	free_node = p;
	p = p->next;
	free(free_node);
    }
}
