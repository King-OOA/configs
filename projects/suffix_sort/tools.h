#ifndef TOOLS_H
#define TOOLS_H
#include  <stdio.h>
#include  <stdlib.h>

#define malloc /* 不允许直接调用malloc函数 */

size_t  get_file_size(char const *);
int read_text(char const *, unsigned char *);
int alphabet_compact(unsigned char *, int);
void *alloc(const char *, size_t);
void all_a(int, int);
#define MALLOC(obj,num,type) (type*)alloc((obj),(num)*sizeof(type))

#endif






