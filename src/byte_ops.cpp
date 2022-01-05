#include "byte_ops.h"

std::string to_string(std::vector<uint8_t> string) {
    std::ostringstream convert;
    for (uint8_t chr : string) {
        convert << (int)chr;
    }
    return convert.str();
}