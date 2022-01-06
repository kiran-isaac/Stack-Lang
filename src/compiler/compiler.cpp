#include "compiler.h"

using namespace std;

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

    
    token_map1["^inbuilt$"] = TokenType::KW_INBUILT;
    token_map1["^call$"] = TokenType::KW_CALL;
    token_map1["^push$"] = TokenType::KW_PUSH;
    token_map1["^[-]?([0-9]*[.])?[0-9]+$"] = TokenType::NUM_LIT;

    token_map2["^[A-Za-z_]+$"] = TokenType::ID;

    macros_map["+"] = "CALL INBUILT ADD";
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
    expand_macros();
    vector<Token> tokens = Tokenize();
    vector<BYTE> out = vector<BYTE>();
    int i = 0;
    while (i < tokens.size()) {
        Token tk = tokens[i];
        switch (tk.type) {
            case TokenType::KW_PUSH:
                {
                    Token tk2 = tokens[i+1];
                    if (tk2.type != TokenType::NUM_LIT) {
                        FAIL << "Invalid Token: " << tk2.val;
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
                                FAIL << "Invalid inbuilt name: " << tk3.val;
                            }
                            for (char chr : tk3.val) {
                                ADD((uint8_t)chr);
                            }
                            // Null Termination
                            ADD(0x00);
                            i += 3;
                            break;
                        default:
                            FAIL << "Invalid call scope: " << tk2.val;
                    }
                    break;
                }
            default:
                FAIL << "Invalid Token: " << tk.val;
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

void Compiler::expand_macros() {
    map<string, string>::iterator i;
    for (i = macros_map.begin(); i != macros_map.end(); i++) {
        size_t pos = 0;
        while((pos = src.find((i->first), pos)) != std::string::npos) {
            src.replace(pos, (i->first).length(), (i->second));
            pos += (i->first).length();
        }
    }
}

vector<Token> Compiler::Tokenize() {
    vector<Token> output = vector<Token>();
    vector<string> strs = split(src);
    for (string str : strs) {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        TokenType tktpe = match(str);
        Token tk = Token{tktpe, str};
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
}