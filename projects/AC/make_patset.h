#ifndef MAKEPATSET_H
#define MAKEPATSET_H

#define MIN_PAT_LEN 1
#define MAX_PAT_LEN 1000
#define MAX_PATS 1000000000

typedef struct pat_node {
    char *pat_str; /*ָ��ģʽ��*/
    int pat_len; /*��ģʽ���ĳ���*/
    long hit_times; /*���д���*/
    struct pat_node *next;
} patnode_t;

typedef struct pat_set {
    char *pats_file;
    patnode_t *pat_list; /*ģʽ������*/
    long total_pats; /*ģʽ������*/
    int max_pat_len; /*��󴮳�*/
    int min_pat_len; /*��С����*/
    long total_pat_len; /*����֮��*/
    double mean_pat_len; /*ƽ������*/
    double pat_len_sd; /*��׼��*/
    int pat_len_distri[MAX_PAT_LEN]; /*�����ֲ�����¼ÿһ�����Ⱥ���ģʽ���ĸ���*/
    long total_hits;/*�����ܴ���*/
} patset_t;

patset_t *cre_pat_set(const char *);
void des_pat_set(patset_t *);
void print_pat_set(const patset_t *, const char  *);
patnode_t *cre_pat_node(const char *); 

#endif
