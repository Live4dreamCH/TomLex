#include "NFA.h"
#include <memory>
#include <queue>
#include <tuple>
using std::get;
using std::make_shared;
using std::queue;
using std::shared_ptr;
using std::to_string;
using std::tuple;
using namespace Tom;

const unordered_set<state> *NFA::func(const state &from, const string &str, string &err_msg) const {
    // 输入字符不在字符集中
    for (const auto &ch : str) {
        if (use_alphabet && alphabet.find(ch) == alphabet.end()) {
            err_msg = string("a char: ") + ch + " in input: " + str + " not in alphabet, current state: " + to_string(from);
            return nullptr;
        }
    }

    // 源状态不在转移表中
    auto it_from = trans.find(from);
    if (it_from == trans.end()) {
        err_msg = string("current state: ") + to_string(from) + " not in trans table";
        return nullptr;
    }

    // 源状态的出边中无此字符串
    auto it_edge = it_from->second.find(str);
    if (it_edge == it_from->second.end()) {
        err_msg = string("current state: ") + to_string(from) + " doesn't have an edge with " + str;
        return nullptr;
    }
    return &(it_edge->second);
}

int NFA::check(string &err_msg) const {
    // 初态不应为空集
    if (starts.size() == 0) {
        return -1;
    }
    // 初态不在状态集中
    for (const auto &start : starts) {
        if (states.find(start) == states.end()) {
            err_msg = "start state " + to_string(start) + " not in state set!";
            return -2;
        }
    }
    // 终态不在状态集中
    for (const auto &s : final_states) {
        if (states.find(s) == states.end()) {
            err_msg = "final state " + to_string(s) + " not in state set!";
            return -3;
        }
    }
    // 转移表
    for (const auto &it_from : trans) {
        // 源状态不在状态集中
        const auto &from = it_from.first;
        if (states.find(from) == states.end()) {
            err_msg = "from state " + to_string(from) + " not in state set!";
            return -4;
        }
        // 源状态的出边
        for (const auto &edge : it_from.second) {
            const auto &str = edge.first;
            const auto &tos = edge.second;
            // 串中的字符不在字符集中
            for (auto &ch : str) {
                if (use_alphabet && alphabet.find(ch) == alphabet.end()) {
                    err_msg = string("char ") + ch + " not in alphabet! " + "edge: from=" + to_string(from) + ", str=" + str + ", ch=" + ch;
                    return -5;
                }
            }
            // 目标状态不在状态集中
            for (const auto &to : tos) {
                if (states.find(to) == states.end()) {
                    err_msg = string("to state ") + to_string(to) + " not in state set! " + "edge: from=" + to_string(from) + ", str=" + str + ", to=" + to_string(to);
                    return -6;
                }
            }
        }
    }
    return 0;
}

// NFA的识别本质上是图的寻路
// 由于此NFA允许转移边上的字符串为空串ε
// 若使用DFS, 会因ε圈而进入死循环(不消耗字符,无法前进)
// 所以使用BFS
// 实际使用中应当转化为DFA, 再识别串
bool NFA::accept(const string &str, state &final_state, string &err_msg) const {
    err_msg = "input string: " + str;
    // BFS所需队列, 当前状态+已识别字符串的长度+转移次数
    // 转移次数 > 10 * str.size()时, 剪枝
    queue<tuple<state, std::size_t, std::size_t>> fifo;
    // 所有初态同时入队,一同进行BFS
    // 避免某初态拓展出的路径中有ε圈
    // 导致其它含有正确路径的初态无法开始
    for (const auto &start : starts) {
        fifo.emplace(start, 0, 0);
    }
    while (!fifo.empty()) {
        const auto &curr = get<0>(fifo.front());
        const auto &pos = get<1>(fifo.front());
        const auto &times = get<2>(fifo.front());
        // 很可能进入ε圈, 剪枝
        if (times > 10 * str.size()) {
            err_msg += "\ncut path, curr state = " + to_string(curr) + ", pos = " + to_string(pos);
            fifo.pop();
            continue;
        }
        // 源状态不在转移表中
        auto it_curr = trans.find(curr);
        if (it_curr == trans.end()) {
            fifo.pop();
            continue;
        }
        // 逐边尝试匹配, 若匹配则将新状态、新位置加入队列
        for (const auto &edge : it_curr->second) {
            const auto &s = edge.first;
            if (str.compare(pos, s.size(), s) == 0) {
                for (const auto &to : edge.second) {
                    if (pos + s.size() == str.size() &&
                        final_states.find(to) != final_states.end()) {
                        final_state = to;
                        return true;
                    }
                    fifo.emplace(to, pos + s.size(), times + 1);
                }
            }
        }
        fifo.pop();
    }
    return false;
}

string NFA::show() const {
    return "";
}

int NFA::to_DFA(shared_ptr<DFA> &target, string &err_msg) const {
    // 拷贝需要修改的NFA属性
    NFANei temp_trans;
    unordered_set<state> temp_states = states;

    // 添加唯一初态和终态
    state start_state = get_next_unused_state(0, err_msg);
    if (start_state == -1) {
        return -1;
    }
    temp_states.emplace(start_state);
    temp_trans[start_state].emplace("", starts);

    state final_state = get_next_unused_state(start_state, err_msg);
    if (final_state == -1) {
        return -2;
    }
    temp_states.emplace(final_state);
    for (const auto &end : final_states) {
        temp_trans[end][""].emplace(final_state);
    }

    // 拆分边上的字符串为单个字符
    state unused_state = final_state;
    // 上一个状态, 类似链表操作
    state prev;
    // 逐边检查, 消除字符串边, 并复制到temp_trans中
    for (auto &it_from : trans) {
        auto &from = it_from.first;
        for (auto &it_str : it_from.second) {
            auto &str = it_str.first;
            auto &to = it_str.second;
            prev = from;
            if (str.size() > 1) {
                for (size_t i = 0; i < str.size() - 1; ++i) {
                    // 前后连接
                    unused_state = get_next_unused_state(unused_state, err_msg);
                    if (-1 == unused_state) {
                        return -3;
                    }
                    temp_states.emplace(unused_state);
                    temp_trans[prev][string(1, str[i])].emplace(unused_state);
                    prev = unused_state;
                }
                temp_trans[prev].emplace(string(1, str.back()), to);
            } else {
                for (auto &t : to) {
                    temp_trans[from][str].emplace(t);
                }
            }
        }
    }

    // 子集法消除ε弧和多目标
    // 新DFA的状态->旧NFA的状态子集
    unordered_map<state, unordered_set<state>> state_map;
    DFANei new_trans;
    // BFS队列
    queue<state> fifo;
    // 自增的DFA状态
    state new_state = 0;
    // DFA的终态
    unordered_set<state> new_ends;
    // epsilon闭包或Ia运算的结果
    auto epsilon = epsilon_closure({start_state}, temp_trans);
    state_map.emplace(new_state, *epsilon);
    fifo.emplace(new_state);
    ++new_state;
    // Ia扩展每一个状态子集
    // 填写state_map和new_trans
    // 生成新DFA
    // 返回类型应为智能指针
    while (!fifo.empty()) {
        // 统计所有出边上的字符
        unordered_set<string> chars;
        auto &old_states = state_map.at(fifo.front());
        for (auto &s : old_states) {
            if (temp_trans.find(s) == temp_trans.end()) {
                continue;
            }
            for (auto &edge : temp_trans.at(s)) {
                if (!edge.first.empty())
                    chars.emplace(edge.first);
            }
        }
        // 扩展
        for (auto &ch : chars) {
            epsilon = Ia(old_states, temp_trans, ch);
            state next = -1;
            // 去重
            for (auto &maps : state_map) {
                if (maps.second == *epsilon) {
                    next = maps.first;
                    break;
                }
            }
            if (next != -1) {
                new_trans[fifo.front()].emplace(ch[0], next);
                continue;
            }
            state_map.emplace(new_state, *epsilon);
            fifo.emplace(new_state);
            new_trans[fifo.front()].emplace(ch[0], new_state);
            if (epsilon->find(final_state) != epsilon->end()) {
                new_ends.emplace(new_state);
            }
            ++new_state;
        }
        fifo.pop();
    }

    unordered_set<state> new_states;
    for (state i = 0; i < new_state; ++i) {
        new_states.emplace(i);
    }
    new_state = 0;
    if (use_alphabet) {
        target = make_shared<DFA>(new_states, alphabet, new_trans, new_state, new_ends);
    } else {
        target = make_shared<DFA>(new_states, new_trans, new_state, new_ends);
    }
    return 0;
}

state NFA::get_next_unused_state(const state &curr, string &err_msg) const {
    if (curr == __INT_MAX__) {
        err_msg = "no unused state! cannot add a state";
        return -1;
    }
    for (state i = curr + 1; i < __INT_MAX__; ++i) {
        if (states.find(i) == states.end()) {
            return i;
        }
    }
    if (states.find(__INT_MAX__) == states.end()) {
        return __INT_MAX__;
    }
    err_msg = "no unused state! cannot add a state";
    return -1;
}

// 求state_set的ε闭包
// BFS, 仅限在ε弧上移动
// 将所有途径的状态放入返回值中
shared_ptr<unordered_set<state>> Tom::epsilon_closure(const unordered_set<state> &state_set, const NFANei &in_trans) {
    shared_ptr<unordered_set<state>> closure = make_shared<unordered_set<state>>(state_set);
    // 输入集合的每一个元素, 都是BFS的起点
    for (auto &s : state_set) {
        // BFS队列
        queue<state> fifo;
        fifo.emplace(s);
        while (!fifo.empty()) {
            // 扩展
            if (in_trans.find(fifo.front()) != in_trans.end()) {
                for (auto &edge : in_trans.at(fifo.front())) {
                    if (edge.first == "") {
                        for (auto &to : edge.second) {
                            if (closure->find(to) == closure->end()) {
                                closure->emplace(to);
                                fifo.emplace(to);
                            }
                        }
                    }
                }
            }
            fifo.pop();
        }
    }
    return closure;
}

// Ia运算
// 对输入状态子集I, 先使用字符串a移动一次, 再求ε闭包
shared_ptr<unordered_set<state>> Tom::Ia(const unordered_set<state> &I, const NFANei &in_trans, const string &a) {
    // I做一次a移动后的结果为J
    unordered_set<state> J;
    for (auto &from : I) {
        if (in_trans.find(from) == in_trans.end()) {
            continue;
        }
        auto &edge = in_trans.at(from);
        if (edge.find(a) == edge.end()) {
            continue;
        }
        auto &tos = edge.at(a);
        for (auto &to : tos) {
            J.emplace(to);
        }
    }
    return epsilon_closure(J, in_trans);
}
