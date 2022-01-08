#ifndef A6DD5A66_C435_414F_A5A8_6A67EFE6FFAC
#define A6DD5A66_C435_414F_A5A8_6A67EFE6FFAC

#include <string>

enum TokenType {
    KW_PUSH,
    KW_CALL,
    KW_INBUILT,
    KW_SWITCH,
    KW_CREATE,
    KW_BRING,
    KW_COPY,
    
    ID,
    NUM_LIT,
    CHAR_LIT,
    STRING_LIT,
    MACRO,
    MACRO_LINE_ROLLOVER,

    INVALID
};

struct Token {
    TokenType type;
    std::string val;
    std::string macro;
};

#endif