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
#include "../values.h"
#include "logger.h"
#include "../opcode.h"
#include "string_ops.h"

// 10kB
#define BUFFER_SIZE 1024 * 10

#define ADD(b) out.push_back(b);
#define ADD_WORD(w) {BYTE *byte = (BYTE*)&w; ADD(byte[7]); ADD(byte[6]); ADD(byte[5]); ADD(byte[4]);\
            ADD(byte[3]); ADD(byte[2]); ADD(byte[1]); ADD(byte[0]);}

#define MACRO_ERROR(tk) (tk.macro != "") ? " (expanded from macro: " + tk.macro + ")" : ""

#define ADD_INBUILT_MACRO(alias, macro) {macros_map[alias] = vector<Token>();ADD_MACRO(alias, (string)"CALL INBUILT " + macro)}

#define ADD_DATATYPE_MACRO(alias, value) {macros_map[alias] = vector<Token>();\
    macros_map[alias].push_back(Token{TokenType::NUM_LIT, value, alias});}
#define ADD_MACRO(alias, value) {macros_map[alias] = vector<Token>();\
    for (Token tk : Tokenize(value)) {\
        macros_map[alias].push_back(tk);\
    }}

#define WHTIESPACE_CHARS() 

typedef uint8_t BYTE;
typedef uint64_t WORD;

class Compiler {
public:
    std::vector<std::string> inbuilts;

    Compiler(char* fname);
    std::vector<BYTE> compile();
    TokenType match(std::string);
    WORD add_constant(Value val);
    void add_function(std::string name);
    std::vector<BYTE> get_constants();
    std::vector<BYTE> get_funcs();
    void remove_directives();
    void expand_macros(std::vector<Token> &tokens);
    void extract_macros();
    void expand_strings(std::vector<Token> &tokens);
    void add_exit(std::vector<Token> &tokens);
    void reverse(std::vector<Token> &tokens);
    void extract_functions(std::vector<Token> &tokens);
    int get_macro_length(std::vector<Token> tokens);
    std::vector<Token> Tokenize(std::string, std::string = "");

    std::vector<BYTE> code_gen(std::vector<Token> &tokens);

    std::vector<Value> constants;
    std::string filename;
    std::string dir;
    std::string src;
    std::ofstream fout;

    std::map<std::string, std::vector<Token>> functions;

    // Priorities
    std::map<std::string, TokenType> token_map1;
    std::map<std::string, TokenType> token_map2;

    std::map<std::string, std::vector<Token>> macros_map;
};

#endif /* A0309F06_E921_4D1E_838C_70638E041BD3 */
