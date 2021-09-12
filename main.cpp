#include <iostream>
#include <string>
#include <unordered_set>
using std::string;
using std::unordered_set;
using state = int;

int func();

int func();

int func() {
    return 1;
}

// int func(){
//     return 0;
// }

class Obj;

class Obj;

class Obj {
public:
    int a;
};

// class Obj{
// public:
//     int a;
// };

class Init;

class Init {
    int b;

public:
    Init(int _b) : b(_b) {}
};

// class Init{
//     int b;
// public:
//     Init(int _b):b(_b){}
// };

int main() {
    func();
    Obj o1;
    o1.a = 42;
    ++o1.a;
    Init i1(3);

    string str;
    char chs[] = "";
    // char e=''; // 带引号的字符串应至少包含一个字符C/C++(25)
    char zero = '\0';
    auto res1 = (str == &zero); // true
    auto res2 = (chs == &zero); // false
    std::cout << res1 << '\n';
    std::cout << res2 << '\n';

    unordered_set<state> set_a{1, 3, 5, 7, 9};
    unordered_set<state> set_b;
    set_b.emplace(5);
    set_b.emplace(9);
    set_b.emplace(7);
    set_b.emplace(1);
    set_b.emplace(3);
    unordered_set<unordered_set<state>> set_set{{9, 7, 5, 3, 1}}; // need key_equal and hash
    auto res3 = (set_set.find(set_a) == set_set.end());
    auto res4 = (set_set.find(set_b) == set_set.end());
    std::cout << res3 << '\n';
    std::cout << res4 << '\n';
    return 0;
}