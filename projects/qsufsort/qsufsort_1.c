#include <limits.h>
#include <stdlib.h>
#include "qsufsort.h"
#include <stdio.h>

static int *sa,                  /* group array, ultimately suffix array.*/
*rank,                          /* inverse array, ultimately inverse of sa.*/
r,                           /* number of symbols aggregated by transform.*/
h;                           /* length of already-sorted prefixes.*/

#define MIN(a, b)	((a) > (b) ? (b) : (a))
#define KEY(p)          (rank[sa[p]+(h)])
#define SWAP(p, q)      ((tmp) = (p), (p) = (q), (q) = (tmp))
#define MED3(a, b, c)   (KEY(a)<KEY(b) ?                        \
	(KEY(b)<KEY(c) ? (b) : KEY(a)<KEY(c) ? (c) : (a))       \
	: (KEY(b)>KEY(c) ? (b) : KEY(a)>KEY(c) ? (c) : (a)))

/* Subroutine for select_sort_split and sort_split. Sets group numbers for a
group whose lowest position in I is pl and highest position is pm.*/

static void update_group(int first, int last)
{
    if (first == last) {   /* one element, sorted group.*/
	rank[sa[first]] = last;
	sa[first] = -1;
    } else  /* more than one element, unsorted group.*/
	while (first <= last)
	    rank[sa[first++]] = last;	
}

/* Quadratic sorting method to use for small subarrays. To be able to update
group numbers consistently, a variant of selection sorting is used.*/

static void select_sort_split(int first, int last) /*no recurrence*/
{
    int group_last, i, smallest, key, tmp;

    while (first < last) {
	for (i = group_last = first + 1, smallest = KEY(first); i <= last; i++)
	    if ((key = KEY(i)) < smallest) {
		smallest = key;   /* f is smallest key found.*/
		SWAP(sa[i], sa[first]);  /* place smallest element at firstning.*/
		group_last = first + 1;  /* group_last is position for elements equal to f.*/
	    } else if (key == smallest) {     /* if equal to smallest key.*/
		SWAP(sa[i], sa[group_last]);       /* place next to other smallest elements.*/
		group_last++;
	    }

	update_group(first, group_last - 1);   /* update group values for new group.*/
	first = group_last;   /* continue sorting rest of the subarray.*/
    }

    if (first == last) {   /* check if last part is single element.*/
	rank[sa[first]] = first;
	sa[first] = -1;   /* sorted group.*/
    }
}

/* Subroutine for sort_split, algorithm by Bentley & McIlroy.*/


static int choose_pivot(int first, int last) 
{
    int  middle, n;
    int s;
	
    n = last - first + 1;
    middle = first + (n >> 1);  /* small arrays, middle = p + n/2 is the middle element. */
	
    if (n > 7) {
	if (n > 40) {   /* big arrays, pseudomedian of 9.*/
	    s = n >> 3;   /*s = n/8*/
	    first = MED3(first, first + s, first + s + s);
	    middle = MED3(middle - s, middle, middle + s);
	    last = MED3(last - s - s, last - s, last);
	}
	middle = MED3(first, middle, last);      /* midsize arrays, median of 3.*/
    }

    return KEY(middle);
}

/* Sorting routine called for each unsorted group. Sorts the array of integers
(suffix numbers) of length n starting at p. The algorithm is a ternary-split
quicksort taken from Bentley & McIlroy, "Engineering a Sort Function",
Software -- Practice and Experience 23(11), 1249-1265 (November 1993). This
function is based on Program 7.*/

static void sort_split(int first, int last)
{
    int equal_l, less, larger, equal_r, i, j;
    int key, pivot, min, less_num, larger_num, tmp;

    if (last - first + 1 < 7) {     /* multi-selection sort smallest arrays.*/
	select_sort_split(first, last);
	return;
    }
	
    pivot = choose_pivot(first, last); /*v is the key of some index*/
    equal_l = less = first;
    larger = equal_r = last;
       
    while (1) {   /* split-end greaterrtition.*/
	while (less <= larger && (key = KEY(less)) <= pivot) {
	    if (key == pivot) {
		SWAP(sa[less], sa[equal_l]);
		equal_l++;
	    }
	    less++;
	}
	while (larger >= less && (key = KEY(larger)) >= pivot) {
	    if (key == pivot) {
		SWAP(sa[larger], sa[equal_r]);
		equal_r--;
	    }
	    larger--;
	}

	if (less > larger)
	    break;
	SWAP(sa[less], sa[larger]);
	less++;
	larger--;
    }

    less_num = less - equal_l;
    larger_num = equal_r - larger;

    min = MIN(equal_l - first, less_num);
    for (i = first, j = less - min; min; min--, i++, j++)
	SWAP(sa[i], sa[j]);
	
    min = MIN(larger_num, last - equal_r);
    for (i = less, j = last - min + 1; min; min--, i++, j++)
	SWAP(sa[i],sa[j]);

    if (less_num > 0)
	sort_split(first, first + less_num - 1);

    update_group(first + less_num, last - larger_num);

    if (larger_num > 0)
	sort_split(last - larger_num + 1, last);
}

/* Bucketsort for first iteration.

Input: x[0...n-1] holds integers in the range 1...k-1, all of which appear
at least once. x[n] is 0. (This is the corresponding output of transform.) k
must be at most n+1. p is array of size n+1 whose contents are disregarded.

Output: x is V and p is I after the initial sorting stage of the refined
suffix sorting algorithm.*/

static void bucketsort(int n, int alphabet_size)
{
    int  i, j, current, next, group_num;

    for (i = 0; i < alphabet_size; i++)  /*0 ~ k is the alphabet. sa is the list head*/
	sa[i] = -1;  /* mark linked lists empty. */
    for (i = 0; i <= n; i++) {
	current = rank[i];
	rank[i] = sa[current];  /* insert in linked list.*/
	sa[current] = i;
    }

    for (j = alphabet_size - 1, i = n; j >= 0; j--) { /* 每次遍历以某个字符开头的所有后缀 */
	group_num = i;
	current = sa[j];	/*current != -1*/
		
	if (rank[current] < 0) { /* only one element */
	    rank[current] = group_num;
 	    sa[i--] = -1;
	} else 
	    do {
		next = rank[current];
		rank[current] = group_num;
		sa[i--] = current;
		current = next;
	    } while (current >= 0);
    }
}

/* Transforms the alphabet of x by attempting to aggregate several symbols into
one, while preserving the suffix order of x. The alphabet may also be
compacted, so that x on output comprises all integers of the new alphabet
with no skipped numbers.

Input: x is an array of size n+1 whose first n elements are positive
integers in the range l...k-1. p is array of size n+1, used for temporary
storage. q controls aggregation and compaction by defining the maximum value
for any symbol during transformation: q must be at least k-l; if q<=n,
compaction is guaranteed; if k-l>n, compaction is never done; if q is
INT_MAX, the maximum number of symbols are aggregated into one.

Output: Returns an integer j in the range 1...q representing the size of the
new alphabet. If j<=n+1, the alphabet is compacted. The global variable r is
set to the number of old symbols grouped into one. Only x[n] is 0.*/

static int transform(int n, int max_char, int q)
{
    int start_chunk, chunk, max_chunk,incr, e, i, j, m, s, chunk_size;

    for (s = 0, i = max_char; i; i >>= 1)  /* s is number of bits in old symbol.*/
	s++;

    e = INT_MAX >> s;   /* e is for overflow checking.*/

    /*同时完成两个任务：计算start_chunk和max_chunk,它们相互独立*/
    for (start_chunk = max_chunk = chunk_size = 0; chunk_size < n && max_chunk <= e && (chunk = max_chunk << s | max_char) <= q; chunk_size++) {
	start_chunk = start_chunk << s | rank[chunk_size];  /* b is start of x in chunk alphabet.(min symbol)*/
	max_chunk = chunk;   /* d is max symbol in chunk alphabet.*/
    }

    r = chunk_size;
     
    m = (1 << (chunk_size - 1) * s) - 1;   /* m masks off top old symbol from chunk.*/
    rank[n] = 0;  /* emulate zero terminator.*/

    if (max_chunk <= n) {   /* if bucketing possible, compact alphabet.*/
	for (i = 0; i <= max_chunk; i++) /* zero transformation table.*/
	    sa[i] = 0;

	for (i = chunk_size, chunk = start_chunk; i <= n; i++) {
	    sa[chunk] = 1;  /* mark used chunk symbol.*/
	    chunk = (chunk & m) << s | rank[i];  /* shift in next old symbol in chunk.*/
	}
	for (i = 1; i < chunk_size; i++) {  /* handle last r-1 positions., i=n 时不需要处理*/
	    sa[chunk] = 1;   /* mark used chunk symbol.*/
	    chunk = (chunk & m) << s;   /* shift in next old symbol in chunk.*/
	}

	for (i = 0, incr = 1; i <= max_chunk; i++)
	    if (sa[i])
		sa[i] = incr++;  /* j is new alphabet size	.*/
	 	
	for (i = 0, j = chunk_size, chunk = start_chunk; j <= n; i++, j++) {
	    rank[i] = sa[chunk];
	    chunk = (chunk & m) << s | rank[j];  /* shift in next old symbol in chunk.*/
	}
	while (i < n) {  /* handle last r-1 positions.*/
	    rank[i++] = sa[chunk];  /* transform to new alphabet.*/
	    chunk = (chunk & m) << s;   /* shift right-end zero in chunk.*/
	}
    } else {  /* bucketing not possible, don't compact.*/
	for (i = 0, j = chunk_size, chunk = start_chunk; j <= n; i++, j++) {
	    rank[i] = chunk;
	    chunk = (chunk & m) << s | rank[j];  /* shift in next old symbol in chunk.*/
	}
	
	while (i < n) {  /* handle last r-1 positions.*/
	    rank[i++] = chunk;  /* transform to new alphabet.*/
	    chunk = (chunk & m) << s;   /* shift right-end zero in chunk.*/
	}

	incr = max_chunk++;    /* new alphabet size.*/
    }

    return incr; /* return new alphabet size. 包括0*/
}

/* Makes suffix array p of x. x becomes inverse of p. p and x are both of size
n+1. Contents of x[0...n-1] are integers in the range l...k-1. Original
contents of x[n] is disregarded, the n-th symbol being regarded as
end-of-string smaller than all other symbols.*/

void suffixsort(int *x, int *p, int n, int max_char)
{
    int group_head, group_end;
    int i, new_alphabet, v, sorted_len;

    rank = x;  /* set global values.*/
    sa = p;

    if (n >= max_char) {  /* if bucketing possible,*/
	new_alphabet = transform(n, max_char, n);
	bucketsort(n, new_alphabet);   /* bucketsort on first r positions.*/
    } else {
	transform(n, max_char, INT_MAX);
	for (i = 0; i <= n; i++)
	    sa[i] = i;  /* initialize I with suffix numbers.*/
	h = 0;
	sort_split(0, n);  /* quicksort on first r positions.*/
    }

    h = r;  /* number of symbols aggregated by transform.*/

    while (sa[0] >= -n) {
	sorted_len = group_head = 0;  /* group_head is first position of group.sorted_len is negated length of sorted groups*/
	do {
	    if ((v = sa[group_head]) < 0) {
		group_head -= v;   /* skip over sorted group.*/
		sorted_len += v;   /* add negated length to sl.*/
	    } else {
		if (sorted_len) {
		    sa[group_head+sorted_len] = sorted_len;  /* combine sorted groups before pi.*/
		    sorted_len = 0;
		}
		sort_split(group_head, group_end = rank[v]);
		group_head = group_end + 1;  /* next group.*/
	    }
	} while (group_head <= n);
	/* if the array ends with a sorted group.*/
	if (sorted_len)
	    sa[group_head+sorted_len] = sorted_len; /* combine sorted groups at end of sa.*/
	/* double sorted-depth.*/
	h *= 2;
    }

    for (i = 0; i <= n; i++)   /* reconstruct suffix array from inverse.*/
	sa[rank[i]] = i;

#ifdef SHOW_RESULTS    
    for (i = 0; i <= n; i++)   /* 在屏幕上显示排序结果*/
    	printf("%d ",sa[i]);
#endif
}
