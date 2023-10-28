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
  std::cout << "  -std\tSpecify the standard library file";
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
      // Check if the file is a directory
      if (std::filesystem::is_directory(argv[i])) {
        for (const auto &entry : std::filesystem::directory_iterator(argv[i])) {
          std::string fname = entry.path().string();
          verify_filename((char *)fname.c_str());

          // Check if the file is bytecode or not
          // if it has the extension .sl, it is not bytecode
          bool is_bytecode = true;
          if (fname.substr(fname.find_last_of(".") + 1) == "sl") {
            is_bytecode = false;
          }


          // copy string
          char *cstr = new char[fname.length() + 1];
          strcpy(cstr, fname.c_str());
          config->inputs.push_back(new File(cstr, is_bytecode));
        }
      } else {
        verify_filename(argv[i]);

        // Check if the file is bytecode or not
        // if it has the extension .sl, it is not bytecode
        bool is_bytecode = true;
        std::string fname = std::string(argv[i]);
        if (fname.substr(fname.find_last_of(".") + 1) == "sl") {
          is_bytecode = false;
        }

        config->inputs.push_back(new File(argv[i], is_bytecode));
      }
    }
  }

  return config;
}