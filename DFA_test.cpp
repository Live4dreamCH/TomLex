#include "DFA.h"
#include <iostream>
using std::string;
using std::unordered_set;
using Tom::state;

int main() {
    unordered_set<state> S{0, 1, 2, 3}, F{3};
    unordered_set<char> AB{'a', 'b'};
    Tom::NeiTable nei{
        {0, {{'a', 1}, {'b', 2}}},
        {1, {{'a', 3}, {'b', 2}}},
        {2, {{'a', 1}, {'b', 3}}},
        {3, {{'a', 3}, {'b', 3}}},
    };
    state s0 = 0, final_state = 0;
    Tom::DFA test(S, AB, nei, s0, F);

    string err_msg, str;
    while (std::cin >> str) {
        bool res = test.accept(str, final_state, err_msg);
        if (res) {
            std::cout << str << " accepted! final state: " << final_state << '\n';
        } else {
            std::cout << str << " refused! err_code: " << final_state << " err_msg: " << err_msg << '\n';
        }
    }
    return 0;
}