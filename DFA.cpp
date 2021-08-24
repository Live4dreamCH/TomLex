#include "DFA.h"
#include <iostream>

Tom::state Tom::DFA::func(const state &from, const char &ch, string &err_msg) {
    if (use_alphabet && alphabet.find(ch) == alphabet.end()) {
        err_msg = string("a char: ") + ch + " not in alphabet, current state: " + std::to_string(from);
        return -1;
    }

    auto it_from = trans.find(from);
    if (it_from == trans.end()) {
        err_msg = string("current state: ") + std::to_string(from) + " doesn't have any out edges";
        return -2;
    }

    auto it_edge = it_from->second.find(ch);
    if (it_edge == it_from->second.end()) {
        err_msg = string("current state: ") + std::to_string(from) + " doesn't have an edge with " + ch;
        return -3;
    }
    return it_edge->second;
}

bool Tom::DFA::accept(const string &str, state &final_state, string &err_msg) {
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
        err_msg = string("stop at state: ") + std::to_string(curr) + ", not a final state";
        return false;
    }

    final_state = curr;
    return true;
}