#include "vm/baskvm.h"
#include "compiler/compiler.h"
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;

int main(int argc, char *argv[]) {
    options_description desc("Allowed options");
	string out{};
    string lib{};
	vector<string> inputs{};

    desc.add_options()
		("help", "produce help message")
        ("compile,c", "mode compile")
        ("run,r", "mode compile")
        ("standalone,s", "create standalone executable")
		("out,o", value<string>(&out), "output file name")
        ("std", value<string>(&lib)->default_value("/usr/lib/bask/stdlib"), "standard library location")
		("inputs,i", value<vector<string>>(&inputs)->multitoken(), "input files");

    variables_map vm{};
	store(parse_command_line(argc, argv, desc), vm);
    notify(vm);
    if (vm.size() == 0 || vm.count("help"))
	{
		cout << desc << "\n";
		return 1;
	}

    if (vm.count("compile") != 0) {
        Compiler compiler(out, inputs);
        compiler.compile();
    } else if (vm.count("standalone") != 0) {
        Compiler compiler("a.out", inputs);
        compiler.stdlib = lib;
        compiler.compile();
        char dest[10000];
        memset(dest,0,sizeof(dest));
        if (readlink("/proc/self/exe", dest, 10000) == -1) {
            perror("Cannot readlink");
        } else {
            printf("%s\n", dest);
        }

        if (!filesystem::exists(lib)) {
            FAIL << "Cannot find library file: " << lib;
        }

        system("ld -o binary.o -r -b binary a.out");
        system(("g++ -o " + out + " binary.o " + string(filesystem::path(dest).parent_path()) + string("/bask_portable")).c_str());
        system("rm *.o");
        system("rm a.out");

        return 0;
    } else {
        Compiler compiler(out, inputs);
        compiler.compile();
    }
    return 0;
}