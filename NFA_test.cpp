#include "NFA.h"
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using std::pair;
using std::string;
using std::unordered_set;
using std::vector;
using Tom::NFANei;
using Tom::state;

int main() {
    // 用例驱动测试
    // 1. 含aa或bb
    // 2. 至少一个a连接至少一个b
    // 3. 汉字
    // 4. 不识别任何字符串
    vector<unordered_set<state>> states_list{
        {0, 1, 2},
        {0, 1, 2},
        {0, 1, 2},
        // {1, 2, 3, 4},
        // {1},
        // {1},
    };
    vector<unordered_set<state>> finals_list{
        {1, 2},
        {2},
        {2},
        // {4},
        // {1},
        // {},
    };
    vector<pair<bool, unordered_set<char>>> ab_list{
        {true, {'a', 'b'}},
        {true, {'a', 'b'}},
        {false, {}},
        // {true,{'0', '1'}},
        // {false,{}},
    };
    vector<NFANei> nei_list{
        {
            {0, {
                    {"a", {0}},
                    {"b", {0}},
                    {"aa", {1}},
                    {"bb", {2}},
                }},
            {1, {
                    {"a", {1}},
                    {"b", {1}},
                }},
            {2, {
                    {"a", {2}},
                    {"b", {2}},
                }},
        },
        {
            {0, {
                    {"a", {0, 1}},
                }},
            {1, {
                    {"b", {1, 2}},
                }},
        },
        {
            {0, {
                    {"你", {1}},
                    {"我", {1}},
                    {"他", {1}},
                }},
            {1, {
                    {"在", {1}},
                    {"编程", {2}},
                    {"打游戏", {2}},
                }},
        },
        // {
        //     {1, {{'0', 2}, {'1', 3}}},
        //     {2, {{'0', 4}, {'1', 3}}},
        //     {3, {{'0', 2}, {'1', 3}}},
        //     {4, {{'0', 4}, {'1', 3}}},
        // },
        // {},
        // {},
    };
    vector<unordered_set<state>> start_list{
        {0},
        {0},
        {0},
        // 1,
        // 1,
        // 1,
    };
    vector<vector<pair<string, bool>>> strs_list{
        {
            {"", false},
            {" ", false},
            {"123", false},
            {"123aabb", false},
            {"a", false},
            {"b", false},
            {"ab", false},
            {"abababa", false},
            {"aa", true},
            {"aaa", true},
            {"bb", true},
            {"bbb", true},
            {"aabb", true},
            {"ababb", true},
            {"ababaa", true},
            {"abaaababab", true},
            {"ababbbabab", true},
        },
        {
            {"", false},
            {" ", false},
            {"123", false},
            {"123aabb", false},
            {"a", false},
            {"b", false},
            {"abababa", false},
            {"aa", false},
            {"bb", false},
            {"ababb", false},
            {"ababaa", false},
            {"abaaababab", false},
            {"ababbbabab", false},
            {"ab", true},
            {"aabb", true},
            {"abb", true},
            {"aab", true},
            {"ba", false},
            {"bbaa", false},
            {"baa", false},
            {"bba", false},
            {"aaabbbbb", true},
            {"abbbbb", true},
            {"bbbbb", false},
            {"aaaaa", false},
        },
        {
            {"", false},
            {" ", false},
            {"123", false},
            {"abc", false},
            {"953ccxp", false},
            {"行", false},
            {"我", false},
            {"你", false},
            {"他", false},
            {"打游戏", false},
            {"我在我打游戏", false},
            {"我在你打游戏", false},
            {"我在打游戏编程", false},
            {"我在编程打游戏", false},
            {"我编程", true},
            {"你编程", true},
            {"他编程", true},
            {"我在编程", true},
            {"你在编程", true},
            {"他在编程", true},
            {"我在在编程", true},
            {"我打游戏", true},
            {"你在打游戏", true},
            {"他在在打游戏", true},
        },
        // {
        //     {"",false},
        //     {" ",false},
        //     {"123",false},
        //     {"1230011",false},
        //     {"123001100",false},
        //     {"0",false},
        //     {"1",false},
        //     {"01",false},
        //     {"0101010",false},
        //     {"11",false},
        //     {"111",false},
        //     {"01011",false},
        //     {"0100010101",false},
        //     {"0101110101",false},
        //     {"00",true},
        //     {"000",true},
        //     {"1100",true},
        //     {"010100",true},
        // },
        // {
        //     {"",true},
        //     {" ",false},
        //     {"1",false},
        //     {"a",false},
        //     {"123",false},
        //     {"abc",false},
        // },
        // {
        //     {"",false},
        //     {" ",false},
        //     {"123",false},
        //     {"123aabb",false},
        //     {"a",false},
        //     {"b",false},
        //     {"ab",false},
        //     {"abababa",false},
        //     {"aa",false},
        //     {"aaa",false},
        //     {"bb",false},
        //     {"bbb",false},
        //     {"aabb",false},
        //     {"ababb",false},
        //     {"ababaa",false},
        //     {"abaaababab",false},
        //     {"ababbbabab",false},
        //     {"",false},
        //     {" ",false},
        //     {"123",false},
        //     {"1230011",false},
        //     {"123001100",false},
        //     {"0",false},
        //     {"1",false},
        //     {"01",false},
        //     {"0101010",false},
        //     {"11",false},
        //     {"111",false},
        //     {"01011",false},
        //     {"0100010101",false},
        //     {"0101110101",false},
        //     {"00",false},
        //     {"000",false},
        //     {"1100",false},
        //     {"010100",false},
        //     {"",false},
        //     {" ",false},
        //     {"1",false},
        //     {"a",false},
        //     {"123",false},
        //     {"abc",false},
        // },
    };
    auto loops = std::min({states_list.size(), finals_list.size(), ab_list.size(), nei_list.size(), start_list.size(), strs_list.size()});

    for (std::size_t i = 0; i < loops; ++i) {
        // 用例初始化NFA
        unordered_set<state> &S = states_list[i], F = finals_list[i];
        pair<bool, unordered_set<char>> &AB = ab_list[i];
        NFANei &nei = nei_list[i];
        unordered_set<state> &start = start_list[i];
        state final_state = 0;
        Tom::NFA *test;
        if (AB.first) {
            test = new Tom::NFA(S, AB.second, nei, start, F);
        } else {
            test = new Tom::NFA(S, nei, start, F);
        }

        // NFA合法性
        string err_msg;
        if (test->check(err_msg) != 0) {
            std::cout << err_msg << '\n';
            delete test;
            return -1;
        } else {
            std::cout << "NFA check success!\n";
        }

        // 打印NFA内容
        std::cout << "this NFA:\n"
                  << test->show() << '\n';

        // 测试
        const auto &strs = strs_list[i];
        for (const auto &str_res : strs) {
            err_msg.clear();
            const auto &str = str_res.first;
            const auto &res = str_res.second;
            if (res != test->accept(str, final_state, err_msg)) {
                std::cerr << str << " should have been " << (res == true ? "accepted" : "refused")
                          << " but actually " << (res == true ? "refused" : "accepted")
                          << ", err_code = " << final_state << " err_msg = " << err_msg << '\n';
                delete test;
                return -1;
            }
        }

        delete test;
        std::cout << '\n';
    }
    std::cout << "PASS!";
    return 0;
}