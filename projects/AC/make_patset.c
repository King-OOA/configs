#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "textools.h"
#include "make_patset.h"

//static double cal_sd(patset_t *pat_set);
static void ins_pat(patnode_t *, patset_t *);
static void read_pats(FILE *, patset_t *);

/* 构建集数据结构 */
patset_t *cre_pat_set(const char *pats_file_name)
{
    FILE *fp_pats; /*模式串文件*/
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

/* 依次读取模式串 */
static void read_pats(FILE *fp_pats, patset_t *pat_set)
{
    char pat[MAX_PAT_LEN+1]; /*模式串缓存，最大1000个字符，包括换行符*/
    int pat_len;
    long pats_readed = 0; /*已经读入的模式数量*/
    char *line_break; /*换行符指针*/

    while (fgets(pat, sizeof(pat), fp_pats) && pats_readed <= MAX_PATS) {
        if (line_break = strchr(pat, '\n'))
            *line_break = '\0';
        if ((pat_len = strlen(pat)) >= MIN_PAT_LEN && pat_len <= MAX_PAT_LEN) { /*模式串符合串长要求*/
            ins_pat(cre_pat_node(pat), pat_set); /*构建模式节点，并插入到模式集中*/
            pats_readed++;
            pat_set->total_pats++;
            pat_set->total_pat_len += pat_len;
            pat_set->pat_len_distri[pat_len-1]++;
        }
    }

    pat_set->mean_pat_len = (double) pat_set->total_pat_len / pat_set->total_pats; /*计算模式串平均长度*/
    //   pat_set->pat_len_sd = cal_sd(pat_set);

}

patnode_t *cre_pat_node(const char *pat_str)
{
    /*分配新的pat节点*/
    patnode_t *new_pat_node =  MALLOC(1, patnode_t, __FILE__, __LINE__);
    /*拷贝模式串*/
    new_pat_node->pat_str = strdup(pat_str);
    /*处理其余成员*/
    new_pat_node->pat_len = strlen(pat_str);
    new_pat_node->hit_times = 0;
    new_pat_node->next = NULL;

    return new_pat_node;
}

static void ins_pat(patnode_t *pat_node, patset_t *pat_set)
{
    static patnode_t *tail = NULL; 
    /*静态变量保存模式链表尾指针*/

    if (pat_set->total_pats == 0) { /*模式集为空*/
        pat_set->pat_list = pat_node;/*插入到头*/
        tail = pat_node; 
    } else { /*插入模式串集合尾部*/
        tail->next = pat_node;
        tail = pat_node;
    }

    if (pat_node->pat_len < pat_set->min_pat_len)
        pat_set->min_pat_len = pat_node->pat_len;
    if (pat_node->pat_len > pat_set->max_pat_len)
        pat_set->max_pat_len = pat_node->pat_len;

}

void des_pat_set(patset_t *pat_set) /*销毁模式集合*/
{
    patnode_t *p, *next;

    /*依次销毁串链表的每个节点*/
    for (p = pat_set->pat_list; p; p = next) { 
        next = p->next;
        free(p->pat_str); /*销毁模式字符串*/
        free(p);/*销毁模式节点*/
    }

    free(pat_set->pats_file);
    free(pat_set); /*销毁模式集本身*/
}

/* static double cal_sd(patset_t *pat_set) /\*计算标准差*\/ */
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
		max len： %d	min len： %d	mean len: %.2f	sd: %.2f\n	total len: %ld\
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
      printf("长度: %2d  个数: %5d 所占比例: %%%7f\n", index + 1, pat_set->pat_len_distri[index], ((double) pat_set->pat_len_distri[index] / pat_set->total_pats) * 100);
      for (num = 1, pat_node_index = pat_set->pat_list; 
      pat_node_index != NULL;
      pat_node_index = pat_node_index->next, num++)
      printf("%4d：%3d %s\n", num, pat_node_index->pat_len, pat_node_index->pat_str);*/
}
#endif
