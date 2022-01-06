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

    ofstream fout(dir + "/a.out", ios_base::binary);
    uint8_t chrs[] = {0x00, 0x65, 0x66, '\n'};
    fout.write((char*)(&chrs), sizeof(chrs));

    constants = vector<Value>();

    
    token_map1["^inbuilt$"] = TokenType::KW_INBUILT;
    token_map1["^call$"] = TokenType::KW_CALL;
    token_map1["^push$"] = TokenType::KW_PUSH;
    token_map1["^[-]?([0-9]*[.])?[0-9]+$"] = TokenType::NUM_LIT;

    token_map2["^[A-Za-z_]+$"] = TokenType::KW_ID;
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

void Compiler::compile() {
    vector<Token> tokens = Tokenize();
    cout << "hello" << endl;
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