#include "compiler.h"

using namespace std;

string keyword(string kw) {
    string kwUpper = kw;
    transform(kwUpper.begin(), kwUpper.end(), kwUpper.begin(), ::toupper);
    return "^(" + kw + "|" + kwUpper + ")$";
} 

Compiler::Compiler(char* fname) {
    filename = fname;
    ifstream file(filename);

    const size_t last_slash_idx = filename.rfind("/");
    if (string::npos != last_slash_idx)
    {
        dir = filename.substr(0, last_slash_idx);
    }

    char buffer[BUFFER_SIZE];
    file.read(buffer, BUFFER_SIZE);
    src = buffer;

    fout = ofstream(dir + "/a.out", ios_base::binary);

    constants = vector<Value>();

    token_map1[keyword("inbuilt")] = TokenType::KW_INBUILT;
    token_map1[keyword("call")] = TokenType::KW_CALL;
    token_map1[keyword("push")] = TokenType::KW_PUSH;
    token_map1["^&\\S+$"] = TokenType::MACRO;
    token_map1["^[-]?([0-9]*[.])?[0-9]+$"] = TokenType::NUM_LIT;

    token_map2["^[A-Za-z_]+$"] = TokenType::ID;

    ADD_INBUILT_MACRO("&+", "add");
    ADD_INBUILT_MACRO("&*", "mul");
    ADD_INBUILT_MACRO("&-", "sub");
    ADD_INBUILT_MACRO("&/", "div");
    ADD_INBUILT_MACRO("&print", "print");
    ADD_INBUILT_MACRO("&exit", "exit");

    macros_map["&num"] = vector<Token>();
    macros_map["&num"].push_back(Token{TokenType::NUM_LIT, "0", "num"});
}


WORD Compiler::add_constant(Value val) {
    for (int i = 0; i < constants.size(); i++) {
        if (constants[i].number == val.number && constants[i].type == val.type) {
            return i;
        } 
    }
    constants.push_back(val);
    return constants.size() - 1;
}

vector<BYTE> Compiler::get_constants() {
    vector<BYTE> out = vector<BYTE>();
    uint32_t size = constants.size();
    ADD_WORD(size);
    for (Value val : constants) {
        if (val.type == DT_NUM) {
            ADD(DT_NUM);
            WORD word = *((WORD*)(&val.number));
            BYTE *byte = (BYTE*)&word;
            ADD(byte[3]); ADD(byte[2]); ADD(byte[1]); ADD(byte[0]);
        }
    }
    return out;
}

void Compiler::compile() {
    extract_macros();
    remove_directives();
    vector<Token> tokens = Tokenize(this->src);
    expand_macros(tokens);
    vector<BYTE> out = vector<BYTE>();
    int i = 0;
    while (i < tokens.size()) {
        Token tk = tokens[i];
        switch (tk.type) {
            case TokenType::KW_PUSH:
                {
                    Token tk2 = tokens[i+1];
                    if (tk2.type != TokenType::NUM_LIT) {
                        FAIL << "Invalid Token: " << tk2.val << MACRO_ERROR(tk2);
                        break;
                    }
                    
                    ADD(OP_CONST);
                    
                    WORD const_index = add_constant(NUMBER(stof(tk2.val)));
                    ADD_WORD(const_index);
                    i += 2;
                } 
                break;
            case TokenType::NUM_LIT:
                {
                    ADD(OP_CONST);
                        
                    WORD const_index = add_constant(NUMBER(stof(tk.val)));
                    ADD_WORD(const_index);
                    i += 1;
                }
                break;
            case TokenType::KW_CALL:
                {
                    Token tk2 = tokens[i+1];
                    Token tk3 = tokens[i+2];
                    ADD(OP_FUNC_CALL);
                    switch (tk2.type) {
                        case KW_INBUILT:
                            ADD(FUNC_CALL_MODE_INBUILT);
                            if (tk3.type != TokenType::ID) {
                                string macro_error = MACRO_ERROR(tk3);
                                FAIL << "Invalid inbuilt name: " << tk3.val << macro_error;
                            }
                            for (char chr : tk3.val) {
                                ADD((uint8_t)chr);
                            }
                            // Null Termination
                            ADD(0x00);
                            i += 3;
                            break;
                        default:
                            FAIL << "Invalid call scope: " << tk2.val << MACRO_ERROR(tk2);
                    }
                    break;
                }
            case TokenType::MACRO:
                FAIL << "Cannot expand macro: " << tk.val;
            default:
                FAIL << "Invalid Token: " << tk.val << MACRO_ERROR(tk);
        }
    }

    for (BYTE byte : get_constants()) {
        fout.write((char*)&byte, 1);
    }
    for (BYTE byte : out) {
        fout.write((char*)&byte, 1);
    }
    fout.close();
}



int Compiler::get_macro_length(vector<Token> tokens) {
    int size = tokens.size();
    vector<Token>::iterator token_iterator = tokens.begin();
    do {
        if (token_iterator->type != TokenType::MACRO) {
            token_iterator++;
            continue;
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
        for (map_iterator = macros_map.begin(); map_iterator != macros_map.end(); map_iterator++) {
            if (map_iterator->first == token_iterator->val) {
                tokens.erase(token_iterator);
                vector<Token> replace = map_iterator->second;
                for (Token tk : replace) {
                    tokens.insert(token_iterator, tk);
                    token_iterator = next(token_iterator);
                }
                token_iterator = tokens.begin();
            }
        }
    } while (token_iterator != tokens.end());
}

void Compiler::extract_macros() {
    vector<string> strs = split_lines(src);
    for (string str : strs) {
        auto words = split_space(str);
        if (words[0] == "#MACRO") {
            string key = "&" + words[1];
            words.erase(words.begin());
            words.erase(words.begin());

            macros_map[key] = vector<Token>();
            for (string word : words) {
                macros_map[key].push_back(Tokenize(word, key)[0]);
            }
            expand_macros(macros_map[key]);
        }
    }
}

void Compiler::remove_directives() {
    vector<string> strs = split_lines(src);
    vector<string>::iterator i;
    for (i = strs.begin(); i != strs.end(); i++) {
        if (i->substr(0, 2) == "//" || i->substr(0, 1) == "#") {
            strs.erase(i);
            i--;
        }
    }
    src = "";
    for (string str : strs) {
        src += str + "\n";
    }
}

vector<Token> Compiler::Tokenize(string src, string macro_id) {
    vector<Token> output = vector<Token>();
    vector<string> strs = split_space(src);
    for (string str : strs) {
        TokenType tktpe = match(str);
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
    FAIL << "Unrecognised token: " << str;
    return TokenType::INVALID;
}