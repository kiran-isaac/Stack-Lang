#include "vm/baskvm.h"
#include "compiler/compiler.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        string mode = argv[1];
        if (mode == "compile") {
            Compiler compiler = Compiler(argv[2]);
            compiler.compile();
        } else if (mode == "run") {
            BaskVM vm = BaskVM();
            vm.load(argv[2]);
            vm.exec();
        } 
    }
}