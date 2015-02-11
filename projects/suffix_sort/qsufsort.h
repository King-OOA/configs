#ifndef QSUFSORT_H
#define QSUFSORT_H


#define RESULTS_FILE "/home/pz/projects/suffix_sort/results"
#define TEXT_FILES_NAMES "/home/pz/projects/suffix_sort/file_names"
#define ROUNDS 1
#define SHOW_RESULTS 0
#define VERSION 1

#define CHECK 0


void qsufsort(unsigned char *, int, int);
void sa_checker(int *, unsigned char *, int);
void sa_checker_1(int *, int *, unsigned char *, int);


#endif
