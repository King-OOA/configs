#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "libcommon.h"
#include "make_data.h"
			

void pats_to_text(const char *pats_filename, const char *text_filename, long nl, long np) /*��ģʽ�������ı�*/
{
     FILE *fp_text, *fp_pats;
     char pat_buf[MAXBUFSIZE]; /*���ڱ�����ļ��ж�ȡ��һ��ģʽ��*/
     char *line_break; /*���з�*/
     long pats_readed = 0, array_size;
     char **pats_array, **new_array; /*��������ģʽ��������*/
     char *pat_copy;
     long index;
     size_t text_len = 0;
     int *tag;
     long pat_selected = 0; /*��ʾ�ж��ٸ�ģʽ����ѡ��(���ظ�)*/ 

     fp_text = fopen(text_filename, "w");
     if (!(fp_pats = fopen(pats_filename, "r")))
          terminate("Can not open file!: make_data.c/pats_to_text()\n");

     pats_array = (char **) realloc(NULL, INI_SIZE * sizeof(char *)); 
     array_size = INI_SIZE;
     while (fgets(pat_buf, sizeof(pat_buf), fp_pats)) {
          if (line_break = strchr(pat_buf, '\n'))
               *line_break = '\0';
          if (strlen(pat_buf)) { /*�ǿ�ģʽ*/
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
          pats_array = (char **) realloc(pats_array, pats_readed * sizeof(char *)); /*��ģʽ��������С��ʵ�ʴ�С*/
          if (!pats_array)
               terminate("Can not resize pats array!: make_data.c/pats_to_text()\n");
          array_size = pats_readed;
	
          tag = (int *) calloc(pats_readed, sizeof(int)); /*������ʾÿ��ģʽ��ѡ������*/

          srand(time(NULL));
          while (nl--) /*���ѡ��ģʽ���*/
               while (np--) {
                    index = ulrand() % pats_readed;
                    fputs(pats_array[index], fp_text);
                    putc(np == 0 ? '\n' : ' ', fp_text);
                    text_len += strlen(pats_array[index]) + 1;
                    if (tag[index]++ == 0)
                         pat_selected++;		
               }
     }

     /*�������ݽṹ*/
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

     /*���ı��������ڴ�*/
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
          new_array = (char **) realloc(text_array, lines_readed * sizeof(char *)); /*���ı���������С��ʵ�ʴ�С*/
          if (!new_array)
               terminate("Can not resize text array! make_data.c/pats_to_text()\n");
          text_array = new_array;
          array_size = lines_readed;

          /*���ı����������ȡģʽ*/
          srand(time(NULL));
          while (np--) {
               index = ulrand() % array_size; /*ȷ���ı���*/
               line_len = strlen(text_array[index]);
               pat_len = (line_len > max_pat_len) ? rand_int(min_pat_len, max_pat_len) : rand_int(min_pat_len, line_len); /*ȷ��ģʽ����*/
               start_pos = rand() % (line_len - pat_len + 1); /*ȷ��ģʽ����ʼλ��*/
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
