#include "compiler.h"

using namespace std;

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

vector<BYTE> get_labels(map<string, WORD> labels) {
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

vector<BYTE> Compiler::code_gen(vector<Token> &tokens) {
    map<string, WORD> labels;
    vector<BYTE> out = vector<BYTE>();
    vector<BYTE> code = vector<BYTE>();
    int i = 0;
    while (i < tokens.size()) {
        Token tk = tokens[i];
        switch (tk.type) {
            case TokenType::KW_NOOP:
                i++;
                break;
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
                    labels[tk.val.substr(0, tk.val.size() - 1)] = out.size();
                    i++;
                }
                break;
            case TokenType::KW_POP:
                ADD(OP_POP);
                i++;
                break;
            case TokenType::KW_DUP:
                ADD(OP_DUP);
                i++;
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
                        call = call.substr(1, call.size() - 1);
                        if (find(inbuilts.begin(), inbuilts.end(), call) != inbuilts.end()) {
                            ADD(FUNC_CALL_MODE_INBUILT);
                        } else {
                            ADD(FUNC_CALL_MODE_LOCAL);
                        }
                    }
                    
                    for (char chr : call) {
                        ADD((uint8_t)chr);
                    }
                    // Null Termination
                    ADD(0x00);
                    i++;
                }
                break;
            case TokenType::KW_CLONE:
                {
                    Token tk2 = tokens[i+1];
                    ADD(OP_CLONE);
                    if (tk2.type != TokenType::ID) {
                        string macro_error = MACRO_ERROR(tk2);
                        FAIL << "Invalid clone id: " << tk2.val << macro_error;
                    }

                    for (char chr : tk2.val) {
                        ADD((uint8_t)chr);
                    }
                    // Null Termination
                    ADD(0x00);
                    i += 2;
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

    code = get_constants();
    constants.clear();
    
    for (BYTE byte : get_labels(labels)) {
        code.push_back(byte);
    }

    for (BYTE byte : out) {
        code.push_back(byte);
    }

    return code;
}