#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF 1000
#define N 10000

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int files;
  int empty;
} flags;

void flag_processing(int argc, char** argv, char* pattern, flags* flag);
void open_file(int argc, char** argv, char* pattern, flags flag);
void find_line(char** argv, FILE* fp, char* pattern, int file_index,
               flags flag);
int find_file(int argc, char** argv, flags flag);
void flag_e(int* counter, char* pattern, char* optarg, flags* flag);
void flag_f(int* counter, char* pattern, char* optarg, flags* flag);
void flag_c(char** argv, int* file_index, flags flag, int* matching_lines);
void flag_o(char* line, regex_t* compiled, regmatch_t* pm, int* match_lines,
            char** argv, int file_index, flags flag, int count_lines);
void flag_v_or_without_flag_o(char* line, regex_t* compiled, regmatch_t* pm,
                              int* match_lines, char** argv, int file_index,
                              flags flag, int count_lines);
void process_ef_flags(char* arg, char** pttrn_e, char** pttrn_f, int* i);
void print_info(flags flag, char* filename, int count_lines);
void increment_pattern_and_index_if_end(char** pattern, int* index);
#endif  //  S21_GREP_H