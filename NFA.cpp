#include "NFA.h"
#include <queue>
#include <tuple>
using std::tuple;
using std::get;
using std::queue;
using std::to_string;
using namespace Tom;

const unordered_set<state>* NFA::func(const state &from, const string &str, string &err_msg) const{
    // 输入字符不在字符集中
    for(const auto &ch:str){
        if (use_alphabet && alphabet.find(ch) == alphabet.end()) {
            err_msg = string("a char: ") + ch +" in input: "+str+ " not in alphabet, current state: " + to_string(from);
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

int NFA::check(string &err_msg) const{
    // 初态不应为空集
    if(starts.size()==0){
        return -1;
    }
    // 初态不在状态集中
    for(const auto& start:starts){
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
            for(auto &ch:str){
                if (use_alphabet && alphabet.find(ch) == alphabet.end()) {
                    err_msg = string("char ") + ch + " not in alphabet! " + "edge: from=" + to_string(from) + ", str=" + str + ", ch=" + ch;
                    return -5;
                }
            }
            // 目标状态不在状态集中
            for(const auto &to:tos){
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
bool NFA::accept(const string &str, state &final_state, string &err_msg) const{
    err_msg="input string: "+str;
    // BFS所需队列, 当前状态+已识别字符串的长度+转移次数
    // 转移次数 > 10 * str.size()时, 剪枝
    queue<tuple<state,std::size_t,std::size_t>> fifo;
    // 所有初态同时入队,一同进行BFS
    // 避免某初态拓展出的路径中有ε圈
    // 导致其它含有正确路径的初态无法开始
    for(const auto &start:starts){
        fifo.emplace(start,0,0);
    }
    while(!fifo.empty()){
        const auto &curr = get<0>(fifo.front());
        const auto &pos = get<1>(fifo.front());
        const auto &times = get<2>(fifo.front());
        // 很可能进入ε圈, 剪枝
        if(times > 10*str.size()){
            err_msg+="\ncut path, curr state = "+to_string(curr)+", pos = "+to_string(pos);
            fifo.pop();
            continue;
        }
        // 源状态不在转移表中
        auto it_curr = trans.find(curr);
        if(it_curr==trans.end()){
            fifo.pop();
            continue;
        }
        // 逐边尝试匹配, 若匹配则将新状态、新位置加入队列
        for(const auto &edge : it_curr->second){
            const auto &s = edge.first;
            if(str.compare(pos, s.size(),s)==0){
                for(const auto &to : edge.second){
                    if(pos+s.size() == str.size() &&
                       final_states.find(to)!=final_states.end()){
                        final_state = to;
                        return true;
                    }
                    fifo.emplace(to, pos+s.size(), times+1);
                }
            }
        }
        fifo.pop();
    }
    return false;
}

string NFA::show() const{
    return "";
}
