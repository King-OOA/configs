#include <stdio.h>
#include <limits.h>

typedef unsigned char* byte_pointer;


void show_bytes(byte_pointer start, int len)
{
    while (len--)
	printf("%.2x ", *start++);
    
    putchar('\n');
}

void show_int(int x)
{
    show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x)
{
    show_bytes((byte_pointer) &x, sizeof(float));
}
	
void show_pointer(void *x)
{
    show_bytes((byte_pointer) &x, sizeof(void *));
}

int uadd_ok(unsigned a, unsigned b)
{
    return a + b > a;
}

int main(int argc, char **argv)
{
    unsigned  c;
    c = UINT_MAX + UINT_MAX;
    
    printf("%d\n", uadd_ok(1,1));

}
