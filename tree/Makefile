TARGET = a.out
CFLAGS = -g -O0 -fprofile-arcs -ftest-coverage

.PHONY: all clean

all: $(TARGET)

$(TARGET): tree.o tests.o
	gcc *.o -fprofile-arcs -ftest-coverage -o $(TARGET)

$@: $< tree.h
	$(cc) $(CFLAGS) $< -c -o $@

clean:
	rm -rf $(TARGET) *.o *.info *.gcda *.gcno *.gcov report

