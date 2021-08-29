#include "DFA.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include<utility>
using std::pair;
using std::string;
using std::unordered_set;
using std::vector;
using Tom::NeiTable;
using Tom::state;

int main() {
    // 用例驱动测试
    // 1. 含aa或bb
    // 2. 以00结尾
    // 3. 空串ε
    // 4. 不识别任何字符串
    vector<unordered_set<state>> states_list{
        {0, 1, 2, 3},
        {1, 2, 3, 4},
        {1},
        {1},
    };
    vector<unordered_set<state>> finals_list{
        {3},
        {4},
        {1},
        {},
    };
    vector<pair<bool,unordered_set<char>>> ab_list{
        {true,{'a', 'b'}},
        {true,{'0', '1'}},
        {false,{}},
        {false,{}},
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
        {},
        {},
    };
    vector<state> start_list{
        0,
        1,
        1,
        1,
    };
    vector<vector<pair<string,bool>>> strs_list{
        {
            {"",false},
            {" ",false},
            {"123",false},
            {"123aabb",false},
            {"a",false},
            {"b",false},
            {"ab",false},
            {"abababa",false},
            {"aa",true},
            {"aaa",true},
            {"bb",true},
            {"bbb",true},
            {"aabb",true},
            {"ababb",true},
            {"ababaa",true},
            {"abaaababab",true},
            {"ababbbabab",true},
        },
        {
            {"",false},
            {" ",false},
            {"123",false},
            {"1230011",false},
            {"123001100",false},
            {"0",false},
            {"1",false},
            {"01",false},
            {"0101010",false},
            {"11",false},
            {"111",false},
            {"01011",false},
            {"0100010101",false},
            {"0101110101",false},
            {"00",true},
            {"000",true},
            {"1100",true},
            {"010100",true},
        },
        {
            {"",true},
            {" ",false},
            {"1",false},
            {"a",false},
            {"123",false},
            {"abc",false},
        },
        {
            {"",false},
            {" ",false},
            {"123",false},
            {"123aabb",false},
            {"a",false},
            {"b",false},
            {"ab",false},
            {"abababa",false},
            {"aa",false},
            {"aaa",false},
            {"bb",false},
            {"bbb",false},
            {"aabb",false},
            {"ababb",false},
            {"ababaa",false},
            {"abaaababab",false},
            {"ababbbabab",false},
            {"",false},
            {" ",false},
            {"123",false},
            {"1230011",false},
            {"123001100",false},
            {"0",false},
            {"1",false},
            {"01",false},
            {"0101010",false},
            {"11",false},
            {"111",false},
            {"01011",false},
            {"0100010101",false},
            {"0101110101",false},
            {"00",false},
            {"000",false},
            {"1100",false},
            {"010100",false},
            {"",false},
            {" ",false},
            {"1",false},
            {"a",false},
            {"123",false},
            {"abc",false},
        },
    };
    auto loops = std::min({states_list.size(), finals_list.size(), ab_list.size(), nei_list.size(), start_list.size(),strs_list.size()});

    for (std::size_t i = 0; i < loops; ++i) {
        // 用例初始化DFA
        unordered_set<state> &S = states_list[i], F = finals_list[i];
        pair<bool,unordered_set<char>> &AB = ab_list[i];
        NeiTable &nei = nei_list[i];
        state &s0 = start_list[i], final_state = 0;
        Tom::DFA *test;
        if(AB.first){
            test = new Tom::DFA(S, AB.second, nei, s0, F);
        }else{
            test = new Tom::DFA(S, nei, s0, F);
        }

        // DFA合法性
        string err_msg, str;
        if (test->check(err_msg) != 0) {
            std::cout << err_msg << '\n';
            delete test;
            return -1;
        } else {
            std::cout << "DFA check success!\n";
        }

        // 打印DFA内容
        std::cout << "this DFA:\n"
                  << test->show() << '\n';

        // 测试
        const auto& strs =strs_list[i];
        for(const auto & str_res:strs){
            if(str_res.second!=test->accept(str_res.first, final_state, err_msg)){
                std::cerr<<str<<" should have been "<<(str_res.second==true?"accepted":"refused")
                <<" but actually "<<(str_res.second==true?"refused":"accepted")
                <<", err_code = "<<final_state<<" err_msg = "<< err_msg << '\n';
                delete test;
                return -1;
            }
        }

        delete test;
        std::cout << '\n';
    }
    std::cout<<"PASS!";
    return 0;
}