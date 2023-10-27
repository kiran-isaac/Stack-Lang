#include <unistd.h>
#include <cstdio>
#include "compiler/compiler.h"
#include "config.h"
#include "vm/vm.h"

using namespace std;

int main(int argc, char *argv[])
{
  BSKConfig *config = parse_args(argc, argv);

  if (config->mode == Mode::RUN)
  {
    VM vm{};
    vm.exec(config);
    return 0;
  } else if (config->mode == Mode::COMPILE) {
    Compiler compiler(config);
    compiler.compile();
  }
  
  return 0;
}