#ifndef BFF120AA_9C16_4E69_AEFC_841F74C50A29
#define BFF120AA_9C16_4E69_AEFC_841F74C50A29

#include <sstream>
#include <vector>
#include <iostream> 
#include <string.h>

typedef uint8_t BYTE;
typedef uint32_t WORD;

#define READ_BYTE() *ip++
#define READ_BYTES(n) {for (int i = 0; i < n; i++) {READ_BYTE();}}
#define GET_WORD() COMBINE_4_BYTES(READ_BYTE(), READ_BYTE(), READ_BYTE(), READ_BYTE())

#define COMBINE_2_BYTES(a, b) ((uint16_t)a << 8) | b
#define COMBINE_4_BYTES(a, b, c, d) (WORD)a << 24 | (WORD)b << 16 | (WORD)c << 8 | (WORD)d

WORD float_to_word(float f);
WORD char_to_word(char c);

std::string bytes_to_string(std::vector<uint8_t> string);

#endif /* BFF120AA_9C16_4E69_AEFC_841F74C50A29 */