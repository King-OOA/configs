#ifndef	MAKE_DATA_H	
#define MAKE_DATA_H

#define MAXBUFSIZE 50000
#define INI_SIZE 10000


void pats_to_text(const char *, const char *, long, long); /*从模式串生成文本*/
void text_to_pats(const char *, const char *, long, int, int);

void cre_rand_text(const char *, long, int);
void cre_rand_pats(const char *, long, int, int, int);

#endif
