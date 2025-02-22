C_FLags=-Wall -Werror -Wextra -std=c11

all: build

build: s21_cat test clean

s21_cat: s21_cat.c
	gcc $(C_FLags) s21_cat.c -o s21_cat

test: s21_cat
	chmod 777 test_cat.sh
	bash test_cat.sh

style:
	clang-format -style=Google -n *.c
	clang-format -style=Google -n *.h

clean:
	rm -rf s21_cat