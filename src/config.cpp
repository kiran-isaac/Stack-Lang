#include "config.h"

enum LAST_ARG {
  NONE,
  OUT,
  LIB
};

int verify_filename(char *fname) {
  int len = strlen(fname);
  if (len < 4) {
    return 0;
  }

  File *file = new File();
  file->name = fname;

  
  // Check if the file is a bytecode file
  file->is_bytecode = fname[len - 1] != 'k' || fname[len - 2] != 's' || fname[len - 3] != 'b' || fname[len - 4] != '.';

  // Check if the file exists
  std::ifstream file_stream(fname);
  if (!file_stream.good()) {
    std::cout << "File '" << fname << "' does not exist" << std::endl;
  }

  return 1;
}

BSKConfig *parse_args(int argc, char *argv[]) {
  BSKConfig *config = new BSKConfig();
  config->out = "a.out";
  config->lib = "/usr/lib/bask/stdlib";

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