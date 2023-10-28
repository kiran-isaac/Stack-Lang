#ifndef BFF120AA_9C16_4E69_AEFC_841F74C50A29
#define BFF120AA_9C16_4E69_AEFC_841F74C50A29

#include <sstream>
#include <vector>
#include <iostream> 
#include <string.h>

typedef uint8_t BYTE;
typedef uint64_t WORD;

#define READ_BYTES(n) {for (int i = 0; i < n; i++) {read_byte();}}
#define GET_WORD() COMBINE_8_BYTES(read_byte(), read_byte(), read_byte(), read_byte(), read_byte(), read_byte(), read_byte(), read_byte())

#define COMBINE_2_BYTES(a, b) ((uint16_t)a << 8) | b
#define COMBINE_8_BYTES(a, b, c, d, e, f, g, h) (WORD)a << 56 | (WORD)b << 48 | (WORD)c << 40 | (WORD)d << 32 | (WORD)e << 24 | (WORD)f << 16 | (WORD)g << 8 | (WORD)h

WORD double_to_word(double f);
WORD char_to_word(char c);

std::string bytes_to_string(std::vector<uint8_t> string);

#endif /* BFF120AA_9C16_4E69_AEFC_841F74C50A29 */
