#ifndef A6DD5A66_C435_414F_A5A8_6A67EFE6FFAC
#define A6DD5A66_C435_414F_A5A8_6A67EFE6FFAC

#include <string>

enum TokenType {
    KW_PUSH,
    KW_CALL,
    KW_INBUILT,
    ID,
    NUM_LIT,
    MACRO,

    INVALID
};

struct Token {
    TokenType type;
    std::string val;
    std::string macro;
};

#endif