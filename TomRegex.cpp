#include "TomRegex.h"

TomRegex::TomRegex(istream &input) {
    string line;
    while (input) {
        getline(input, line);
        this->origin_regex.emplace_back(line);
    }
}