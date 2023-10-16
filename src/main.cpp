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
  } else if (config->mode == Mode::STANDALONE) {
    Compiler compiler(config);
    compiler.stdlib = config->lib;
    compiler.compile();
    char dest[10000];
    memset(dest, 0, sizeof(dest));
    if (readlink("/proc/self/exe", dest, 10000) == -1)
    {
      perror("Cannot readlink");
    }
    else
    {
      printf("%s\n", dest);
    }

    if (!filesystem::exists(config->lib)) {
      FAIL << "Cannot find library file: " << config->lib;
    }

    system("ld -o binary.o -r -b binary a.out");
    system(("g++ -o " + config->out + " binary.o /usr/lib/slang/slang_portable").c_str());
    system("rm *.o");
    system("rm a.out");

    return 0;
  }

  // if (vm.count("compile") != 0)
  // {
  //   Compiler compiler(out, inputs);
  //   compiler.compile();
  // }
  // else if (vm.count("standalone") != 0)
  // {
  //   Compiler compiler("a.out", inputs);
  //   compiler.stdlib = lib;
  //   compiler.compile();
  //   char dest[10000];
  //   memset(dest, 0, sizeof(dest));
  //   if (readlink("/proc/self/exe", dest, 10000) == -1)
  //   {
  //     perror("Cannot readlink");
  //   }
  //   else
  //   {
  //     printf("%s\n", dest);
  //   }

  //   if (!filesystem::exists(lib))
  //   {
  //     FAIL << "Cannot find library file: " << lib;
  //   }

  //   system("ld -o binary.o -r -b binary a.out");
  //   system(("g++ -o " + out + " binary.o /usr/lib/slang/slang_portable").c_str());
  //   system("rm *.o");
  //   system("rm a.out");

  //   return 0;
  // }
  // else if (vm.count("run") != 0)
  // {
  //   SlangVM vm{};
    
  //   vm.exec(config);
  // }

  return 0;
}