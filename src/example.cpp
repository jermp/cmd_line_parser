#include <iostream>

#include "../include/parser.hpp"

void configure(cmd_line_parser::parser& parser) {
    parser.add("perc",                 // name
               "A percentage value.",  // description
               "-p",                   // shorthand
               true,                   // required argument
               false                   // not boolean option (default is false)
    );
    parser.add("input_filename", "An input file name.", "-i", true);

    parser.add("output_filename",       // name
               "An output file name.",  // description
               "-o",                    // shorthand
               false, false);
    parser.add("num_trials", "Number of trials.", "-n", false, false);

    parser.add("sorted", "Sort output.", "--sort", false,
               true  // boolean option: a value is not expected after the shorthand
    );
    parser.add("buffered", "Buffer input.", "--buffer", false, true);

    parser.add("ram", "Amount of ram to use.", "--ram", false, false);
}

int main(int argc, char** argv) {
    // declare the parser
    cmd_line_parser::parser parser(argc, argv);
    // configure the parser
    configure(parser);

    // parse command line and return on failure
    bool success = parser.parse();
    if (!success) return 1;

    // now get some variables
    auto perc = parser.get<float>("perc");                            // deduced type is float
    auto input_filename = parser.get<std::string>("input_filename");  // deduced type is std::string
    auto sorted_output = parser.get<bool>("sorted");                  // deduced type is bool
    auto buffered_input = parser.get<bool>("buffered");               // deduced type is bool

    size_t ram = 999;  // some default value
    if (parser.parsed("ram")) {
        ram = parser.get<size_t>("ram");  // deduced type is size_t
    }

    std::cout << "perc: " << perc << std::endl;
    std::cout << "input_filename: " << input_filename << std::endl;
    std::cout << "sorted_output: " << sorted_output << std::endl;
    std::cout << "buffered_input: " << buffered_input << std::endl;
    std::cout << "ram: " << ram << std::endl;

    try {
        auto val = parser.get<int>("bar");  // fail: no name 'bar' was specified
        (void)val;                          // shut up, compiler!
    } catch (std::runtime_error const& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}