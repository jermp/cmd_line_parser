#include <iostream>

#ifdef __GNUC__
#include <cxxabi.h>
#endif

#include "../include/parser.hpp"

std::string demangle(std::string const& name) {
#ifdef __GNUC__
    int status = 0;
    char* tmp = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
    std::string demagled(tmp);
    free(tmp);
    return demagled;
#else
    return name;
#endif
}

template <typename T>
void info(T val) {
    std::cout << "value: " << val << "; type: " << demangle(typeid(val).name()) << std::endl;
}

template <typename T>
void test(cmd_line_parser::parser const& parser, std::string const& value) {
    info(parser.parse<T>(value));
}

int main(int argc, char** argv) {
    cmd_line_parser::parser parser(argc, argv);

    test<char>(parser, "a");
    test<signed char>(parser, "a");
    test<unsigned char>(parser, "a");

    test<short int>(parser, "-2423");
    test<int>(parser, "-93811");
    test<long int>(parser, "-8284239");

    test<unsigned short int>(parser, "1452");
    test<unsigned int>(parser, "1841");
    test<unsigned long int>(parser, "82757188104");

    test<float>(parser, "234.2");
    test<double>(parser, "0.11");
    test<long double>(parser, "-22.1111");

    test<bool>(parser, "true");
    test<bool>(parser, "false");

    // not supported: test<wchar_t>(parser, "c");

    return 0;
}