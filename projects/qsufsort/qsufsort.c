#include <limits.h>
#include <stdlib.h>
#include "qsufsort.h"
#include <stdio.h>
#include "tools.h"

static int *sa,                  /* group array, ultimately suffix array.*/
     *rank,                          /* inverse array, ultimately inverse of sa.*/
     *lcp,
     chunk_size;                           /* number of symbols aggregated by transform.*/


unsigned  sort_split_times;
unsigned  select_sort_times;
unsigned  loops;

#define MIN(a, b)	((a) > (b) ? (b) : (a))
#define KEY(p, k)          (rank[sa[p]+(k)])
#define SWAP(p, q)      ((tmp) = (p), (p) = (q), (q) = (tmp))

/* #define MED3(a, b, c, k)   (KEY(a, k)<KEY(b, k) ?			\ */
/* 			    (KEY(b, k)<KEY(c, k) ? (b) : KEY(a, k)<KEY(c, k) ? (c) : (a)) \ */
/* 			    : (KEY(b, k)>KEY(c, k) ? (b) : KEY(a, k)>KEY(c, k) ? (c) : (a))) */


int MED3(int a, int b, int c, int bucket_lcp)
{
     int v1, v2, v3;
    
     v1 = KEY(a, bucket_lcp);
     v2 = KEY(b, bucket_lcp);
     v3 = KEY(c, bucket_lcp);
    
     return (v1 < v2 ?
             (v2 < v3 ? b : (v1 < v3 ? c : a))
             :(v2 > v3 ? b : (v1 > v3 ? c : a)));
}

static void update_group(int first, int last)
{
     if (first == last) {   /* one element, sorted group.*/
          rank[sa[first]] = last;
          sa[first] = -1;
     } else  {/* more than one element, unsorted group.*/
          while (first <= last)
               rank[sa[first++]] = last;	
     }
}

static void select_sort_split(int first, int last, int bucket_lcp) /*no recurrence*/
{
     int group_last, i, smallest, key, tmp, v;
     select_sort_times++;

     while (first < last) {
          for (i = group_last = first + 1, smallest = KEY(first, bucket_lcp); i <= last; i++)
               if ((key = KEY(i, bucket_lcp)) < smallest) {
                    smallest = key;  
                    SWAP(sa[i], sa[first]);
                    group_last = first + 1;
               } else if (key == smallest) {     /* if equal to smallest key.*/
                    SWAP(sa[i], sa[group_last]);       /* place next to other smallest elements.*/
                    group_last++;
               }
	
          update_group(first, v = group_last - 1);   /* update group values for new group.*/

#if (VERSION == 1)
          if (first != v)
               lcp[v] = bucket_lcp + lcp[smallest];
#endif     
	
          first = group_last;   /* continue sorting rest of the subarray.*/
     } 

     if (first == last) {   /* check if last part is single element.*/
          rank[sa[first]] = first;
          sa[first] = -1;   /* sorted group.*/
     }
}

static int choose_pivot(int first, int last, int bucket_lcp) 
{
     int  middle, n;
     int s;
	
     n = last - first + 1;
     middle = first + (n >> 1);  /* small arrays, middle = p + n/2 is the middle element. */
	
     if (n > 7) {
          if (n > 40) {   /* big arrays, pseudomedian of 9.*/
               s = n >> 3;   /*s = n/8*/
               first = MED3(first, first + s, first + s + s, bucket_lcp);
               middle = MED3(middle - s, middle, middle + s, bucket_lcp);
               last = MED3(last - s - s, last - s, last, bucket_lcp);
          }
          middle = MED3(first, middle, last, bucket_lcp);      /* midsize arrays, median of 3.*/
     }

     return KEY(middle, bucket_lcp);
}

static void sort_split(int first, int last, int bucket_lcp)
{
     int equal_l, less, larger, equal_r, i, j, temp_1, temp_2;
     int key, pivot, min, less_num, larger_num, tmp;
    
     if (last - first + 1 < 7) {     /* multi-selection sort smallest arrays.*/
          select_sort_split(first, last, bucket_lcp);
          return;
     }
    
     sort_split_times++;

     pivot = choose_pivot(first, last, bucket_lcp); /*v is the key of some index*/
     equal_l = less = first;
     larger = equal_r = last;
       
     while (1) {   /* split-end greaterrtition.*/
          while (less <= larger && (key = KEY(less, bucket_lcp)) <= pivot) {
               if (key == pivot) {
                    SWAP(sa[less], sa[equal_l]);
                    equal_l++;
               }
               less++;
          }
          while (larger >= less && (key = KEY(larger, bucket_lcp)) >= pivot) {
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
          sort_split(first, first + less_num - 1, bucket_lcp);

     update_group(temp_1 = first + less_num, temp_2 = last - larger_num);

#if (VERSION == 1)
     if (temp_1 != temp_2)
          lcp[temp_2] = bucket_lcp + lcp[pivot];
#endif     

     if (larger_num > 0)
          sort_split(last - larger_num + 1, last, bucket_lcp);
}

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
          } else {
               do {
                    next = rank[current];
                    rank[current] = group_num;
                    sa[i--] = current;
                    current = next;
               } while (current >= 0);
#if (VERSION == 1)	    
               lcp[group_num] = chunk_size;
#endif 
          }
     }
}

static int transform(int n, int max_char, int q)
{
     int start_chunk, chunk, max_chunk,incr, e, i, j, mask, s;

     for (s = 0, i = max_char; i; i >>= 1)  /* s is number of bits in old symbol.*/
          s++;

     e = INT_MAX >> s;   /* e is for overflow checking.*/

     /*同时完成两个任务：计算start_chunk和max_chunk,它们相互独立*/
     for (start_chunk = max_chunk = chunk_size = 0; chunk_size < n && max_chunk <= e && (chunk = max_chunk << s | max_char) <= q; chunk_size++) {
          start_chunk = start_chunk << s | rank[chunk_size];  /* b is start of x in chunk alphabet.(min symbol)*/
          max_chunk = chunk;   /* d is max symbol in chunk alphabet.*/
     }
    
     mask = (1 << (chunk_size - 1) * s) - 1;   /* m masks off top old symbol from chunk.*/
     rank[n] = 0;  /* emulate zero terminator.*/

     if (max_chunk <= n) {   /* if bucketing possible, compact alphabet.*/
          for (i = 0; i <= max_chunk; i++) /* zero transformation table.*/
               sa[i] = 0;

          for (i = chunk_size, chunk = start_chunk; i <= n; i++) {
               sa[chunk] = 1;  /* mark used chunk symbol.*/
               chunk = (chunk & mask) << s | rank[i];  /* shift in next old symbol in chunk.*/
          }
          for (i = 1; i < chunk_size; i++) {  /* handle last r-1 positions., i=n 时不需要处理*/
               sa[chunk] = 1;   /* mark used chunk symbol.*/
               chunk = (chunk & mask) << s;   /* shift in next old symbol in chunk.*/
          }

          for (i = 0, incr = 1; i <= max_chunk; i++)
               if (sa[i])
                    sa[i] = incr++;  /* j is new alphabet size	.*/
	 	
          for (i = 0, j = chunk_size, chunk = start_chunk; j <= n; i++, j++) {
               rank[i] = sa[chunk];
               chunk = (chunk & mask) << s | rank[j];  /* shift in next old symbol in chunk.*/
          }
          while (i < n) {  /* handle last r-1 positions.*/
               rank[i++] = sa[chunk];  /* transform to new alphabet.*/
               chunk = (chunk & mask) << s;   /* shift right-end zero in chunk.*/
          }
     } else {  /* bucketing not possible, don't compact.*/
          for (i = 0, j = chunk_size, chunk = start_chunk; j <= n; i++, j++) {
               rank[i] = chunk;
               chunk = (chunk & mask) << s | rank[j];  /* shift in next old symbol in chunk.*/
          }
	
          while (i < n) {  /* handle last r-1 positions.*/
               rank[i++] = chunk;  /* transform to new alphabet.*/
               chunk = (chunk & mask) << s;   /* shift right-end zero in chunk.*/
          }

          incr = max_chunk++;    /* new alphabet size.*/
     }

     return incr; /* return new alphabet size. 包括0*/
}

void qsufsort(unsigned char *text, int text_len, int max_char)
{
     int group_head, group_end;
     int i, new_alphabet, v, sorted_len, bucket_lcp;

     rank = MALLOC("rank", text_len, int);  /* set global values.*/
     sa = MALLOC("sa", text_len, int);
    
#if (VERSION == 1)
     lcp = MALLOC("lcp", text_len, int);
#endif

     for (i = 0; i < text_len; i++)
          rank[i] = text[i];

     new_alphabet = transform(text_len, max_char, text_len);
     bucketsort(text_len, new_alphabet);   /* bucketsort on first r positions.*/

#if (VERSION == 0)
     bucket_lcp = chunk_size;
#endif

     while (sa[0] >= -text_len) {
          loops++;
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

#if (VERSION == 1)           
                    bucket_lcp = lcp[rank[v]];
#endif
                    sort_split(group_head, group_end = rank[v], bucket_lcp);
                    group_head = group_end + 1;  /* next group.*/
               }
          } while (group_head <= text_len);
          /* if the array ends with a sorted group.*/
          if (sorted_len)
               sa[group_head+sorted_len] = sorted_len; /* combine sorted groups at end of sa.*/
#if (VERSION == 0)
          bucket_lcp *= 2;
#endif
     }

     for (i = 0; i <= text_len; i++)   /* reconstruct suffix array from inverse.*/
          sa[rank[i]] = i;

#if(CHECK == 1)
     sa_checker_1(sa, rank, text,text_len);
#endif

     free(sa);free(rank);
#if (VERSION == 1) 
     free(lcp);
#endif

    
#if  (SHOW_RESULTS == 1)
     for (i = 0; i <= text_len; i++)   /* 在屏幕上显示排序结果*/
          printf("%d ",sa[i]);
#endif
}
