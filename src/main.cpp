#include <unistd.h>
#include <cstdio>
#include "include/compiler.h"
#include "include/config.h"
#include "include/vm.h"

using namespace std;

int main(int argc, char *argv[])
{
  BSKConfig *config = parse_args(argc, argv);

  if (config->mode == Mode::RUN)
  {
    // get all files that arent bytecode from config
    vector<File *> to_be_compiled = vector<File *>();
    for (File *file : config->inputs)
    {
      if (!file->is_bytecode)
      {
        to_be_compiled.push_back(file);
        config->inputs.erase(std::remove(config->inputs.begin(), config->inputs.end(), file), config->inputs.end());
      }
    }

    // compile all files that arent bytecode
    BSKConfig *compiler_config = new BSKConfig();
    compiler_config->mode = Mode::COMPILE;
    compiler_config->inputs = to_be_compiled;
    compiler_config->out = "temp.out";
    compiler_config->lib = config->lib;

    Compiler compiler(compiler_config);
    compiler.compile();

    config->inputs.push_back(new File("temp.out", true));

    VM vm{};
    vm.exec(config);

    system("rm temp.out");
    return 0;
  } else if (config->mode == Mode::COMPILE) {
    Compiler compiler(config);
    compiler.compile();
  }

  delete config;

  return 0;
}