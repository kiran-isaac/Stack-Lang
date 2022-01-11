#ifndef A6DD5A66_C435_414F_A5A8_6A67EFE6FFAC
#define A6DD5A66_C435_414F_A5A8_6A67EFE6FFAC

#include <string>

enum TokenType {
    KW_PUSH,
    KW_INBUILT,
    KW_SWITCH,
    KW_CREATE,
    KW_BRING,
    KW_COPY,
    KW_GOTO,
    KW_BRANCH,
    KW_POP,
    KW_DUP,
    KW_NOOP,
    KW_FUNC,
    KW_ENDFUNC,

    REVERSE_START,
    REVERSE_END,

    LABEL,
    FUNC_CALL,
    
    ID,
    MACRO,
    MACRO_LINE_ROLLOVER,

    NUM_LIT,
    CHAR_LIT,
    STRING_LIT,

    INVALID
};

struct Token {
    TokenType type;
    std::string val;
    std::string macro;
    bool reverse;
};

#endif