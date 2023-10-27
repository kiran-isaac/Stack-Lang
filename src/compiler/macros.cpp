#include "compiler.h"

using namespace std;

int Compiler::get_macro_length(vector<Token> tokens) {
    int size = tokens.size();
    vector<Token>::iterator token_iterator = tokens.begin();
    do {
        if (token_iterator->type == TokenType::STRING_LIT) {
            size += token_iterator->val.size();
            token_iterator ++;
            continue;
        } 
        if (token_iterator->type != TokenType::MACRO) {
            token_iterator++;
            continue;
        }
        if (macros_map.find(token_iterator->val) == macros_map.end()) {
            FAIL << "Cannot expand macro: " << token_iterator->val << endl;
        }
        size += get_macro_length(macros_map[token_iterator->val]) - 1;
        token_iterator++;  
    } while (token_iterator != tokens.end());
    return size;
}

void Compiler::expand_macros(vector<Token> &tokens) {
    int x = get_macro_length(tokens);
    tokens.reserve(x);
    map<string, vector<Token>>::iterator map_iterator;
    vector<Token>::iterator token_iterator = tokens.begin();
    do {
        if (token_iterator->type != TokenType::MACRO) {
            token_iterator++;
            continue;
        }
        bool found = false;
        for (map_iterator = macros_map.begin(); map_iterator != macros_map.end(); map_iterator++) {
            if (map_iterator->first == token_iterator->val) {
                found = true;
                tokens.erase(token_iterator);
                vector<Token> replace = map_iterator->second;
                for (Token tk : replace) {
                    tokens.insert(token_iterator, tk);
                    token_iterator = next(token_iterator);
                }
                token_iterator = tokens.begin();
            }
        }
        if (!found) {
            FAIL << "Cannot expand macro: " << token_iterator->val << endl;
        }
    } while (token_iterator != tokens.end());
}

void Compiler::extract_macros() {
    vector<string> strs = split_lines(src);
    string rollover = "";
    for (string str : strs) {
        auto words = split_space(str);
        if (words.size() == 0) continue;
        std::transform(words[0].begin(), words[0].end(), words[0].begin(), ::tolower);
        if (words[0] == "#macro" || rollover != "") {
            string key; 
            if (rollover != "") {
                key = rollover;
            } else {
                key = words[1];
                if (!regex_match(key, regex("^&[A-Za-z0-9_]+$"))) {
                    FAIL << "Invalid macro name: '" << key << "'. Must start with &, and must contain only alphanumeric characters, plus _";
                }
                words.erase(words.begin());
                words.erase(words.begin());
                macros_map[key] = vector<Token>();
            }

            rollover = "";
            
            for (string word : words) {
                Token tk = Tokenize(word, key)[0];
                if (tk.type == TokenType::MACRO_LINE_ROLLOVER) {
                    rollover = key;
                    break;
                }
                macros_map[key].push_back(tk);

            }
            expand_macros(macros_map[key]);
        }
    }
}
