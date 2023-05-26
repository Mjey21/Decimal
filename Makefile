STD_FLAGS = -g -Wall -Wextra -Werror -std=c11
COMPILER = gcc
COV_FLAGS = --coverage

TEST_FLAGS = -lcheck -lpthread -lm

SOURCES_0 = s21_*.o
SOURCES_1 = s21_decimal_funcs/*.c
SOURCES_2 = s21_test_decimal.c
SOURCES_3 = s21_decimal.a

ifeq ($(shell uname), Linux)
	TEST_FLAGS += -lrt -lsubunit
endif

all: s21_decimal.a test gcov_report

t:
	gcc -g t.c s21_decimal_funcs/*.c -o t

s21_test_decimal.o: s21_test_decimal.c
	$(COMPILER) $(STD_FLAGS) -c  s21_test_decimal.c

test_case:
	$(COMPILER) $(STD_FLAGS) -c test_cases/*.c

s21_decimal.a:
	$(COMPILER) -c $(SOURCES_1)
	ar rcs $(SOURCES_3) $(SOURCES_0)

test: s21_decimal_with_coverage s21_test_decimal.o  test_case
	$(COMPILER) -o tests $(STD_FLAGS) *.o -L. $(SOURCES_3) $(COV_FLAGS) $(TEST_FLAGS)
	./tests

s21_decimal_with_coverage:
	$(COMPILER) $(STD_FLAGS) -c $(SOURCES_1) $(COV_FLAGS)
	ar rcs $(SOURCES_3) $(SOURCES_0)

gcov_report:
	./tests > /dev/null
	lcov -t "report" -o report.info -c -d .
	genhtml -o "html_report" report.info
	open html_report/index.html

leak:
	leaks -atExit -- ./tests

clean:
	@rm -rf *.html *.gcno *.gcda *.o *.out *.css *coverage* *.a html_report

style_check:
	cp ../materials/linters/.clang-format .
	clang-format -n test_cases/*.c *.h *.c
	clang-format -n s21_decimal_funcs/*.c
	rm -f .clang-format

style_correct: style_check
	cp ../materials/linters/.clang-format .
	clang-format -i test_cases/*.c *.h *.c
	clang-format -i s21_decimal_funcs/*.c
	rm -f .clang-format

