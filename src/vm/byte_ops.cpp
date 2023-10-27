#include "../include/byte_ops.h"

using namespace std;

string bytes_to_string(vector<uint8_t> string) {
    ostringstream convert;
    for (uint8_t chr : string) {
        convert << (char)chr;
    }
    return convert.str();
}

WORD double_to_word(double d) {
    WORD word;
    memcpy(&word, &d, 8);
    
    return word;
}

WORD char_to_word(char c) {
    BYTE byte = (BYTE)c;
    WORD word = 0;
    
    return word + byte;
}