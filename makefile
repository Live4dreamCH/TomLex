.PHONY: all clean test

cppc = g++ -std=c++11 -ggdb -Wall -Werror -Wextra

all: clean test main.out

main.out: main.obj
	$(cppc) main.obj -o main.out
main.obj: main.cpp
	$(cppc) -c main.cpp -o main.obj

test: Regex_test.out DFA_test.out NFA_test.out

Regex_test.out: Regex.obj Regex_test.cpp
	$(cppc) Regex.obj Regex_test.cpp -o Regex_test.out
Regex.obj: Regex.cpp
	$(cppc) -c Regex.cpp -o Regex.obj

DFA_test.out: DFA.obj DFA_test.cpp
	$(cppc) DFA.obj DFA_test.cpp -o DFA_test.out
DFA.obj: DFA.cpp
	$(cppc) -c DFA.cpp -o DFA.obj

NFA_test.out: NFA.obj NFA_test.cpp
	$(cppc) NFA_test.cpp NFA.obj -o NFA_test.out
NFA.obj: NFA.cpp DFA.obj
	$(cppc) -c NFA.cpp DFA.obj -o NFA.obj

clean:
	-rm *.out *.obj