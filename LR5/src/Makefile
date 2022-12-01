all: main

main: main.o SuffArray.o
	g++ -g -pg -std=c++14 -w -O0 -Wextra -Wall -Werror -Wno-sign-compare -Wno-unused-result -pedantic -lm  main.o SuffArray.o -o main

SuffArray.o: SuffArray.cpp
	g++ -g -pg -std=c++14 -w -O0 -Wextra -Wall -Werror -Wno-sign-compare -Wno-unused-result -pedantic -lm -c SuffArray.cpp

main.o: main.cpp
	g++ -g -pg -std=c++14 -w -O0 -Wextra -Wall -Werror -Wno-sign-compare -Wno-unused-result -pedantic -lm -c main.cpp

run: main
	./main

run_test: main
	./main < 24

run_gdb: main
	gdb ./main

run_valgrind: main
	valgrind ./main

run_valgrind_memory: main
	valgrind --leak-check=full ./main

pack:
	zip solution.zip Makefile *pp *h

clean:
	rm -rf *.o

clean_all:
	rm -rf *.o main *.bin *.zip