#include "string_ops.h"

using namespace std;

vector<string> split_space(std::string str)
{
    auto out = vector<string>();
    string current;

    for (char chr : str) {
        if (isspace(chr)) {
            if (current.size() != 0) {
                out.push_back(current);
            }

            current = "";
        } else {
            current += chr;
        }
    }
    if (current.size() != 0) {
        out.push_back(current);
    }

    return out;
}

vector<string> split_lines(std::string str)
{
    auto out = vector<string>();
    string current;

    for (char chr : str) {
        if (chr == '\n') {
            if (current.size() != 0) {
                out.push_back(current);
            }

            current = "";
        } else {
            current += chr;
        }
    }
    if (current.size() != 0) {
        out.push_back(current);
    }

    return out;
}