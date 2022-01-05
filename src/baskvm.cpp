#include "baskvm.h"

using namespace std;

int main(int argc, char *argv[]) {
    vector<uint8_t> code;
    if (argc >= 2) {
        string mode = argv[1];
        if (mode == "compile") {
            
        } else if (mode == "run") {
            string filename = argv[2];
            ifstream file(filename, ios::in | ios::binary);
            char buffer[BUFFER_SIZE];
            file.read(buffer, BUFFER_SIZE);
            for (char chr : buffer) {
                code.push_back((uint8_t)chr);
            }
        } 
    }
    auto stack = new Stack();
    init_inbuilts();
    BaskVM vm = BaskVM(code, stack);
    vm.exec();
    return 0; 
}