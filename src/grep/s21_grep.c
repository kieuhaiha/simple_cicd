#include "s21_grep.h"

int main(int argc, char** argv) {
  flags flag = {0};
  char pattern[BUFF] = {0};
  if (argc > 1) {
    flag_processing(argc, argv, pattern, &flag);
    open_file(argc, argv, pattern, flag);
  }
  return 0;
}

void flag_processing(int argc, char** argv, char* pattern, flags* flag) {
  int count = 0, result = 0, file_in;
  while ((result = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (result) {
      case 'e':
        flag->e = 1;
        flag_e(&count, pattern, optarg, flag);
        break;
      case 'i':
        flag->i = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'f':
        flag->f = 1;
        flag_f(&count, pattern, optarg, flag);
        break;
      case 'o':
        flag->o = 1;
        break;
    }
  }
  file_in = find_file(argc, argv, *flag);
  if ((file_in < argc - 1) && !(flag->h)) {
    flag->files = 1;
  }
  if (!(flag->e) && !(flag->f)) {
    if (file_in > 0 && argv[file_in - 1] != NULL) {
      if (argv[file_in - 1][0] != '\0') {
        strcat(pattern, argv[file_in - 1]);
      } else {
        strcat(pattern, ".");
      }
    }
  }
  if (flag->empty) {
    flag->o = 0;
  }
}

void open_file(int argc, char** argv, char* pattern, flags flag) {
  FILE* file = NULL;
  for (int i = find_file(argc, argv, flag); i < argc; ++i) {
    if ((file = fopen(argv[i], "r")) != NULL) {
      fseek(file, 0, SEEK_SET);
      find_line(argv, file, pattern, i, flag);
      fclose(file);
    } else if (!(flag.s)) {
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
    }
  }
}

void find_line(char** argv, FILE* file, char* pattern, int file_index,
               flags flag) {
  char line[N] = {0};
  int count_lines = 0, match_lines = 0;
  int cflags = REG_EXTENDED;
  regex_t compiled;
  regmatch_t pm[1];
  if (flag.i) cflags |= REG_ICASE;
  regcomp(&compiled, pattern, cflags);
  while (fgets(line, N, file) != NULL) {
    ++count_lines;
    if (flag.o && !flag.v) {
      flag_o(line, &compiled, pm, &match_lines, argv, file_index, flag,
             count_lines);
    } else {
      flag_v_or_without_flag_o(line, &compiled, pm, &match_lines, argv,
                               file_index, flag, count_lines);
    }
  }
  regfree(&compiled);
  if (flag.c) {
    flag_c(argv, &file_index, flag, &match_lines);
  }
  if (flag.l && match_lines) {
    printf("%s\n", argv[file_index]);
  }
}

void flag_o(char* line, regex_t* compiled, regmatch_t* pm, int* match_lines,
            char** argv, int file_index, flags flag, int count_lines) {
  int status = regexec(compiled, line, 1, pm, 0);
  if (status == REG_NOMATCH) return;
  ++(*match_lines);
  char* pattern = line;
  if (!flag.c && !flag.l) {
    if (flag.files && !flag.h) {
      printf("%s:", argv[file_index]);
    }
    if (flag.n) {
      printf("%d:", count_lines);
    }
  }
  while (status != REG_NOMATCH) {
    if (pm->rm_so != pm->rm_eo && !flag.c && !flag.l) {
      printf("%.*s\n", (int)(pm->rm_eo - pm->rm_so), &pattern[pm->rm_so]);
    }
    pattern = &pattern[pm->rm_eo];
    status = regexec(compiled, pattern, 1, pm, REG_NOTBOL);
  }
}

void flag_v_or_without_flag_o(char* line, regex_t* compiled, regmatch_t* pm,
                              int* match_lines, char** argv, int file_index,
                              flags flag, int count_lines) {
  int status = regexec(compiled, line, 1, pm, 0);
  if ((flag.v && status != REG_NOMATCH) || (!flag.v && status == REG_NOMATCH)) {
    return;
  }
  ++(*match_lines);
  if (line[strlen(line) - 1] == '\n') {
    line[strlen(line) - 1] = 0;
  }
  if (!flag.c && !flag.l) {
    print_info(flag, argv[file_index], count_lines);
    printf("%s\n", line);
  }
}

void print_info(flags flag, char* filename, int count_lines) {
  if (flag.files) {
    printf("%s:", filename);
  }
  if (flag.n) {
    printf("%d:", count_lines);
  }
}

int find_file(int argc, char** argv, flags flag) {
  char *pttrn_e = NULL, *pttrn_f = NULL;
  int i = 1;
  while (i < argc) {
    if (argv[i][0] == '-') {
      process_ef_flags(argv[i], &pttrn_e, &pttrn_f, &i);
      ++i;
    } else {
      break;
    }
  }
  return (flag.e || flag.f) ? i : i + 1;
}

void process_ef_flags(char* arg, char** pattern_e, char** pattern_f,
                      int* index) {
  *pattern_e = strchr(arg, 'e');
  *pattern_f = strchr(arg, 'f');
  if (*pattern_e && *pattern_f) {
    char** first_pattern = (*pattern_e > *pattern_f) ? pattern_f : pattern_e;
    increment_pattern_and_index_if_end(first_pattern, index);
  } else if (*pattern_e) {
    increment_pattern_and_index_if_end(pattern_e, index);
  } else if (*pattern_f) {
    increment_pattern_and_index_if_end(pattern_f, index);
  }
}

void increment_pattern_and_index_if_end(char** pattern, int* index) {
  ++(*pattern);
  if (**pattern == '\0') ++(*index);
}

void flag_e(int* count, char* pattern, char* optarg, flags* flag) {
  if (*count > 0) strcat(pattern, "|");
  *count = *count + 1;
  if (*optarg == '\0') {
    flag->empty = 1;
    strcat(pattern, ".");
  } else {
    strcat(pattern, optarg);
  }
}

void flag_f(int* count, char* pattern, char* optarg, flags* flag) {
  FILE* file = fopen(optarg, "r");
  if (!file) {
    fprintf(stderr, "grep: %s: No such file or directory\n", optarg);
    return;
  }
  char line[N] = {0};
  while (fgets(line, N, file)) {
    if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
    if (*count > 0) strcat(pattern, "|");
    if (*line == '\0') {
      flag->empty = 1;
      strcat(pattern, ".");
    } else {
      strcat(pattern, line);
    }
    (*count)++;
  }
  fclose(file);
}

void flag_c(char** argv, int* file_index, flags flag, int* match_lines) {
  if (flag.files) {
    printf("%s:", argv[*file_index]);
  }
  if (flag.l && *match_lines) {
    printf("1\n");
  } else {
    printf("%d\n", *match_lines);
  }
}
