#include "baskvm.h"

int main(int argc, char const *argv[]) {
    auto stack = new Stack();
    init_inbuilts();
    BaskVM vm = BaskVM(stack);
    vm.exec();
    std::cout << "Done" << std::endl;
    return 0; 
}