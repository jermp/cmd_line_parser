#include <iostream>

#include "../include/parser.hpp"

void configure(cmd_line_parser::parser& parser) {
    // for the following two arguments, we do not specify any shorthand,
    // thus they will be *required* arguments
    parser.add("perc",                // name
               "A percentage value."  // description
    );
    parser.add("input_filename", "An input file name.");

    // here, we specify some shorthand for *optional* arguments
    parser.add("output_filename",       // name
               "An output file name.",  // description
               "-o",                    // shorthand
               false                    // not boolean option: expected a value after the shorthand
    );
    parser.add("num_trials", "Number of trials.", "-n", false);

    parser.add("sorted", "Sort output.", "--sort",
               true  // boolean option: a value is not expected after the shorthand
    );
    parser.add("buffered", "Buffer input.", "--buffer"
               // the option is considered boolean by default if we do not
               // specify anything
    );

    parser.add("ram", "Amount of ram to use.", "--ram", false);
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
    auto perc = parser.get<float>("perc");  // deduced type is float
    auto input_filename =                   // deduced type is std::string
        parser.get<std::string>("input_filename");
    auto sorted_output = parser.get<bool>("sorted");     // deduced type is bool
    auto buffered_input = parser.get<bool>("buffered");  // deduced type is bool

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
    } catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}