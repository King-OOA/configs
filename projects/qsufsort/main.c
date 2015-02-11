#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include "qsufsort.h"
#include "tools.h"


extern unsigned loops, sort_split_times, select_sort_times;

int main(int argc, char *argv[])
{
     int text_len, k, round;
     double duaration;
     clock_t start, finish;
     char file_name[1000], *version;
     FILE *results_fp, *file_names_fp, *fp_text;
     unsigned char *text;

     if (!(results_fp = fopen(RESULTS_FILE, "a"))) {
          perror(RESULTS_FILE);
          return 1;
     }

     if (!(file_names_fp = fopen(TEXT_FILES_NAMES, "r"))) {
          perror(TEXT_FILES_NAMES);
          return 1;
     }
     
     /* all_a(200000000,50); */
     
     version = (VERSION == 0 ? "原始版本!!!!! \n" : "改进版本!!!\n");
     fprintf(results_fp, version, file_name);
     
     while (fgets(file_name, sizeof(file_name), file_names_fp))
          if (strlen(file_name) > 1 && file_name[0] != ';') {
               puts(file_name);
               fprintf(results_fp, "\n\n%s \n", file_name);
	    
               filec_name[strlen(file_name)-1] = '\0'; /* 作文件名之前,替换掉换行符 */
               puts("read text...");
               text_len = get_file_size(file_name);
               text = MALLOC("text", text_len + 1, unsigned char);
	    
               if (!(fp_text = fopen(file_name, "r"))) {
                    perror(file_name);
                    exit(EXIT_FAILURE);
               }
               fread(text, text_len, 1, fp_text);	    
               text[text_len] = 0;

               puts("alphabet compacting...");
               k = alphabet_compact(text, text_len);

               for (duaration = 0, round = 1; round <= ROUNDS; round++) {
                    printf("sorting: round %d...  ", round);
                    start = clock();
                    qsufsort(text, text_len+1, k);
                    finish = clock();
                    printf("%f\n", (double)(finish - start) / CLOCKS_PER_SEC);
                    duaration += (double)(finish - start) / CLOCKS_PER_SEC;
               }
            
               fprintf(results_fp, "loops:%d\n", loops);
               fprintf(results_fp, "sort_split:%d\n", sort_split_times);
               fprintf(results_fp, "select_sort:%d\n", select_sort_times);
               fprintf(results_fp, "Mean sorting time over %d rounds: %f\n", ROUNDS, duaration / ROUNDS);
          }
     fclose(results_fp);
     fclose(file_names_fp);
}
