CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LDFLAGS = -lcheck

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    LDFLAGS += -lm -lrt -lsubunit -lpthread
endif

TARGET = s21_matrix.a

# Находим все файлы функций, исключая tests.c
SRC = $(filter-out tests.c, $(wildcard s21_*.c))
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)
	ranlib $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	$(CC) $(CFLAGS) tests.c $(TARGET) $(LDFLAGS) -o test_exec
	./test_exec

gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC) tests.c $(LDFLAGS) -o gcov_exec
	./gcov_exec
	lcov -t "gcov_report" -o gcov_report.info -c -d . --no-external
	lcov --remove gcov_report.info '*/tests.c' -o gcov_report.info
	genhtml -o gcov_report gcov_report.info
	open gcov_report/index.html || xdg-open gcov_report/index.html

valgrind: test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test_exec

clean:
	rm -rf *.o *.a *.gcda *.gcno *.info test_exec gcov_exec gcov_report

rebuild: clean all

.PHONY: all clean test gcov_report rebuild