#ifndef TEXTOOLS_H
#define TEXTOOLS_H

#define MALLOC(num,type,file,line) (type *)alloc((num)*sizeof(type), file, line)
#define malloc

void *alloc(size_t , const char *, int);

char *str_cat(const char *, const char *);
int rand_int(int, int);
void cut_file(char const *, int);
void fill_file(char const *, int, int);
int file_alphabet(char const *);
void gen_patterns(char const *, long, int, int, char const *, char const *);

#endif

