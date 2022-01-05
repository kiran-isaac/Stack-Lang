#ifndef BFF120AA_9C16_4E69_AEFC_841F74C50A29
#define BFF120AA_9C16_4E69_AEFC_841F74C50A29

#include <sstream>
#include <vector>
#include <iostream> 

#define READ_BYTE() *ip++
#define GET_WORD() COMBINE_4_BYTES(READ_BYTE(), READ_BYTE(), READ_BYTE(), READ_BYTE())

#define COMBINE_2_BYTES(a, b) ((uint16_t)a << 8) | b
#define COMBINE_4_BYTES(a, b, c, d) ((uint32_t)COMBINE_2_BYTES(a, b) << 16) | COMBINE_2_BYTES(c, d)

std::string to_string(std::vector<uint8_t> string);

#endif /* BFF120AA_9C16_4E69_AEFC_841F74C50A29 */
