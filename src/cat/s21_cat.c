#include "s21_cat.h"

int main(int argc, char **argv) {
  flags flag = {0};
  if (argc > 1) {
    flag_processing(argc, argv, &flag);
    open_file(argc, argv, flag);
  } else {
    while (1) {
      int c = getchar();
      printf("%c", c);
    }
  }
  return 0;
}

void flag_processing(int argc, char **argv, flags *flag) {
  int result = 0;
  const struct option long_options[] = {{"number-nonblank", 0, &flag->b, 1},
                                        {"number", 0, &flag->n, 1},
                                        {"squeeze-blank", 0, &flag->s, 1},
                                        {NULL, 0, NULL, 0}};
  while ((result = getopt_long(argc, argv, "+benstvET", long_options, NULL)) !=
         -1) {
    switch (result) {
      case 'b':
        flag->b = 1;
        break;
      case 'e':
        flag->v = 1;
        flag->e = 1;
        break;
      case 'E':
        flag->e = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 't':
        flag->v = 1;
        flag->t = 1;
        break;
      case 'T':
        flag->t = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      default:
        fprintf(stderr, "usage: cat [-benstuv] [file ...]\n");
    }
  }
  if (flag->b) flag->n = 0;
}

void open_file(int argc, char **argv, flags flag) {
  FILE *file = NULL;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      if ((file = fopen(argv[i], "r")) != NULL) {
        fseek(file, 0, SEEK_SET);
        output_file(file, flag);
        fclose(file);
      } else {
        fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
      }
    }
  }
}

void output_file(FILE *file, flags flag) {
  int number = 0, new_line = 1;
  char pr_ch = 0, ch = 0;
  while ((ch = fgetc(file)) != EOF) {
    if (flag.s) {
      if (pr_ch == '\n' && ch == '\n') {
        ch = fgetc(file);
        if (ch != '\n') {
          if (flag.n) printf("%6d\t", ++number);
          flag.e ? printf("$\n") : printf("\n");
        } else {
          continue;
        }
      }
      pr_ch = ch;
    }
    if (flag.n) {
      flag_n(&ch, &new_line, &number);
    }
    if (flag.b) {
      flag_b(&ch, &new_line, &number);
    }
    if (flag.e) {
      flag_e(&ch);
    }
    if (flag.t) {
      if (ch == '\t') {
        printf("^I");
        continue;
      }
    }
    if (flag.v) {
      flag_v(&ch);
    }
    printf("%c", ch);
  }
}

void flag_n(char *ch, int *new_line, int *number) {
  if (*new_line) {
    printf("%6d\t", ++(*number));
    *new_line = 0;
  }
  if (*ch == '\n') *new_line = 1;
}

void flag_b(char *ch, int *new_line, int *number) {
  if (*new_line && *ch != '\n') {
    printf("%6d\t", ++(*number));
    *new_line = 0;
  }
  if (*ch == '\n') *new_line = 1;
}

void flag_e(char *ch) {
  if (*ch == '\n') printf("$");
}

void flag_v(char *ch) {
  if (*ch <= 31 && *ch != 10 && *ch != 9) {
    printf("^");
    *ch += 64;
  }
  if (*ch == 127) {
    printf("^");
    *ch -= 64;
  }
}
