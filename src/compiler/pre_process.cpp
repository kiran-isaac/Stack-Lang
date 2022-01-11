#include "compiler.h"

using namespace std;

void Compiler::expand_strings(vector<Token> &tokens) {
    for (vector<Token> ::iterator token_iterator = tokens.begin(); token_iterator != tokens.end(); token_iterator++) {
        if (token_iterator->type == TokenType::STRING_LIT) {
            string str = token_iterator->val.substr(1, token_iterator->val.size() - 2) + (char)0x00;
            std::reverse(str.begin(), str.end());
            tokens.erase(token_iterator);
            
            for (int i = 0; i < str.size(); i++) {
                if (str[i] == '\\') {
                    tokens.insert(token_iterator, Token{TokenType::CHAR_LIT, "'" + string(1, str[i]) + string(1, str[i+1]) + "'"});
                    i++;
                } else {
                    tokens.insert(token_iterator, Token{TokenType::CHAR_LIT, "'" + string(1, str[i]) + "'"});
                }
                token_iterator++;
            }

            token_iterator = tokens.begin();
        }
    }
}

void Compiler::add_exit(vector<Token> &tokens) {
    bool exit = false;
    for (vector<Token> ::iterator token_iterator = tokens.begin(); token_iterator != tokens.end(); token_iterator++) {
        if (token_iterator->type == TokenType::FUNC_CALL && (token_iterator + 1)->type == TokenType::KW_INBUILT 
        && (token_iterator+2)->val == "exit") {
            exit = true;
        }
    }

    if (!exit) {
        tokens.push_back(Token{NUM_LIT, "0", "auto added exit"});
        tokens.push_back(Token{FUNC_CALL, ":exit", "auto added exit"});
    }
}

void Compiler::reverse(std::vector<Token> &tokens) {
    bool reverse = false;
    for (vector<Token> ::iterator token_iterator = tokens.begin(); token_iterator != tokens.end(); token_iterator++) {
        if (token_iterator->type == TokenType::REVERSE_START) {
            reverse = true;
            tokens.erase(token_iterator);
            token_iterator--;
        } else if (token_iterator->type == TokenType::REVERSE_END) {
            reverse = false;
            tokens.erase(token_iterator);
            token_iterator--;
        } else {
            token_iterator->reverse = reverse;
        }
    }

    vector<Token>::iterator reverse_start;
    vector<Token>::iterator reverse_end;
    reverse = false;
    for (vector<Token> ::iterator token_iterator = tokens.begin(); token_iterator != tokens.end(); token_iterator++) {
        if (!reverse) {
            if (token_iterator->reverse) 
            {
                reverse_start = token_iterator; 
                reverse = true;
            }
        } else {
            if (!token_iterator->reverse) {
                reverse_end = token_iterator;
                std::reverse(reverse_start, reverse_end);
                reverse = false;
            }
        }
    }
}

void Compiler::remove_directives() {
    vector<string> strs = split_lines(src);
    vector<string>::iterator i;
    for (i = strs.begin(); i != strs.end(); i++) {
        if (i->substr(0, 2) == "//" || i->substr(0, 1) == "#") {
            vector<string> words = split_space(*i);
            if (words[words.size()- 1] == "->") {
                strs.erase(i + 1);
            }
            strs.erase(i);
            i--;
        }
    }
    src = "";
    for (string str : strs) {
        src += str + "\n";
    }
    for (string::iterator i = src.begin(); i != src.end(); i++) {
        if (*i == '(' || *i == ')') {
            src.erase(i);
            i--;
        }
    }
}