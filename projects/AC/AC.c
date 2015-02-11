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

    if (SHOW_IN_FILE)          /* ���ļ�����ʾʵ���� */
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

static void build_goto(const patset_t *pat_set) /*����goto����*/
{
    patnode_t *pat; /*ָ��ǰģʽ���ڵ�*/
    state_0 = cre_state();
    int ch;
 
    /*create state 0*/
    ac_automata.root = state_0;
    list_to_table(state_0);
    ac_automata.state_list = cre_state_node(state_0);
    ac_automata.count++;

    /*���β�������ģʽ��*/
    for (pat = pat_set->pat_list; pat; pat = pat->next) 
	enter(pat);

    /*���0״̬����goto����*/
    for (ch = 0; ch < ALPHABET_SIZE; ch++)
	if (!((state_t **) state_0->go_to)[ch]) /*0״̬���κ�û����ת״̬���ַ�����ת������*/
	    ((state_t **) state_0->go_to)[ch] = state_0; 
}

static void enter(patnode_t *pat)
{
    state_t *cur_state = state_0; /*��ǰ����״̬*/
    state_t *next_state, *new_state; /*��һ��״̬*/
    s_node_t *state_list_node;
    const char *p = pat->pat_str;

    /*�Ѿ����ڵ�״̬*/
    while (*p && (next_state = get_next_state(cur_state, *p))) {
        cur_state = next_state;
        p++;
    }

    /*Ϊ�����ַ��½�״̬*/
    while (*p) { 
        new_state = cre_state();
        link_states(cur_state, *p++, new_state); /*����״̬����״̬����*/
        cur_state = new_state;
        
        state_list_node = cre_state_node(new_state);	
        state_list_node->next = ac_automata.state_list;
        ac_automata.state_list = state_list_node;
    
        ac_automata.count++;
    }
    
    /*��״̬cur_state��output�в���pattern*/		
    ins_output(cur_state, pat);
}

static state_t *cre_state(void)
{
    state_t *new_state = MALLOC(1, state_t, __FILE__, __LINE__);
	
    new_state->go_to = NULL;
    new_state->fail = NULL;
    new_state->output = NULL;
    new_state->g_type = LIST;	/* Ĭ��Ϊlist */

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

    if (from_state->g_type == TABLE) /*goto�����Ƿ��ʱ�*/
	((state_t **) from_state->go_to)[input_char] = to_state;
    /*goto���������������ҽڵ����������ٽ�ֵ������������в����µĽڵ�*/
    else if (get_out_degree(from_state) != MAX_LIST_LEN) {
	new_node = cre_goto_node(input_char, to_state);
	for (p = (g_node_t *) from_state->go_to, q = NULL; p && p->input_char < input_char; q = p, p = p->next)
	    ;
	new_node->next = p;
	if (!q)
	    from_state->go_to = new_node;
	else
	    q->next = new_node;

    } else {/*�������ٽ�ֵ������תΪ��洢,�ٲ���*/
	list_to_table(from_state);
	((state_t **) from_state->go_to)[input_char] = to_state;
    }
}

static void ins_output(state_t *state, patnode_t *pat)
{
    o_node_t *output_node;
	
    if (!state->output) { /*to prevent repeated inserting*/
        output_node  = MALLOC(1, o_node_t, __FILE__, __LINE__);
        /* ���뵽��ͷ */
        output_node->pat = pat;
        output_node->next = state->output;
        state->output = output_node;
    }
}

static state_t *get_next_state(state_t *state, unsigned char input_char)
{
    g_node_t *p;

    if (state->g_type == TABLE) /*�����״̬��goto�����Ƿ��ʱ���ֱ�Ӷ�ȡ��һ��״̬*/
	return ((state_t **) state->go_to)[input_char];

    /*�����״̬��goto���������������ζ�ȡ��������Ƿ���ƥ����ַ�����һ��״̬*/
    for (p = (g_node_t *) state->go_to; p && p->input_char < input_char; p = p->next)
	;

    if (p && p->input_char == input_char)
	return p->next_state;
    else
	return NULL; /*û���ҵ���Ӧ�ڵ�ǰ�����ַ��ģ�ƥ��ʧ��*/

}

static g_node_t *cre_goto_node(unsigned char input_char, state_t *next_state)
{
    g_node_t *new_node =  MALLOC(1, g_node_t, __FILE__, __LINE__);

    new_node->input_char = input_char;
    new_node->next_state = next_state;
    new_node->next = NULL;

    return new_node;
}

static int get_out_degree(state_t *state) /*����state�ĳ���*/
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
    state_t **new_table =  MALLOC(ALPHABET_SIZE, state_t *, __FILE__, __LINE__); /*����һ���µķ��ʱ�*/
    
    memset(new_table, 0, ALPHABET_SIZE * sizeof(state_t *)); /*��ʼ���·��ʱ�*/

    for (p = (g_node_t *) state->go_to; p; p = p->next) /*�������е����ݵ��뵽���ʱ���*/
	new_table[(unsigned) p->input_char] = p->next_state;

    des_goto(state); /*����state״̬��goto����*/
    state->go_to = new_table; /*�������µķ��ʱ�*/
    state->g_type = TABLE;
}

static void build_fail(void)
{
    queue_t *state_q = cre_queue(); /*״̬����*/
    int ch;
    state_t *state; /*���Ϊd��״̬*/
    state_t *fail; /*state��failֵ*/
    state_t *next_state; /*state�ĺ��ӣ����Ϊd+1��״̬*/
    state_t *next_fail; /*next_state��failֵ*/
    o_node_t *f_output; /*����fail״̬��output���������next_state��ouput��*/

    /*�������Ϊ1��״̬*/
    for (ch = 0; ch < ALPHABET_SIZE; ch++) /*ɨ��״̬0��goto��*/
	if ((state = ((state_t **) state_0->go_to)[ch]) != state_0) { 
	    state->fail = state_0; /*�����״̬��Ϊ0״̬*/
	    in_queue(state_q, state); 
	}

    /*������ȱ���״̬��*/
    while (!queue_is_empty(state_q)) {
	state = out_queue(state_q); /*�������Ϊd��״̬*/
	fail = state->fail;
	for (ch = 0; ch < ALPHABET_SIZE; ch++) /*Ѱ��state��������һ����Ч״̬*/
	    if (next_state = get_next_state(state, ch)) {
		/*����״̬�������*/
		in_queue(state_q, next_state); 
		while (!(next_fail = get_next_state(fail, ch))) /*һֱ����ֱ���ҵ��Ϸ���fail״̬*/
		    fail = fail->fail;
		next_state->fail = next_fail;
		/*����fail����outputֵ*/
		for (f_output = next_fail->output; f_output; f_output = f_output->next)
		    ins_output(next_state, f_output->pat);
	    }
    }

    des_queue(state_q); /*���ٶ���*/
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

/*��������AC״̬��*/
static void des_ac_automata(void) 
{
    s_node_t *free_node, *p = ac_automata.state_list;

    while (p) {
	free_node = p;
	p = p->next;
        state_out_degree[get_out_degree(free_node->state)]++;
	if (free_node->state->g_type == TABLE) /*ͳ��goto��tab����list��*/
	    ac_automata.tab_num++;
	else
	    ac_automata.list_num++;
	des_state(free_node->state); /*����ÿ��״̬����*/
	free(free_node);
    }
}

static void des_state(state_t *state)
{
    des_goto(state);
    des_output(state); /*����output*/
    free(state);
}

static void des_goto(state_t *state)
{
    g_node_t *p;
    g_node_t *free_node;

    if (state->g_type == TABLE) /*����goto*/
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
