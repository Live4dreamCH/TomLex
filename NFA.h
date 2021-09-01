#pragma once
#include "DFA.h"

namespace Tom {
    // NFA邻接表 源状态+字符串->目标状态集合
    using NFANei = unordered_map<state, unordered_map<string,unordered_set<state>>>;
    class NFA{
    private:
        unordered_set<state> states;
        bool use_alphabet;
        unordered_set<char> alphabet;
        NFANei trans;
        unordered_set<state> starts; // 非空
        unordered_set<state> final_states; // 可为空集

        // 状态转移一步
        // 为避免不必要的复制, 返回值为指针
        // 若出错, 返回nullptr
        const unordered_set<state>* func(const state &from, const string &str, string &err_msg) const;

    public:
        NFA()=default;

        // 四元组定义，字母表为全体字符
        NFA(unordered_set<state> &S, NFANei &f, unordered_set<state> &S0, unordered_set<state> &F)
            : states(S), use_alphabet(false), trans(f), starts(S0), final_states(F) {}

        // 五元组定义
        NFA(unordered_set<state> &S, unordered_set<char> &AB, NFANei &f, unordered_set<state> &S0, unordered_set<state> &F)
            : states(S), use_alphabet(true), alphabet(AB), trans(f), starts(S0), final_states(F) {}

        // 校验NFA合法性, 返回0则合法
        int check(string &err_msg) const;

        // NFA是否接受一个串
        // 由于NFA的不确定性, 最终可能到达多个终态
        // final_state只返回 "第一个找到的路径" 的终态
        bool accept(const string &str, state &final_state, string &err_msg) const;

        // 展示NFA结构
        string show() const;
    };
} // namespace Tom