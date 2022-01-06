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

// 10kB
#define BUFFER_SIZE 1024 * 10

class Compiler {
public:
    Compiler(char* fname);
    void compile();
    TokenType match(std::string);
    std::vector<Token> Tokenize();

    std::vector<Value> constants;
    std::string filename;
    std::string dir;
    std::string src;

    // Priorities
    std::map<std::string, TokenType> token_map1;
    std::map<std::string, TokenType> token_map2;
};

#endif /* A0309F06_E921_4D1E_838C_70638E041BD3 */
