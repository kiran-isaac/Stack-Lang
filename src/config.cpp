#include "include/config.h"

enum LAST_ARG {
  NONE,
  OUT,
  LIB
};

void verify_filename(char *fname) {
  std::ifstream file_stream(fname);
  if (!file_stream.good()) {
    std::cout << "File '" << fname << "' does not exist" << std::endl;
    exit(1);
  }
}

void help() {
  std::cout << "Usage: slang [mode] [options] <file>" << std::endl;
  std::cout << "Modes:" << std::endl;
  std::cout << "  compile\tCompile the file" << std::endl;
  std::cout << "  run\t\tRun the file" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  -o\tSpecify the output file" << std::endl;
  std::cout << "  -std\tSpecify the standard library file" << std::endl;
  std::cout << std::endl;
}

BSKConfig *parse_args(int argc, char *argv[]) {
  BSKConfig *config = new BSKConfig();
  config->inputs = std::vector<File *>();
  config->out = "a.out";
  config->lib = "/usr/lib/slang/stdlib";

  if (argc < 2) {
    help();
    exit(0);
  }

  switch (argv[1][0]) {
    case 'c':
      config->mode = Mode::COMPILE;
      break;
    case 'r':
      config->mode = Mode::RUN;
      break;
    case 'h':
      help();
      exit(0);
    default:
      config->mode = Mode::RUN;
      break;
  }

  LAST_ARG last = NONE;
  for (int i = 2; i < argc; i++) {
    if (last == LAST_ARG::OUT) {
      config->out = string(argv[i]);
      last = NONE;
    } else if (last == LAST_ARG::LIB) {
      config->lib = string(argv[i]);
      verify_filename(argv[i]);
      last = NONE;
    } else if (strcmp(argv[i], "-o") == 0) {
      last = LAST_ARG::OUT;
    } else if (strcmp(argv[i], "-std") == 0) {
      last = LAST_ARG::LIB;
    } else {
      verify_filename(argv[i]);
      char *name = argv[i];
      verify_filename(argv[i]);
      bool is_bytecode = argv[i][strlen(argv[i]) - 1] != 'l' || argv[i][strlen(argv[i]) - 2] != 's' || argv[i][strlen(argv[i]) - 3] != '.';
      File *file = new File(name, is_bytecode);

      config->inputs.push_back(file);
    }
  }

  return config;
}