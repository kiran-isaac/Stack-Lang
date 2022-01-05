#include "inbuilt.h"



using pfunc = void (*)(Stack* stack);
std::map<uint32_t, pfunc> funcMap;
    
void call_inbuilt(std::vector<uint8_t> name) {
    std::string str
}


void print(Stack* stack) {
    std::cout << "WOOWOO\n";
}