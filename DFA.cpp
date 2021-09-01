#include "DFA.h"
#include <iostream>
using std::string;
using std::to_string;

int Tom::DFA::check(string &err_msg) const {
    // 初态不在状态集中
    if (states.find(start) == states.end()) {
        err_msg = "start state " + to_string(start) + " not in state set!";
        return -1;
    }
    // 终态不在状态集中
    for (const auto &s : final_states) {
        if (states.find(s) == states.end()) {
            err_msg = "final state " + to_string(s) + " not in state set!";
            return -2;
        }
    }
    // 转移表
    for (const auto &it_from : trans) {
        // 源状态不在状态集中
        const auto &from = it_from.first;
        if (states.find(from) == states.end()) {
            err_msg = "from state " + to_string(from) + " not in state set!";
            return -3;
        }
        // 源状态的出边
        for (const auto &edge : it_from.second) {
            const auto &ch = edge.first;
            const auto &to = edge.second;
            // 字符不在字符集中
            if (use_alphabet && alphabet.find(ch) == alphabet.end()) {
                err_msg = string("char ") + ch + " not in alphabet! " + "edge: from=" + to_string(from) + ", ch=" + ch + ", to=" + to_string(to);
                return -4;
            }
            // 目标状态不在状态集中
            if (states.find(to) == states.end()) {
                err_msg = string("to state ") + to_string(to) + " not in state set! " + "edge: from=" + to_string(from) + ", ch=" + ch + ", to=" + to_string(to);
                return -5;
            }
        }
    }
    return 0;
}

Tom::state Tom::DFA::func(const state &from, const char &ch, string &err_msg) const {
    // 输入字符不在字符集中
    if (use_alphabet && alphabet.find(ch) == alphabet.end()) {
        err_msg = string("a char: ") + ch + " not in alphabet, current state: " + to_string(from);
        return -1;
    }

    // 源状态不在转移表中
    auto it_from = trans.find(from);
    if (it_from == trans.end()) {
        err_msg = string("current state: ") + to_string(from) + " not in trans table";
        return -2;
    }

    // 源状态的出边中无此字符
    auto it_edge = it_from->second.find(ch);
    if (it_edge == it_from->second.end()) {
        err_msg = string("current state: ") + to_string(from) + " doesn't have an edge with " + ch;
        return -3;
    }
    return it_edge->second;
}

bool Tom::DFA::accept(const string &str, state &final_state, string &err_msg) const {
    state curr = start;
    // 每字符转移一次
    for (const auto &ch : str) {
        curr = func(curr, ch, err_msg);
        if (curr < 0) {
            final_state = curr;
            return false;
        }
    }

    // 是否落在终态集里
    if (final_states.find(curr) == final_states.end()) {
        final_state = -4;
        err_msg = string("stop at state: ") + to_string(curr) + ", not a final state";
        return false;
    }

    final_state = curr;
    return true;
}

string Tom::DFA::show() const {
    // string res{"start: "+to_string(s0)+"\n"};
    string res{"states = {"};
    if (!states.empty()) {
        res += to_string(*states.begin());
        for (auto it = ++states.begin(); it != states.end(); ++it) {
            res += ", " + to_string(*it);
        }
    }
    res += "},\nstart = " + to_string(start) + ", finals = {";
    if (!final_states.empty()) {
        res += to_string(*final_states.begin());
        for (auto it = ++final_states.begin(); it != final_states.end(); ++it) {
            res += ", " + to_string(*it);
        }
    }
    res += "},\nalphabet = ";
    if (use_alphabet) {
        res += '{';
        if (!alphabet.empty()) {
            auto it = alphabet.begin();
            res += *it;
            ++it;
            for (; it != alphabet.end(); ++it) {
                res += ", ";
                res += *it;
            }
        }
        res += "}\n";
    } else {
        res += "all characters\n";
    }
    res += "transition table:\n";
    for (const auto &it_from : trans) {
        const auto &from = it_from.first;
        for (const auto &edge : it_from.second) {
            const auto &ch = edge.first;
            const auto &to = edge.second;
            res += "    " + to_string(from) + " ->" + ch + "-> " + to_string(to) + '\n';
        }
    }
    return res;
}