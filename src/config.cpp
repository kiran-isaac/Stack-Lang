#include "config.h"

enum LAST_ARG {
  NONE,
  OUT,
  LIB
};

void verify_filename(char *fname) {
  int len = strlen(fname);

  // File *file = new File();
  // file->name = fname;
  
  // // Check if the file is a bytecode file
  // file->is_bytecode = fname[len - 1] != 'l' || fname[len - 2] != 's' || fname[len - 3] != '.';

  // Check if the file exists
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
  std::cout << "  standalone\tCompile and link the file to a minimal version of the\n\r\t\t interpreter to create a standalone executable" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  -c\tCompile the file" << std::endl;
  std::cout << "  -r\tRun the file" << std::endl;
  std::cout << "  -s\tCompile and link the file to create a standalone executable" << std::endl;
  std::cout << "  -o\tSpecify the output file" << std::endl;
  std::cout << "  -std\tSpecify the standard library file" << std::endl;
  std::cout << "  -h\tDisplay this help message" << std::endl;
  std::cout << std::endl;
}

BSKConfig *parse_args(int argc, char *argv[]) {
  BSKConfig *config = new BSKConfig();
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
    case 's':
      config->mode = Mode::STANDALONE;
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
      config->inputs.push_back(string(argv[i]));
    }
  }

  return config;
}