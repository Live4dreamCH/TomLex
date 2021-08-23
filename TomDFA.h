#include <unordered_map>
#include <unordered_set>
using std::unordered_map;
using std::unordered_set;

class TomDFA {
private:
    unordered_set<int> state;
    unordered_set<char> alphabet;
    unordered_set<unordered_map<char, int>> trans;
    int s0;
    unordered_set<int> final_state;

public:
    // 五元组定义
    TomDFA(unordered_set<int> &S, unordered_set<char> &AB,
           unordered_set<unordered_map<char, int>> &f, int &S0, unordered_set<int> &F)
        : state(S), alphabet(AB), trans(f), s0(S0), final_state(F) {}
};