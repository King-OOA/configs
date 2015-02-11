#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "qsufsort.h"
#include "tools.h"

#define FIRST_CHAR(i) text[sa[(i)]]
#undef malloc

void *alloc(const char *obj_name,size_t size)
{
    void *new_mem;
    
    if (!(new_mem = malloc(size))) {
	fprintf(stderr, "Can not allocate memory for: %s\n", obj_name);
	exit(EXIT_FAILURE);
    }
    
    return new_mem;
}

/* 返回文本长度 */
size_t  get_file_size(char const *file_name)
{
    FILE *fp;
    size_t size;

    if (!(fp = fopen(file_name, "r"))) {
	perror(file_name);
	exit(EXIT_FAILURE);
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fclose(fp);
    
    return size;
}

/* int read_text(char const *file_name, unsigned char *text) */
/* { */
/*     FILE *fp_text; */
/*     int text_len; */

/*     if (!(fp_text = fopen(file_name, "r"))) { */
/* 	perror(file_name); */
/* 	exit(EXIT_FAILURE); */
/*     } */

/*     fread(text, text_len, 1, fp_text); */

/*     return text_len; */
/* } */

 /* 压缩后字符范围: 1~j,共j个字符 */

int alphabet_compact(unsigned char *text, int text_len)
{
    unsigned char alphabet[256] = { 0 };
    int i, j;

    for (i = 0; i < text_len; i++)
	alphabet[text[i]] = 1;

    for (j = i = 0; i < 256; i++)
	if (alphabet[i])
	    alphabet[i] = ++j;

    for (i = 0; i < text_len; i++)
	text[i] = alphabet[text[i]];
    
    return j;
}

void sa_checker(int *sa, unsigned char *text, int n)
{
    int bucket[256], c;
    int i;
    
    for(i = 0; i <256; i++)
	bucket[i] = -1;
    

    for (i = n - 1; i >= 0; i--)
	bucket[text[sa[i]]] = i;
    bucket[0]++;
    
    for (i = 0; i < n; i++)
	if (sa[i] > 1) {
	    c = text[sa[i]-1];
	    if (sa[bucket[c]] + 1 != sa[i]) {
		fprintf(stderr, "wrong! %d\n",i);
		exit(EXIT_FAILURE);
	    }
	    bucket[c]++;
	}
    
    printf("Correct!\n");
}

void sa_checker_1(int *sa, int *rank,unsigned char *text, int n)
{
    int i;

    for (i = 0; i < n && sa[i] >= 0 && sa[i] < n; i++)
	;

    if (i != n) {
	fprintf(stderr, "wrong! %d\n",i);
	exit(EXIT_FAILURE);
    }

    for (i = 1; i < n && ((FIRST_CHAR(i-1) < FIRST_CHAR(i)) || ((FIRST_CHAR(i-1) == FIRST_CHAR(i) && rank[sa[i-1]+1] < rank[sa[i]+1]))); i++)
	;

    if (i != n) {
	fprintf(stderr, "wrong! %d\n",i);
	exit(EXIT_FAILURE);
    }
    
    printf("Correct!\n");
}

void all_a(int n, int alphabet)
{
    FILE *fp;
    
    fp = fopen("/home/pz/data/aaa", "w");
    
    while (n--) 
         fputc(n % alphabet, fp);
    
    fclose(fp);
}

    
