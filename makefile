.PHONY: all clean test

test: Regex_test.out DFA_test.out

Regex_test.out: Regex.h Regex.cpp Regex_test.cpp
	g++ -std=c++11 -ggdb Regex.h Regex.cpp Regex_test.cpp -o Regex_test.out

DFA_test.out: DFA.h DFA.cpp DFA_test.cpp
	g++ -std=c++11 -ggdb DFA.h DFA.cpp DFA_test.cpp -o DFA_test.out

clean:
	rm *.out