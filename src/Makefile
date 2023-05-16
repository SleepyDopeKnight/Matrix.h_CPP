.PHONY: all clean check rebuild
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++17
OS = $(shell uname -s)

all: s21_matrix_oop.a test gcov_report check

s21_matrix_oop.a: s21_matrix_oop.cc s21_matrix_oop.h
	$(CXX) $(CXXFLAGS) s21_matrix_oop.cc -c
	ar rcs s21_matrix_oop.a s21_matrix_oop.o
	ranlib s21_matrix_oop.a

test: s21_matrix_oop_tests.cc s21_matrix_oop.a
ifeq ($(OS), Darwin)
	$(CXX) $(CXXFLAGS) s21_matrix_oop_tests.cc s21_matrix_oop.a -o test.out -lgtest
else
	$(CXX) $(CXXFLAGS) s21_matrix_oop_tests.cc s21_matrix_oop.a -o test.out -lgtest -lpthread
endif
	./test.out

gcov_report:
ifeq ($(OS), Darwin)
	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage s21_matrix_oop_tests.cc s21_matrix_oop.cc -o test.out -lgtest
else
	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage s21_matrix_oop_tests.cc s21_matrix_oop.cc -o test.out -lgtest -lpthread
endif
	./test.out
	lcov -t "test" -o test.info --no-external -c -d .
	genhtml -o report test.info
ifeq ($(OS), Darwin)
	open ./report/index.html
else
	xdg-open ./report/index.html
endif

clean:
	rm *.out
	rm *.gcda
	rm *.gcno
	rm *.a
	rm *.info
	rm *.o
	rm -rf report

check:
	clang-format -style=google -n *.cc *.h
	cppcheck *.cc
ifeq ($(OS), Darwin)
	CK_FORK=no leaks --atExit -- ./test.out
else
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./test.out
endif

rebuild: clean all
