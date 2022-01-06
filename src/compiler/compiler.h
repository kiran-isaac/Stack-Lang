#ifndef A0309F06_E921_4D1E_838C_70638E041BD3
#define A0309F06_E921_4D1E_838C_70638E041BD3

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <regex>
#include "token.h"
#include "values.h"
#include "logger.h"
#include "../opcode.h"
#include "../datatypes.h"

// 10kB
#define BUFFER_SIZE 1024 * 10

#define ADD(b) out.push_back(b);
#define ADD_WORD(w) {BYTE *byte = (BYTE*)&w; ADD(byte[3]); ADD(byte[2]); ADD(byte[1]); ADD(byte[0]);}

typedef uint8_t BYTE;
typedef uint32_t WORD;

class Compiler {
public:
    Compiler(char* fname);
    void compile();
    TokenType match(std::string);
    std::vector<Token> Tokenize();
    WORD add_constant(Value val);
    std::vector<BYTE> get_constants();

    std::vector<Value> constants;
    std::string filename;
    std::string dir;
    std::string src;
    std::ofstream fout;

    // Priorities
    std::map<std::string, TokenType> token_map1;
    std::map<std::string, TokenType> token_map2;
};

#endif /* A0309F06_E921_4D1E_838C_70638E041BD3 */
