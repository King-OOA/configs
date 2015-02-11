#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "libcommon.h"
#include "make_data.h"
			

void pats_to_text(const char *pats_filename, const char *text_filename, long nl, long np) /*从模式串生成文本*/
{
     FILE *fp_text, *fp_pats;
     char pat_buf[MAXBUFSIZE]; /*用于保存从文件中读取的一个模式串*/
     char *line_break; /*换行符*/
     long pats_readed = 0, array_size;
     char **pats_array, **new_array; /*保存所有模式串的数组*/
     char *pat_copy;
     long index;
     size_t text_len = 0;
     int *tag;
     long pat_selected = 0; /*标示有多少个模式串被选中(无重复)*/ 

     fp_text = fopen(text_filename, "w");
     if (!(fp_pats = fopen(pats_filename, "r")))
          terminate("Can not open file!: make_data.c/pats_to_text()\n");

     pats_array = (char **) realloc(NULL, INI_SIZE * sizeof(char *)); 
     array_size = INI_SIZE;
     while (fgets(pat_buf, sizeof(pat_buf), fp_pats)) {
          if (line_break = strchr(pat_buf, '\n'))
               *line_break = '\0';
          if (strlen(pat_buf)) { /*非空模式*/
               pat_copy = cp_str(pat_buf);
               if (array_size == pats_readed) {
                    new_array = (char **) realloc(pats_array, array_size * 2 * sizeof(char *)); 
                    if (!new_array)
                         terminate("Can not enlarge pats array! : make_data.c/ pats_to_text()\n");
                    pats_array = new_array;
                    array_size *= 2;
               }
               pats_array[pats_readed++] = pat_copy;
          }
     }

     if (pats_readed > 0) {
          pats_array = (char **) realloc(pats_array, pats_readed * sizeof(char *)); /*将模式串数组缩小到实际大小*/
          if (!pats_array)
               terminate("Can not resize pats array!: make_data.c/pats_to_text()\n");
          array_size = pats_readed;
	
          tag = (int *) calloc(pats_readed, sizeof(int)); /*用来标示每个模式被选择的情况*/

          srand(time(NULL));
          while (nl--) /*随机选择模式输出*/
               while (np--) {
                    index = ulrand() % pats_readed;
                    fputs(pats_array[index], fp_text);
                    putc(np == 0 ? '\n' : ' ', fp_text);
                    text_len += strlen(pats_array[index]) + 1;
                    if (tag[index]++ == 0)
                         pat_selected++;		
               }
     }

     /*销毁数据结构*/
     for (index = 0; index < array_size; index++)
          free(pats_array[index]);
     free(pats_array);
     free(tag);

     printf("text len: %ld, selected/total: %ld/%ld\n", text_len, pat_selected, pats_readed);
     fclose(fp_pats);
     fclose(fp_text);

}

void text_to_pats(const char *text_filename, const char *pats_filename, long np, int min_pat_len, int max_pat_len)
{
     FILE *fp_text, *fp_pats;
     long index, line_len, lines_readed = 0; 
     char text_buf[MAXBUFSIZE], pat[max_pat_len + 2];
     char *line_break, *new_line;
     char **text_array, **new_array;
     long array_size;
     long pat_len, start_pos;

     if (!(fp_text = fopen(text_filename, "r")))
          terminate("Can not open file!: make_data.c/text_to_pats()\n");
     fp_pats = fopen(pats_filename, "w");

     /*把文本串读入内存*/
     text_array = (char **) realloc(NULL, INI_SIZE * sizeof(char *));
     array_size = INI_SIZE;
     while (fgets(text_buf, sizeof(text_buf), fp_text)) {
          if (line_break = strchr(text_buf, '\n'))
               *line_break = '\0';
          if (strlen(text_buf) >= min_pat_len) {
               new_line = cp_str(text_buf);
               if (array_size == lines_readed) {
                    new_array = (char **) realloc(text_array, array_size * 2 * sizeof(char *));
                    if (!new_array)
                         terminate("Can not enlarge text array!: make_data.c/text_to_pats()\n");
                    text_array = new_array;
                    array_size *= 2;
               }
               text_array[lines_readed++] = new_line;
          }
     }
		

     if (lines_readed > 0) {
          new_array = (char **) realloc(text_array, lines_readed * sizeof(char *)); /*将文本串数组缩小到实际大小*/
          if (!new_array)
               terminate("Can not resize text array! make_data.c/pats_to_text()\n");
          text_array = new_array;
          array_size = lines_readed;

          /*从文本行中随机抽取模式*/
          srand(time(NULL));
          while (np--) {
               index = ulrand() % array_size; /*确定文本行*/
               line_len = strlen(text_array[index]);
               pat_len = (line_len > max_pat_len) ? rand_int(min_pat_len, max_pat_len) : rand_int(min_pat_len, line_len); /*确定模式串长*/
               start_pos = rand() % (line_len - pat_len + 1); /*确定模式串起始位置*/
               strncpy(pat, text_array[index] + start_pos, pat_len);
               pat[pat_len] = '\n';
               pat[pat_len + 1] = '\0';
               fputs(pat, fp_pats);
          }
     }

     for (index = 0; index < array_size; index++)
          free(text_array[index]);
     free(text_array);

     fclose(fp_text);
     fclose(fp_pats);
}

void cre_rand_text(const char *filename, long text_len, int alphabetsize)
{
     FILE *fp_text = fopen(filename, "w");
 
     srand(time(NULL));

     while (text_len--)
          putc(rand() % (alphabetsize - 1) + 1, fp_text);
	
     fclose(fp_text);
}

void cre_rand_pats(const char *filename, long np, int min_pat_len, int max_pat_len, int alphabetsize)
{p
     
     FILE *fp_text = fopen(filename, "w");
     int pat_len;
     int ch;

     srand(time(NULL));
     while (np--) {
          pat_len = rand_int(min_pat_len, max_pat_len); 
          while (pat_len--) {
               do {
                    ch = rand() % (alphabetsize - 1) + 1;
               } while (ch == '\0' || ch == '\n');
               putc(ch, fp_text);
          }
          putc('\n', fp_text);
     }

     fclose(fp_text);
}
