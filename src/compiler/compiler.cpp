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
    token_map1[keyword("push")] = TokenType::KW_PUSH;
    token_map1[keyword("switch")] = TokenType::KW_SWITCH;
    token_map1[keyword("create")] = TokenType::KW_CREATE;
    token_map1[keyword("bring")] = TokenType::KW_BRING;
    token_map1[keyword("copy")] = TokenType::KW_COPY;
    token_map1[keyword("goto")] = TokenType::KW_GOTO;
    token_map1[keyword("branch")] = TokenType::KW_BRANCH;
    token_map1["^[A-Za-z_]*:[A-Za-z_]+"] = TokenType::FUNC_CALL;
    token_map1["^[A-Za-z_]+:"] = TokenType::LABEL;
    token_map1["^->$"] = TokenType::MACRO_LINE_ROLLOVER;
    token_map1["^\\[$"] = TokenType::REVERSE_START;
    token_map1["^]$"] = TokenType::REVERSE_END;
    token_map1["^(&\\S+$|\\+|/|\\*|-|\\^|==|<|>|<=|>=)$"] = TokenType::MACRO;
    token_map1["^'(\\S|\\\\\\S)'$"] = TokenType::CHAR_LIT;
    token_map1["^\".+\"$"] = TokenType::STRING_LIT;
    token_map1["^[-]?([0-9]*[.])?[0-9]+$"] = TokenType::NUM_LIT;

    token_map2["^[A-Za-z_]+$"] = TokenType::ID;

    ADD_MACRO("+", ":add");
    ADD_MACRO("*", ":mul");
    ADD_MACRO("-", ":sub");
    ADD_MACRO("/", ":div");
    ADD_MACRO("^", ":pow");
    ADD_MACRO("==", ":equal");
    ADD_MACRO(">", ":gt");
    ADD_MACRO("<", ":lt");
    ADD_MACRO(">=", ":gte");
    ADD_MACRO("<=", ":lte");
    
    ADD_MACRO("&newline", "'\\n' &char :print");
    ADD_MACRO("&cs", "CREATE SWITCH");
    ADD_MACRO("&c", "CREATE");
    ADD_MACRO("&s", "SWITCH");
    ADD_MACRO("&sd", "SWITCH default");

    ADD_DATATYPE_MACRO("&num", to_string(DT_NUM));
    ADD_DATATYPE_MACRO("&char", to_string(DT_CHAR));
    ADD_DATATYPE_MACRO("&string", to_string(DT_STRING));
}

WORD Compiler::add_constant(Value val) {
    for (int i = 0; i < constants.size(); i++) {
        switch (val.type)
        {
        case DT_NUM:
            if (constants[i].d() == val.d() && constants[i].type == val.type) {
                return i;
            }
            break;
        case DT_CHAR: 
            if (constants[i].c() == val.c() && constants[i].type == val.type) {
                return i;
            } 
        }
    }
    constants.push_back(val);
    return constants.size() - 1;
}

vector<BYTE> Compiler::get_constants() {
    vector<BYTE> out = vector<BYTE>();
    WORD size = constants.size();
    ADD_WORD(size);
    for (Value val : constants) {
        if (val.type == DT_NUM) {
            ADD(DT_NUM);
            double d = val.d();
            WORD word = *((WORD*)(&d));
            ADD_WORD(word)
        }
        if (val.type == DT_CHAR) {
            ADD(DT_CHAR);
            ADD((BYTE)val.c())
        }
    }
    return out;
}

vector<BYTE> Compiler::get_labels() {
    vector<BYTE> out = vector<BYTE>();
    WORD size = labels.size();
    ADD_WORD(size);
    for (map<string, WORD>::iterator i = labels.begin(); i != labels.end(); i++) {
        for (char chr : i->first) {
            ADD((BYTE)chr);
        }
        ADD(0x00);
        ADD_WORD(i->second);
    }
    return out;
}

void Compiler::compile() {
    extract_macros();
    remove_directives();
    vector<Token> tokens = Tokenize(this->src);
    tokens.reserve(get_macro_length(tokens) + 5);
    expand_strings(tokens);
    expand_macros(tokens);
    reverse(tokens);
    add_exit(tokens);
    vector<BYTE> out = vector<BYTE>();
    int i = 0;
    while (i < tokens.size()) {
        Token tk = tokens[i];
        switch (tk.type) {
            case TokenType::NUM_LIT:
                {
                    ADD(OP_CONST);
                        
                    WORD const_index = add_constant(NUMBER(stof(tk.val)));
                    ADD_WORD(const_index);
                    i++;
                }
                break;
            case TokenType::CHAR_LIT:
                {
                    ADD(OP_CONST);
                    char val = (tk.val != "'\\n'") ? tk.val[1] : '\n';
                    WORD const_index = add_constant(CHAR(val));
                    ADD_WORD(const_index);
                    i++;
                }
                break;
            case TokenType::LABEL: 
                {
                    labels[tk.val.substr(0, tk.val.size() - 1)] = i;
                    i++;
                }
                break;
            case KW_GOTO: case KW_BRANCH:
                {
                    Token tk2 = tokens[i+1];
                    ADD((tk.type == KW_GOTO) ? OP_GOTO : OP_BRANCH);
                    if (tk2.type != TokenType::ID) {
                        string macro_error = (tk.macro != "") ? " (expanded from macro: " + tk.macro + ")" : "";
                        FAIL << "Invalid label id in GOTO: " << tk2.val << macro_error;
                    }
                    for (char chr : tk2.val) {
                        ADD((uint8_t)chr);
                    }
                    // Null Termination
                    ADD(0x00);
                    i += 2;
                }
                break;
            case TokenType::FUNC_CALL:
                {
                    string call = tk.val;
                    ADD(OP_FUNC_CALL);
                    if (call.find(":") == 0) {
                        ADD(FUNC_CALL_MODE_INBUILT);
                        call = call.substr(1, call.size() - 1);
                    }
                    
                    for (char chr : call) {
                        ADD((uint8_t)chr);
                    }
                    // Null Termination
                    ADD(0x00);
                    i++;
                }
                break;
            case TokenType::KW_SWITCH:
                {
                    Token tk2 = tokens[i+1];
                    ADD(OP_SWITCH);
                    if (tk2.type != TokenType::ID) {
                        string macro_error = MACRO_ERROR(tk2);
                        FAIL << "Invalid switch id: " << tk2.val << macro_error;
                    }
                    for (char chr : tk2.val) {
                        ADD((uint8_t)chr);
                    }
                    // Null Termination
                    ADD(0x00);
                    i += 2;
                }
                break;
            case TokenType::KW_CREATE: 
                {
                    ADD(OP_CREATE);

                    Token tk2 = tokens[i+1];
                    Token tk = tk2;
                    if (tk2.type == KW_SWITCH) {
                        Token tk3 = tokens[i+2];
                        if (tk3.type != TokenType::ID) {
                            string macro_error = MACRO_ERROR(tk3);
                            FAIL << "Invalid switch id: " << tk3.val << macro_error;
                        }
                        for (char chr : tk3.val) {
                            ADD((uint8_t)chr);
                        }
                        // Null Termination
                        ADD(0x00);
                        ADD(OP_SWITCH);
                        i++;
                        tk = tk3;
                    }

                    if (tk.type != TokenType::ID) {
                        string macro_error = MACRO_ERROR(tk);
                        FAIL << "Invalid create id: " << tk.val << macro_error;
                    }
                    for (char chr : tk.val) {
                        ADD((uint8_t)chr);
                    }
                    // Null Termination
                    ADD(0x00);
                    i += 2;
                }
                break;
            case TokenType::KW_BRING: case TokenType::KW_COPY:
                {
                    Token tk2 = tokens[i+1];
                    ADD((tk.type == KW_BRING) ? OP_BRING : OP_COPY);
                    if (tk2.type != TokenType::ID) {
                        string macro_error = MACRO_ERROR(tk2);
                        FAIL << "Invalid switch id: " << tk2.val << macro_error;
                    }
                    for (char chr : tk2.val) {
                        ADD((uint8_t)chr);
                    }
                    // Null Termination
                    ADD(0x00);
                    i += 2;
                }
                break;
            case TokenType::MACRO:
                FAIL << "Cannot expand macro: " << tk.val;
            default:
                FAIL << "Invalid Token in code generation: " << tk.val << MACRO_ERROR(tk);
        }
    }

    for (BYTE byte : get_constants()) {
        fout.write((char*)&byte, 1);
    }
    for (BYTE byte : get_labels()) {
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

vector<Token> Compiler::Tokenize(string src, string macro_id) {
    vector<Token> output = vector<Token>();
    vector<string> strs = split_space(src);
    for (string str : strs) {
        TokenType tktpe = match(str);
        if (tktpe == TokenType::INVALID) {
            FAIL << "Invalid token: " << str << ((macro_id == "") ? ""  : " in definition of macro: ") << macro_id;  
        }
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
    return TokenType::INVALID;
}