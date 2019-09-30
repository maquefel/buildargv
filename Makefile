CC=gcc

CFLAGS += -ffunction-sections -fdata-sections -Os
CFLAGS+= -O0 -DDEBUG -ggdb -g3 -Wno-unused-variable
CFLAGS+= -fprofile-arcs -ftest-coverage
CFLAGS+= -fsanitize=address -fsanitize=leak

LDFLAGS += -lcheck -lrt -lpthread -lm -lsubunit

.PHONY: all
all: tests

test_cmdline:	test_cmdline.o buildargv.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_cmdline.o:	test_cmdline.c
	$(CC) $(CFLAGS) -c test_cmdline.c $(INCLUDE)

buildargv.o:	buildargv.c
	$(CC) $(CFLAGS) -c buildargv.c $(INCLUDE)

buildargv.c:	buildargv.rl
	ragel -e -L -F0 -o $@ $^

buildargv.png:	buildargv.rl
	ragel -V buildargv.rl -o buildargv.dot
	dot buildargv.dot -Tpng -o buildargv.png

clean::
	rm -rf buildargv.dot buildargv.png

tests: test_cmdline
	./test_cmdline

.PHONY: report

html:
	mkdir -p $@

report: html tests
	lcov -t "buildargv" -o buildargv.info -c -d .
	genhtml -o html buildargv.info

.PHONY: clean
clean::
	-rm test_cmdline *.o *.gcda *.gcno
