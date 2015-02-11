#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include "qsufsort.h"

int main(int argc, char *argv[])
{
    int i, *sa, *rank;
    int text_len, k, round;
    double duaration;
    clock_t start, finish;
    char file_name[1000];
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

    while (fgets(file_name, sizeof(file_name), file_names_fp))
    	if (strlen(file_name) > 1) {
    	    puts(file_name);
    	    fprintf(results_fp, "\n\n%s \n", file_name);
	    
	    file_name[strlen(file_name)-1] = '\0'; /* 作文件名之前,替换掉换行符 */
	    puts("read text...");
	    text_len = get_file_size(file_name);
	    text = (unsigned char *)malloc((text_len+1));
	    
	    if (!(fp_text = fopen(file_name, "r"))) {
 		perror(fp_text);
		exit(EXIT_FAILURE);
	    }
	    fread(text, text_len, 1, fp_text);	    
	    text[text_len] = 0;

	    puts("alphabet compacting...");
	    start = clock();
	    k = alphabet_compact(text, text_len);
	    finish = clock();
	    duaration = (double)(finish - start) / CLOCKS_PER_SEC;
	    printf("Done: %f\n", duaration);
	    fprintf(results_fp, "compact time: %f\n", duaration);

	    rank = (int *)malloc((text_len + 1) * sizeof(int));
	    sa = (int *)malloc((text_len + 1) * sizeof(int));
	    if (!sa || !rank) {
		fprintf(stderr, "malloc failed\n");
		return 1;
	    }

	    for (duaration = 0, round = 1; round <= ROUNDS; round++) {
		for (i = 0; i < text_len; i++)
		    rank[i] = text[i];
		printf("sorting: round %d...  ", round);
		start = clock();
		suffixsort(rank, sa, text_len, k);
		finish = clock();
		printf("%f\n", (double)(finish - start) / CLOCKS_PER_SEC);
		duaration += (double)(finish - start) / CLOCKS_PER_SEC;
	    }

	    fprintf(results_fp, "Mean sorting time over %d rounds: %f\n", ROUNDS, duaration / ROUNDS);
	    sa_checker_1(sa, rank, text, text_len+1);
	    free(rank); free(sa); free(text);
	}
    fclose(results_fp);
    fclose(file_names_fp);
}
