#include "vm/baskvm.h"

using namespace std;

int main(int argc, char *argv[]) {
    vector<uint8_t> code;
    if (argc >= 2) {
        string mode = argv[1];
        if (mode == "compile") {
            
        } else if (mode == "run") {
            BaskVM vm = BaskVM();
            vm.load(argv[2]);
            vm.exec();
        } 
    }
}