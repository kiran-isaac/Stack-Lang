#ifndef F7D99EE6_79D6_49C4_BABE_ACC73E1A7B3A
#define F7D99EE6_79D6_49C4_BABE_ACC73E1A7B3A

#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>

using std::string;

enum Mode {
  COMPILE,
  RUN
};

typedef struct File {
  File(const char *name, bool is_bytecode) {
    this->name = name;
    this->is_bytecode = is_bytecode;
  }
  const char *name;
  bool is_bytecode;
} File;

typedef class BSKConfig {
public:
  std::vector<File*> inputs;
  string out;
  string lib;
  Mode mode;

  ~BSKConfig() {
    for (File *file : inputs) {
      delete file;
    }
  }
} BSKConfig;

BSKConfig *parse_args(int argc, char *argv[]);

#endif /* F7D99EE6_79D6_49C4_BABE_ACC73E1A7B3A */
