#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>

class CompilerErrorLogger : public std::basic_ostringstream<char> {
    public:
        ~CompilerErrorLogger() {
            fprintf(stderr, "Fatal compiler error: %s\n", str().c_str());
            exit(1);
        }
};

#define FAIL CompilerErrorLogger()

#endif