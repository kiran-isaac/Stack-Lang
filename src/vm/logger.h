#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>

class ErrorLogger : public std::basic_ostringstream<char> {
    public:
        ~ErrorLogger() {
            fprintf(stderr, "Fatal error: %s\n", str().c_str());
            exit(1);
        }
};

#define FAIL ErrorLogger()

#endif