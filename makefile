.PHONY: all clean test

test: TomRegex_test.out TomDFA_test.out

TomRegex_test.out: TomRegex.h TomRegex.cpp TomRegex_test.cpp
	g++ -std=c++11 -ggdb TomRegex.h TomRegex.cpp TomRegex_test.cpp -o TomRegex_test.out

TomDFA_test.out: TomDFA.h TomDFA.cpp TomDFA_test.cpp
	g++ -std=c++11 -ggdb TomDFA.h TomDFA.cpp TomDFA_test.cpp -o TomDFA_test.out

clean:
	rm *.out