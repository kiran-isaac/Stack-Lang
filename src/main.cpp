#include "vm/baskvm.h"
#include "compiler/compiler.h"

using namespace std;

int main(int argc, char *argv[]) {
    string mode = argv[1];
    if (mode == "compile") {
        Compiler compiler = Compiler(argc, argv);
        compiler.compile();
    } else if (mode == "run") {
        BaskVM vm = BaskVM();
        vm.code = vm.load(argv[2]);
        vm.exec();
    } else {
        Compiler compiler = Compiler(argc, argv);
        BaskVM vm = BaskVM();
        vm.code = compiler.compile();
        vm.exec();
    }
}