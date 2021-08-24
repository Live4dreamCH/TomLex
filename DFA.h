#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Tom {
    using std::string;
    using std::unordered_map;
    using std::unordered_set;

    using state = int;
    // 邻接表
    using NeiTable = unordered_map<state, unordered_map<char, state>>;

    class DFA {
    private:
        unordered_set<state> states;
        bool use_alphabet;
        unordered_set<char> alphabet;
        NeiTable trans;
        state s0;
        unordered_set<state> final_states;

        bool is_valid();

        state func(const state &from, const char &ch, string &err_msg);

    public:
        // 四元组定义，字母表为全体字符
        DFA(unordered_set<state> &S, NeiTable &f, state &S0, unordered_set<state> &F)
            : states(S), trans(f), s0(S0), final_states(F), use_alphabet(false) {}

        // 五元组定义
        DFA(unordered_set<state> &S, unordered_set<char> &AB, NeiTable &f, state &S0, unordered_set<state> &F)
            : states(S), alphabet(AB), trans(f), s0(S0), final_states(F), use_alphabet(true) {}

        bool accept(const string &str, state &final_state, string &err_msg);
    };
} // namespace Tom
