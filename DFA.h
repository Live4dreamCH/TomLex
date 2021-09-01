#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Tom {
    using std::string;
    using std::unordered_map;
    using std::unordered_set;

    using state = int;
    // DFA邻接表 源状态+字符->目标状态
    using DFANei = unordered_map<state, unordered_map<char, state>>;

    class DFA {
    private:
        unordered_set<state> states;
        bool use_alphabet;
        unordered_set<char> alphabet;
        DFANei trans;
        state start;
        unordered_set<state> final_states; // 可为空

        // 状态转移一步
        state func(const state &from, const char &ch, string &err_msg) const;

    public:
        // 四元组定义，字母表为全体字符
        DFA(unordered_set<state> &S, DFANei &f, state &S0, unordered_set<state> &F)
            : states(S), use_alphabet(false), trans(f), start(S0), final_states(F) {}

        // 五元组定义
        DFA(unordered_set<state> &S, unordered_set<char> &AB, DFANei &f, state &S0, unordered_set<state> &F)
            : states(S), use_alphabet(true), alphabet(AB), trans(f), start(S0), final_states(F) {}

        // 校验DFA合法性, 返回0则合法
        int check(string &err_msg) const;

        // DFA是否接受一个串
        bool accept(const string &str, state &final_state, string &err_msg) const;

        // 展示DFA结构
        string show() const;
    };
} // namespace Tom
