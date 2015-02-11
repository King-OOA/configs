#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <sys/stat.h>
#include "textools.h"

#undef malloc

unsigned char buf[1024*1024]; /* 1MB buffer */

char *str_cat(const char *str1, const char *str2)
{
    char *new_str = MALLOC(strlen(str1) + strlen(str2) + 1, char, __FILE__, __LINE__);

    return strcat(strcpy(new_str, str1), str2);
}

int rand_int(int low, int high)
{
    double d = (double) rand() / ((double) RAND_MAX + 1);
    int k = (int) (d * (high - low + 1));

    return low + k;
}

/* 截取文件iname的前 n MB 个字节 */
void cut_file(char const *iname, int n)
{
    char oname[1024];
    FILE *ifile,*ofile;

    if (n <= 0) {
	fprintf(stderr,"So <mb> must be positive!\n");
	exit(EXIT_FAILURE);
    }

    /* 输出文件名, 和输入文件同属一个目录*/
    sprintf(oname,"%s.%iMB",iname,n);

    /* fopen64 为linux 特有, 可以打开超过2GB的文件 */
    if (!(ifile = fopen64(iname,"r"))) {
        perror(iname);
        exit(EXIT_FAILURE);
    }
    
    if (!(ofile = fopen64(oname,"w"))) {
        perror(oname);
        exit(EXIT_FAILURE);
    }

    while (n--) {
	if (fread (buf,1024*1024,1,ifile) != 1) { /* 每次读1MB */
	    fprintf(stderr,"Error: cannot read %s\n",iname);
	    fprintf(stderr, "%s\n", strerror(errno));
	    if (errno == 0)
		fprintf(stderr,"Maybe the file is too short?\n");
	    fclose (ifile); fclose (ofile);
	    unlink (oname); 	/* 删除oname文件*/
	    exit(EXIT_FAILURE);
	}
	if (fwrite (buf,1024*1024,1,ofile) != 1) { /* 每次写1MB */
	    fprintf(stderr,"Error: cannot write %s\n",oname);
	    fprintf(stderr, "%s\n", strerror(errno));
	    fclose (ifile); fclose (ofile);
	    unlink (oname);
	    exit(EXIT_FAILURE);
	}
    }

    fclose (ifile);fclose(ofile);
    
    printf("Successfully created %s.\n",oname);
}

static int Seed;
#define ACMa 16807
#define ACMm 2147483647
#define ACMq 127773         
#define ACMr 2836
#define hi (Seed / ACMq)
#define lo (Seed % ACMq)
  
static int fst = 1;

/* 用1~alphabet中的字符填充buf的前n个字符*/
static void fill_buffer(unsigned char *buf, int n, int alphabet)
{
    int i;
    long test;
    struct timeval t;
     
    if (fst) {
        gettimeofday(&t, NULL);
        Seed = t.tv_sec * t.tv_usec;
        fst = 0;
    }
 
   for (i = 0; i < n; i++) {
        Seed = ((test = ACMa * lo - ACMr * hi) > 0) ? test : test + ACMm;
        buf[i] = 1 + ((double)Seed) * alphabet / ACMm;
    }
}

/* 生成 file_name, 大小为 n MB, 字符集为alphabet  */
void fill_file(char const *file_name, int n, int alphabet)
{
    FILE *ofile;

    if ((n < 0) || (n > 4096)) {
        fprintf (stderr,"Error: file length (which is given in MB) must"
                 " be between 0 and 4096 (= 4 GB file)\n");
        exit(EXIT_FAILURE);
    }

    if ((alphabet < 1) || (alphabet > 256)) {
        fprintf (stderr,"Error: alphabet size must be between 1 and 256\n");
        exit(EXIT_FAILURE);
    }

    if (alphabet == 256) {
        fprintf (stderr,"Warning: several indexes need alphabet size < 256"
                 " to work properly.\n Generating anyway.\n");
    }

    if ((ofile = fopen(file_name,"w")) == NULL) {
        perror(file_name);
        exit(EXIT_FAILURE);
    }

    while (n--) {
        fill_buffer(buf, 1024*1024, alphabet); /* 先填充块儿 */
        if (fwrite (buf,1024*1024,1,ofile) != 1) { /* 再将块儿写入文件 */
            fprintf (stderr,"Error: cannot write %s\n",file_name);
            fprintf(stderr, "%s\n", strerror(errno));
            fclose(ofile);
            unlink (file_name);
            exit(EXIT_FAILURE);
        }
    }

    fclose(ofile);

    fprintf (stderr,"File %s successfully generated\n",file_name);
}

/* 计算文件的字符集大小 */
int file_alphabet(char const *filename)
{
    int i, n;
    FILE *ifile;
    size_t sigma[256] = {0};
    

    if (!(ifile = fopen(filename,"r"))) {
        perror(filename);
        exit(EXIT_FAILURE);
    }
    
    /* 分块读取文件, 并累加各字符数 */
    while (n = fread(buf, 1, 1024*1024, ifile))
        while (n--)
            sigma[buf[n]]++;

    /* 统计各字符的出现次数 */
    for (i = 0; i < 256; i++) 
        if (sigma[i]) {
            n++;
            printf (" %c: %lu", i, sigma[i]);
        }
    
    printf ("\n\nFile %s has %i different characters\n", filename, n); 

    fclose (ifile);
    
    return n;
}

int aleat (top)
{
    long test;
    struct timeval t;
     
    if (fst) {
        gettimeofday(&t, NULL);
        Seed = t.tv_sec * t.tv_usec;
        fst = 0;
    }
     
    Seed = ((test = ACMa * lo - ACMr * hi) > 0) ? test : test + ACMm;
    return ((double) Seed) * top / ACMm;
}

/* 将字符串forbid中的字符序列,转化为真正的转义字符序列存入*forbide中 */
static void parse_forbid(char const *forbid, char ** forbide)
{
     int len, i, j;

     len = strlen(forbid);

     *forbide = MALLOC(len + 1, char, __FILE__, __LINE__);
     
     for(i = 0, j = 0; i < len; i++) { /* 遍历forbid字符串 */
         if (forbid[i] != '\\') {
	       if(forbid[i] != '\n')
		    (*forbide)[j++] = forbid[i];
	  } else { 
	       i++;
	       if(i == len) {
		    /* forbid[i-1] = '\0'; */
		    (*forbide)[j] = '\0';
		    fprintf (stderr, "Not correct forbidden string: only one \\\n");
		    return;
	       }
	       switch (forbid[i]) {
		   case'n':  (*forbide)[j++] = '\n'; break;
		   case'\\': (*forbide)[j++] = '\\'; break;
		   case'b':  (*forbide)[j++] = '\b'; break;				
		   case'e':  (*forbide)[j++] = '\e'; break;
		   case'f':  (*forbide)[j++] = '\f'; break;
		   case'r':  (*forbide)[j++] = '\r'; break;
		   case't':  (*forbide)[j++] = '\t'; break;
		   case'v':  (*forbide)[j++] = '\v'; break;
		   case'a':  (*forbide)[j++] = '\a'; break;
		   case'c':  
			if(i+3 >= len) {
			     /* forbid[i-1] = '\0'; */
			     (*forbide)[j] = '\0';
			     fprintf (stderr, "Not correct forbidden string: 3 digits after \\c\n");
			     return;
			}
			(*forbide)[j++] = (forbid[i+1]-48)*100 +
			     (forbid[i+2]-48)*10 + (forbid[i+3]-48); 
			i+=3;
			break;					
		   default:
			fprintf (stdout, "Unknown escape sequence '\\%c'in forbidden string\n", forbid[i]);
			break;
	       }
	  }
     }
     (*forbide)[j] = '\0';
}

/* 从文件file中, 抽取pat_num个模式串, 构成模式串文件pat_file, 串长分布:min_pat_len~max_pat_len, 禁止的字符在保存在forbid中 */
void gen_patterns(char const *file, long pat_num, int min_pat_len, int max_pat_len, char const *pat_file, char const *forbid)
{
    int pat_len, n, file_len;
    struct stat sdata;
    FILE *ifile, *ofile, *infofile;
    unsigned char *buff;
    char *forbide = NULL;
    int *pat_len_tab;
    char *infofile_name;

    if (stat(file, &sdata) != 0) {
        fprintf(stderr, "Error: cannot stat file %s\n", file);
        exit(EXIT_FAILURE);
    }
    
    file_len = sdata.st_size; 	/* length of file */

    if (min_pat_len <= 0 || max_pat_len <= 0 || min_pat_len > max_pat_len || max_pat_len > file_len) {
        fprintf(stderr,"Error: pat length must be >= 1 and <= file length");
        exit(EXIT_FAILURE);
    }

    if (pat_num < 1) {
        fprintf(stderr, "Error: number of patterns must be >= 1\n");
        exit(EXIT_FAILURE);
    }
     
    parse_forbid(forbid, &forbide);

    if (!(ifile = fopen (file, "r"))) {
        perror(file);
        exit(EXIT_FAILURE);
    }
    
    if ((pat_len_tab = (int *) calloc(max_pat_len + 1, sizeof(int))) == NULL) {
        fprintf(stderr, "Error: can not calloc memory! file:%s, line:%d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    buff = MALLOC(file_len, unsigned char , __FILE__, __LINE__);
    
    if (fread (buff, file_len, 1, ifile) != 1)  { /* read the whole file into buf */
        fprintf(stderr, "Can not fread file: %s\n", file);
        exit(EXIT_FAILURE);
    }
    
    fclose (ifile);

    if (!(ofile = fopen(pat_file, "w"))) {
        perror(pat_file);
        exit(EXIT_FAILURE);
    }

    if ((infofile = fopen((infofile_name = str_cat(pat_file, ".info")), "w")) == NULL) {
        free(infofile_name);
        perror(infofile_name);
        exit(EXIT_FAILURE);
    }

    for (n = 0; n < pat_num; n++) {
        int start_position, i;
        pat_len = rand_int(min_pat_len, max_pat_len);
        pat_len_tab[pat_len]++;
        do { /*对每一个产生的起始位置,检查该位置的pat是否包含禁止字符,如果包含,则重新产生一个起始位置,直到该位置的pat不包含任何禁止字符 */
            start_position = aleat (file_len - pat_len + 1);
            for (i = 0; i < pat_len; i++)
                if (strchr(forbide, buff[start_position+i]))
                    break;
        } while (i < pat_len);
	
        for (i = 0; i < pat_len; i++) 
            putc(buff[start_position+i], ofile);
        
        putc('\n', ofile);      /* 模式串以行为单位 */
    }

    fclose(ofile);

    for (n = min_pat_len; n <= max_pat_len; n++)
        fprintf(infofile, "%d: %d\n", n, pat_len_tab[n]);
	  
    printf("File %s successfully generated\n", pat_file);

    free(buff);
    free(forbide);
    free(pat_len_tab);
    free(infofile_name);
}

void *alloc(size_t size, const char *file, int line)
{
    void *new_mem;
    
    if (!(new_mem = malloc(size))) {
         fprintf(stderr, "Out of memory! file:%s, line:%d", file, line);
         exit(EXIT_FAILURE);
    }
    
    return new_mem;
}
