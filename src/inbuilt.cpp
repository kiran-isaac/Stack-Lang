#include "inbuilt.h"

using namespace std;

typedef void (*func_ptr)(Stack* stack);
map<string , func_ptr> funcMap;

void call_inbuilt(vector<uint8_t> name, Stack* stack) {
    string str = bytes_to_string(name);
    cout << str << endl;
    funcMap[str](stack);
}

void print(Stack* stack) {
    uint32_t mode = stack->pop()->number;
    switch (mode) {
        case 0:
            cout << stack->pop()->number << endl;
            break;
        default:
            DIE << "[Inbuilt Function : print]: Invalid value for print type. Push the print type before calling print";
    }
}

void add(Stack* stack) {

}

void init_inbuilts() {
    funcMap["print"] = &print;
    funcMap["add"] = &print;
}