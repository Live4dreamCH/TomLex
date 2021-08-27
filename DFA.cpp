#include "DFA.h"
#include <iostream>
using std::string;
using std::to_string;

int Tom::DFA::check(string &err_msg) const {
    if (states.find(s0) == states.end()) {
        err_msg = "start state " + to_string(s0) + " not in state set!";
        return -1;
    }
    for (const auto &s : final_states) {
        if (states.find(s) == states.end()) {
            err_msg = "final state " + to_string(s) + " not in state set!";
            return -2;
        }
    }
    for (const auto &it_from : trans) {
        const auto &from = it_from.first;
        if (states.find(from) == states.end()) {
            err_msg = "from state " + to_string(from) + " not in state set!";
            return -3;
        }
        for (const auto &edge : it_from.second) {
            const auto &ch = edge.first;
            const auto &to = edge.second;
            if (use_alphabet && alphabet.find(ch) == alphabet.end()) {
                err_msg = string("char ") + ch + " not in alphabet! " + "edge: from=" + to_string(from) + ", ch=" + ch + ", to=" + to_string(to);
                return -4;
            }
            if (states.find(to) == states.end()) {
                err_msg = string("to state ") + ch + " not in state set! " + "edge: from=" + to_string(from) + ", ch=" + ch + ", to=" + to_string(to);
                return -5;
            }
        }
    }
    return 0;
}

Tom::state Tom::DFA::func(const state &from, const char &ch, string &err_msg) const {
    if (use_alphabet && alphabet.find(ch) == alphabet.end()) {
        err_msg = string("a char: ") + ch + " not in alphabet, current state: " + to_string(from);
        return -1;
    }

    auto it_from = trans.find(from);
    if (it_from == trans.end()) {
        err_msg = string("current state: ") + to_string(from) + " doesn't have any out edges";
        return -2;
    }

    auto it_edge = it_from->second.find(ch);
    if (it_edge == it_from->second.end()) {
        err_msg = string("current state: ") + to_string(from) + " doesn't have an edge with " + ch;
        return -3;
    }
    return it_edge->second;
}

bool Tom::DFA::accept(const string &str, state &final_state, string &err_msg) const {
    state curr = s0;
    for (const auto &ch : str) {
        curr = func(curr, ch, err_msg);
        if (curr < 0) {
            final_state = curr;
            return false;
        }
    }

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
    res += "},\nstart = " + to_string(s0) + ", finals = {";
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