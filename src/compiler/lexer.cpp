#include "compiler.h"

using namespace std;

vector<Token> Compiler::Tokenize(string src, string macro_id) {
    vector<Token> output = vector<Token>();
    vector<string> strs = split_space(src);
    for (string str : strs) {
        TokenType tktpe = match(str);
        if (tktpe == TokenType::INVALID) {
            FAIL << "Invalid token: " << str << ((macro_id == "") ? ""  : " in definition of macro: ") << macro_id;  
        }
        Token tk = Token{tktpe, str, macro_id};
        output.push_back(tk);
    }
    return output;
}

TokenType Compiler::match(string str) {
    map<string, TokenType>::iterator i;
    for (i = token_map1.begin(); i != token_map1.end(); i++) {
        regex re(i->first);
        if (regex_match(str, re)) {
            return i->second;
        }
    }
    for (i = token_map2.begin(); i != token_map2.end(); i++) {
        regex re(i->first);
        if (regex_match(str, re)) {
            return i->second;
        }
    }
    return TokenType::INVALID;
}