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
}

vector<string> split(std::string str)
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

    return out;
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
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
    vector<Token> tokens = Tokenize();
    tokens.push_back(Token{TokenType::ID, "exit"});
    vector<BYTE> out = vector<BYTE>();
    int i = 0;
    while (i < tokens.size()) {
        Token tk = tokens[i];
        Token tk2 = tokens[i+1];
        switch (tk.type) {
            case TokenType::KW_PUSH:
                {
                    if (tk2.type != TokenType::NUM_LIT) {
                        FAIL << "Invalid Token: " << tk2.val;
                        break;
                    }
                    i += 2;
                    ADD(OP_CONST);
                    
                    WORD const_index = add_constant(NUMBER(stof(tk2.val)));
                    ADD_WORD(const_index);
                    break;
                }
            case TokenType::KW_CALL:
                {
                    Token tk3 = tokens[i+2];
                    ADD(OP_FUNC_CALL);
                    if (tk2.type != TokenType::KW_INBUILT) {
                        FAIL << "Invalid Token: " << tk2.val;
                    }
                    ADD(FUNC_CALL_MODE_INBUILT);
                    if (tk3.type != TokenType::ID) {
                        FAIL << "Invalid Token: " << tk2.val;
                    }
                    for (char chr : tk3.val) {
                        ADD((uint8_t)chr);
                    }
                    // Null Termination
                    ADD(0x00);
                    i += 3;
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

vector<Token> Compiler::Tokenize() {
    vector<Token> output = vector<Token>();
    vector<string> strs = split(src);
    for (string str : strs) {
        trim(str);
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