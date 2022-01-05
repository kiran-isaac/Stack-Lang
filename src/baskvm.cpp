#include "baskvm.h"

int main(int argc, char const *argv[]) {
    auto stack = new Stack();
    BaskVM vm = BaskVM(stack);
    vm.exec();
    std::cout << "Done" << std::endl;
    return 0; 
}