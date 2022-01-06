#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>

class VMErrorLogger : public std::basic_ostringstream<char> {
    public:
        ~VMErrorLogger() {
            fprintf(stderr, "Fatal runtime error: %s\n", str().c_str());
            exit(1);
        }
};

#define FAIL VMErrorLogger()

#endif