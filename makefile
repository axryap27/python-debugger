build:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall debugger.cpp main.cpp nupython_e.o -lm -Wno-unused-variable -Wno-unused-function

run:
	./a.out

valgrind:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall debugger.cpp main.cpp nupython_e.o -lm -Wno-unused-variable -Wno-unused-function
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./a.out "$(file)"

submit:
	/home/cs211/s2025/tools/project04  submit  debugger.*

commit:
	git add .
	git commit -m "$(msg)"

push:
	git push origin main

nupython:
	rm -f main.o execute.o parser.o programgraph.o scanner.o tokenqueue.o ram.o nupython_e.o
	g++ -std=c++17 -c -w -Wall execute.cpp
	g++ -std=c++17 -c -w -Wall parser.cpp
	g++ -std=c++17 -c -w -Wall programgraph.cpp
	g++ -std=c++17 -c -w -Wall ram.cpp
	g++ -std=c++17 -c -w -Wall scanner.cpp
	g++ -std=c++17 -c -w -Wall tokenqueue.cpp
	g++ -std=c++17 -c -w -Wall main.cpp
	ld -relocatable execute.o parser.o programgraph.o ram.o scanner.o tokenqueue.o -o nupython_e.o
	rm -f execute.o parser.o programgraph.o ram.o scanner.o tokenqueue.o
