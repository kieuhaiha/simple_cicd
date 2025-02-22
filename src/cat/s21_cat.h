#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} flags;

void flag_processing(int argc, char** argv, flags* flag);
void open_file(int argc, char** argv, flags flag);
void output_file(FILE* fp, flags flag);
void flag_v(char* ch);
void flag_e(char* ch);
void flag_b(char* ch, int* new_line, int* number);
void flag_n(char* ch, int* new_line, int* number);

#endif  // S21_CAT_H
