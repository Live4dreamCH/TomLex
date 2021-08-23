#include <istream>
#include <string>
#include <vector>
using std::getline;
using std::istream;
using std::string;
using std::vector;

class TomRegex {
private:
    // 存储读入的原始字符串
    vector<string> origin_regex;

public:
    // 从流input中逐行读入字符串
    TomRegex(istream &input);
};