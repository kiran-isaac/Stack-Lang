#include "byte_ops.h"

std::string bytes_to_string(std::vector<uint8_t> string) {
    std::ostringstream convert;
    for (uint8_t chr : string) {
        convert << (char)chr;
    }
    return convert.str();
}