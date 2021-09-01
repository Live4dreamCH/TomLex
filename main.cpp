#include <string>
#include <iostream>
using std::string;

int func();

int func();

int func(){
    return 1;
}

// int func(){
//     return 0;
// }

class Obj;

class Obj;

class Obj{
public:
    int a;
};

// class Obj{
// public:
//     int a;
// };

class Init;

class Init{
    int b;
public:
    Init(int _b):b(_b){}
};

// class Init{
//     int b;
// public:
//     Init(int _b):b(_b){}
// };

int main(){
    func();
    Obj o1;
    o1.a=42;
    ++o1.a;
    Init i1(3);
    
    string str;
    char chs[]="";
    // char e=''; // 带引号的字符串应至少包含一个字符C/C++(25)
    char zero='\0';
    auto res1 = (str == &zero); // true
    auto res2 = (chs == &zero); // false
    std::cout<< res1;
    std::cout<< res2;
    return 0;
}