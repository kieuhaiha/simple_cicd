C_FLags=-Wall -Werror -Wextra -std=c11

all: build

build: s21_grep test clean

s21_grep: s21_grep.c
	gcc $(C_FLags) s21_grep.c -o s21_grep

test: s21_grep
	chmod 777 test_grep.sh
	bash test_grep.sh

style:
	clang-format -style=Google -n *.c
	clang-format -style=Google -n *.c
clean:
	rm -rf s21_grep
