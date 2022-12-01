all: main

main: main.o patricia.o base.o
	g++ -g -std=c++14 -w -O0 -Wextra -Wall -Werror -Wno-sign-compare -Wno-unused-result -pedantic -lm main.o patricia.o base.o -o main

patricia.o: patricia.cpp
	g++ -g -std=c++14 -w -O0 -Wextra -Wall -Werror -Wno-sign-compare -Wno-unused-result -pedantic -lm -c patricia.cpp

base.o: base.cpp
	g++ -g -std=c++14 -w -O0 -Wextra -Wall -Werror -Wno-sign-compare -Wno-unused-result -pedantic -lm -c base.cpp

main.o: main.cpp
	g++ -g -std=c++14 -w -O0 -Wextra -Wall -Werror -Wno-sign-compare -Wno-unused-result -pedantic -lm -c main.cpp

run: main
	./main

run_gdb: main
	gdb ./main

run_valgrind: main
	valgrind ./main

run_valgrind_memory: main
	valgrind --leak-check=full ./main

pack:
	zip solution.zip Makefile *pp

clean:
	rm -rf *.o

clean_all:
	rm -rf *.o main *.bin *.zip