#ifndef MAKEPATSET_H
#define MAKEPATSET_H

#define MIN_PAT_LEN 1
#define MAX_PAT_LEN 1000
#define MAX_PATS 1000000000

typedef struct pat_node {
    char *pat_str; /*指向模式串*/
    int pat_len; /*该模式串的长度*/
    long hit_times; /*命中次数*/
    struct pat_node *next;
} patnode_t;

typedef struct pat_set {
    char *pats_file;
    patnode_t *pat_list; /*模式串链表*/
    long total_pats; /*模式串总数*/
    int max_pat_len; /*最大串长*/
    int min_pat_len; /*最小串长*/
    long total_pat_len; /*串长之和*/
    double mean_pat_len; /*平均串长*/
    double pat_len_sd; /*标准差*/
    int pat_len_distri[MAX_PAT_LEN]; /*串长分布，记录每一个长度含有模式串的个数*/
    long total_hits;/*命中总次数*/
} patset_t;

patset_t *cre_pat_set(const char *);
void des_pat_set(patset_t *);
void print_pat_set(const patset_t *, const char  *);
patnode_t *cre_pat_node(const char *); 

#endif
