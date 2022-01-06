#ifndef B1C2A010_ABD3_4736_BB36_4C94636703F1
#define B1C2A010_ABD3_4736_BB36_4C94636703F1

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

#endif /* B1C2A010_ABD3_4736_BB36_4C94636703F1 */
