#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "textools.h"
#include "make_patset.h"

//static double cal_sd(patset_t *pat_set);
static void ins_pat(patnode_t *, patset_t *);
static void read_pats(FILE *, patset_t *);

/* ���������ݽṹ */
patset_t *cre_pat_set(const char *pats_file_name)
{
    FILE *fp_pats; /*ģʽ���ļ�*/
    patset_t *pat_set;
     
    pat_set = MALLOC(1, patset_t, __FILE__, __LINE__);
     
    if (!(fp_pats = fopen(pats_file_name, "r"))) {
        perror(pats_file_name);
        exit(EXIT_FAILURE);
    }

    pat_set->pats_file =  strdup(pats_file_name);
    pat_set->pat_list = NULL;
    pat_set->total_pats = 0;
    pat_set->min_pat_len = MAX_PAT_LEN;
    pat_set->max_pat_len = 0;
    pat_set->mean_pat_len = 0;
    pat_set->total_pat_len = 0;
    memset(pat_set->pat_len_distri, 0, sizeof(int) * MAX_PAT_LEN);
    pat_set->total_hits = 0;

    read_pats(fp_pats, pat_set);

    fclose(fp_pats);

    return pat_set;
}

/* ���ζ�ȡģʽ�� */
static void read_pats(FILE *fp_pats, patset_t *pat_set)
{
    char pat[MAX_PAT_LEN+1]; /*ģʽ�����棬���1000���ַ����������з�*/
    int pat_len;
    long pats_readed = 0; /*�Ѿ������ģʽ����*/
    char *line_break; /*���з�ָ��*/

    while (fgets(pat, sizeof(pat), fp_pats) && pats_readed <= MAX_PATS) {
        if (line_break = strchr(pat, '\n'))
            *line_break = '\0';
        if ((pat_len = strlen(pat)) >= MIN_PAT_LEN && pat_len <= MAX_PAT_LEN) { /*ģʽ�����ϴ���Ҫ��*/
            ins_pat(cre_pat_node(pat), pat_set); /*����ģʽ�ڵ㣬�����뵽ģʽ����*/
            pats_readed++;
            pat_set->total_pats++;
            pat_set->total_pat_len += pat_len;
            pat_set->pat_len_distri[pat_len-1]++;
        }
    }

    pat_set->mean_pat_len = (double) pat_set->total_pat_len / pat_set->total_pats; /*����ģʽ��ƽ������*/
    //   pat_set->pat_len_sd = cal_sd(pat_set);

}

patnode_t *cre_pat_node(const char *pat_str)
{
    /*�����µ�pat�ڵ�*/
    patnode_t *new_pat_node =  MALLOC(1, patnode_t, __FILE__, __LINE__);
    /*����ģʽ��*/
    new_pat_node->pat_str = strdup(pat_str);
    /*���������Ա*/
    new_pat_node->pat_len = strlen(pat_str);
    new_pat_node->hit_times = 0;
    new_pat_node->next = NULL;

    return new_pat_node;
}

static void ins_pat(patnode_t *pat_node, patset_t *pat_set)
{
    static patnode_t *tail = NULL; 
    /*��̬��������ģʽ����βָ��*/

    if (pat_set->total_pats == 0) { /*ģʽ��Ϊ��*/
        pat_set->pat_list = pat_node;/*���뵽ͷ*/
        tail = pat_node; 
    } else { /*����ģʽ������β��*/
        tail->next = pat_node;
        tail = pat_node;
    }

    if (pat_node->pat_len < pat_set->min_pat_len)
        pat_set->min_pat_len = pat_node->pat_len;
    if (pat_node->pat_len > pat_set->max_pat_len)
        pat_set->max_pat_len = pat_node->pat_len;

}

void des_pat_set(patset_t *pat_set) /*����ģʽ����*/
{
    patnode_t *p, *next;

    /*�������ٴ������ÿ���ڵ�*/
    for (p = pat_set->pat_list; p; p = next) { 
        next = p->next;
        free(p->pat_str); /*����ģʽ�ַ���*/
        free(p);/*����ģʽ�ڵ�*/
    }

    free(pat_set->pats_file);
    free(pat_set); /*����ģʽ������*/
}

/* static double cal_sd(patset_t *pat_set) /\*�����׼��*\/ */
/* { */
/*      double sd = 0; */
/*      patnode_t *p; */

/*      for (p = pat_set->pat_list; p; p = p->next) */
/*           sd += pow(p->pat_len - pat_set->mean_pat_len, 2); */

/*      return sqrt(sd / pat_set->total_pats); */
/* } */

/*print the pat set info*/
#ifdef DEBUG
void print_pat_set(const patset_t *pat_set, const char  *filename) 
{
    FILE *fp_info = fopen(filename, "w");
    patnode_t *p;
    int num;
    int index;
    long total_hits;

    if (!fp_info)
        terminate("Can not open file! : make_patset.c/print_pat_set()\n");

    fprintf(fp_info, "pats file: %s\n\
		total pats: %ld\n\
		max len�� %d	min len�� %d	mean len: %.2f	sd: %.2f\n	total len: %ld\
		total hits: %ld\n",
            pat_set->pats_file, 
            pat_set->total_pats,
            pat_set->max_pat_len, pat_set->min_pat_len, pat_set->mean_pat_len, pat_set->pat_len_sd, pat_set->total_pat_len,
            pat_set->total_hits);

    /*for (total_hits = 0, p = pat_set->pat_list; p; p = p->next)
      if (p->hit_times) {
      //fprintf(result, "%s: %ld\n", p->pat_str, p->hit_times);
      total_hits += p->hit_times;
      }

      for (index = 0; index < pat_set->max_pat_len; index++)
      printf("����: %2d  ����: %5d ��ռ����: %%%7f\n", index + 1, pat_set->pat_len_distri[index], ((double) pat_set->pat_len_distri[index] / pat_set->total_pats) * 100);
      for (num = 1, pat_node_index = pat_set->pat_list; 
      pat_node_index != NULL;
      pat_node_index = pat_node_index->next, num++)
      printf("%4d��%3d %s\n", num, pat_node_index->pat_len, pat_node_index->pat_str);*/
}
#endif
