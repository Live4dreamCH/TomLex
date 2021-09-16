.PHONY: all clean test

cppc = g++ -std=c++11 -ggdb -Wall -Werror -Wextra

all: clean test main.out

main.out: main.o
	$(cppc) main.o -o main.out
main.o: main.cpp
	$(cppc) -c main.cpp -o main.o

test: Regex_test.out DFA_test.out NFA_test.out

Regex_test.out: Regex.o Regex_test.cpp
	$(cppc) Regex.o Regex_test.cpp -o Regex_test.out
Regex.o: Regex.cpp
	$(cppc) -c Regex.cpp -o Regex.o

NFA_test.out: NFA_test.cpp NFA.o DFA.o
	$(cppc) NFA_test.cpp NFA.o DFA.o -o NFA_test.out
NFA.o: NFA.cpp
	$(cppc) -c NFA.cpp -o NFA.o

DFA_test.out: DFA.o DFA_test.cpp
	$(cppc) DFA.o DFA_test.cpp -o DFA_test.out
DFA.o: DFA.cpp
	$(cppc) -c DFA.cpp -o DFA.o

clean:
	-rm *.out *.o