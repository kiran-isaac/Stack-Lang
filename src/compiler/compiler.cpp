#include "../include/compiler.h"

using namespace std;

string keyword(string kw) {
    string kwUpper = kw;
    transform(kwUpper.begin(), kwUpper.end(), kwUpper.begin(), ::toupper);
    return "^(" + kw + "|" + kwUpper + ")$";
} 

Compiler::Compiler(BSKConfig* config) {
    src = "";

    if (config->out != "") {
        fout = ofstream(config->out);
    }

    for (File *input : config->inputs)
    {   
        ifstream file(input->name);
        char buffer[BUFFER_SIZE] = { 0 };
        file.read(buffer, BUFFER_SIZE);
        string str = buffer;
        src += str + " ";
        file.close();
    }

    constants = vector<Value>();

    token_map1[keyword("inbuilt")] = TokenType::KW_INBUILT;
    token_map1[keyword("push")] = TokenType::KW_PUSH;
    token_map1[keyword("switch")] = TokenType::KW_SWITCH;
    token_map1[keyword("create")] = TokenType::KW_CREATE;
    token_map1[keyword("bring")] = TokenType::KW_BRING;
    token_map1[keyword("copy")] = TokenType::KW_COPY;
    token_map1[keyword("goto")] = TokenType::KW_GOTO;
    token_map1[keyword("branch")] = TokenType::KW_BRANCH;
    token_map1[keyword("pop")] = TokenType::KW_POP;
    token_map1[keyword("dup")] = TokenType::KW_DUP;
    token_map1[keyword("noop")] = TokenType::KW_NOOP;
    token_map1[keyword("clone")] = TokenType::KW_CLONE;
    token_map1[keyword("func")] = TokenType::KW_FUNC;
    token_map1[keyword("endfunc")] = TokenType::KW_ENDFUNC;
    token_map1["^:[A-Za-z_][A-Za-z_0-9]+"] = TokenType::FUNC_CALL;
    token_map1["^[A-Za-z_][A-Za-z_0-9]*:"] = TokenType::LABEL;
    token_map1["^->$"] = TokenType::MACRO_LINE_ROLLOVER;
    token_map1["^\\[$"] = TokenType::REVERSE_START;
    token_map1["^]$"] = TokenType::REVERSE_END;
    token_map1["^(&\\S+$|\\+|/|\\*|-|\\^|==|<|>|<=|>=|!=|!|&&|\\|\\||^)$"] = TokenType::MACRO;
    token_map1["^'(.|\\\\.)'$"] = TokenType::CHAR_LIT;
    token_map1["^\".+\"$"] = TokenType::STRING_LIT;
    token_map1["^[-]?([0-9]*[.])?[0-9]+$"] = TokenType::NUM_LIT;

    token_map2["^[A-Za-z_]+$"] = TokenType::ID;

    ADD_MACRO("+", ":add");
    ADD_MACRO("*", ":mul");
    ADD_MACRO("-", ":sub");
    ADD_MACRO("/", ":div");
    ADD_MACRO("^", ":pow");

    ADD_MACRO("==", ":equal");
    ADD_MACRO("!=", ":not_equal");
    ADD_MACRO(">", ":gt");
    ADD_MACRO("<", ":lt");
    ADD_MACRO(">=", ":gte");
    ADD_MACRO("<=", ":lte");
    ADD_MACRO("!", ":not");
    ADD_MACRO("&&", ":and");
    ADD_MACRO("||", ":or");
    ADD_MACRO("^", ":xor");
    
    ADD_MACRO("&newline", "'\\n' &char :print");
    ADD_MACRO("&cs", "CREATE SWITCH");
    ADD_MACRO("&c", "CREATE");
    ADD_MACRO("&s", "SWITCH");
    ADD_MACRO("&sd", "SWITCH default");

    ADD_DATATYPE_MACRO("&num", to_string(DT_NUM));
    ADD_DATATYPE_MACRO("&char", to_string(DT_CHAR));
    ADD_DATATYPE_MACRO("&string", to_string(DT_STRING));

    inbuilts = {
        "add",
        "sub",
        "mul",
        "div",
        "pow",
        "equal",
        "not_equal",
        "lt",
        "gt",
        "lte",
        "gte",
        "not",
        "and",
        "xor",
        "or",
        "print",
        "exit",
        "stack_len",
        "reverse",
        "stack_id",
        "str_concat",
        "start_timer",
        "end_timer"
    };
}

std::vector<BYTE> readFile(const char* filename)
{
    // open the file:
    std::ifstream file(filename, std::ios::binary);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<BYTE> vec;
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<BYTE>(file),
               std::istream_iterator<BYTE>());

    return vec;
}

std::vector<BYTE> Compiler::compile() {
    extract_macros();
    remove_directives_and_comments();
    vector<Token> tokens = Tokenize(this->src);
    tokens.reserve(get_macro_length(tokens) + 5);
    expand_macros(tokens);
    reverse(tokens);
    expand_strings(tokens);
    extract_functions(tokens);

    vector<BYTE> output;
    vector<BYTE> out;
    vector<BYTE> fn_code;

    WORD size = 0;
    size += functions.size();

    if (stdlib != "") {
        std::vector<BYTE> v2 = readFile(stdlib.c_str());
        WORD stdlib_size = COMBINE_8_BYTES(
            v2[0], 
            v2[1], 
            v2[2], 
            v2[3], 
            v2[4], 
            v2[5], 
            v2[6], 
            v2[7]
            );
        WORD x = size + stdlib_size;
        ADD_WORD(x);
        v2.erase(v2.begin());
        v2.erase(v2.begin());
        v2.erase(v2.begin());
        v2.erase(v2.begin());
        v2.erase(v2.begin());
        v2.erase(v2.begin());
        v2.erase(v2.begin());
        v2.erase(v2.begin());
        for (BYTE byte : v2) {
            out.push_back(byte);
        }
    } else {
        ADD_WORD(size);
    }

    for (map<string, vector<Token>>::iterator i = functions.begin(); i != functions.end(); i++) {
        fn_code = code_gen(i->second);
        
        for (char c : i->first) {
            ADD((BYTE)c);
        }

        ADD(0x00);

        WORD length = fn_code.size() + 1;

        ADD_WORD(length);
        for (BYTE bte : fn_code) {
            ADD(bte);
        }
        ADD(0x00);
    }

    for (BYTE byte : out) {
        output.push_back(byte);
        fout.write((char*)&byte, 1);
    }

    fout.close();

    return output;
}