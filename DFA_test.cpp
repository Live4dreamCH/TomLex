#include "DFA.h"
#include <algorithm>
#include <iostream>
#include <vector>
using std::string;
using std::unordered_set;
using std::vector;
using Tom::NeiTable;
using Tom::state;

int main() {
    vector<unordered_set<state>> state_list{
        {0, 1, 2, 3},
        {1, 2, 3, 4},
    };
    vector<unordered_set<state>> final_list{
        {3},
        {4},
    };
    vector<unordered_set<char>> ab_list{
        {'a', 'b'},
        {'0', '1'},
    };
    vector<NeiTable> nei_list{
        {
            {0, {{'a', 1}, {'b', 2}}},
            {1, {{'a', 3}, {'b', 2}}},
            {2, {{'a', 1}, {'b', 3}}},
            {3, {{'a', 3}, {'b', 3}}},
        },
        {
            {1, {{'0', 2}, {'1', 3}}},
            {2, {{'0', 4}, {'1', 3}}},
            {3, {{'0', 2}, {'1', 3}}},
            {4, {{'0', 4}, {'1', 3}}},
        },
    };
    vector<state> start_list{
        0,
        1,
    };
    auto loops = std::min({state_list.size(), final_list.size(), ab_list.size(), nei_list.size(), start_list.size()});

    for (std::size_t i = 0; i < loops; ++i) {
        unordered_set<state> &S = state_list[i], F = final_list[i];
        unordered_set<char> &AB = ab_list[i];
        NeiTable &nei = nei_list[i];
        state &s0 = start_list[i], final_state = 0;
        Tom::DFA test(S, AB, nei, s0, F);

        string err_msg, str;
        if (test.check(err_msg) != 0) {
            std::cout << err_msg << '\n';
            return -1;
        } else {
            std::cout << "DFA check success!\n";
        }

        std::cout << "this DFA:\n"
                  << test.show() << '\n';
        while (std::cin >> str) {
            if (str == "\\n") {
                break;
            }
            bool res = test.accept(str, final_state, err_msg);
            if (res) {
                std::cout << str << " accepted! final state: " << final_state << '\n';
            } else {
                std::cout << str << " refused! err_code: " << final_state << " err_msg: " << err_msg << '\n';
            }
        }
        std::cout << '\n';
    }
    return 0;
}