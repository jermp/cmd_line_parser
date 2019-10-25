#include <iostream>

#include "parser.hpp"

void configure(cmd_line_parser::parser& parser) {
    // for the following two arguments, we do not specify any shorthand,
    // thus they will be considered as *required* arguments
    parser.add("perc",                // name
               "A percentage value."  // description
    );
    parser.add("input_filename", "An input file name.");

    // here, we specify some shorthand for *optional* arguments
    parser.add("output_filename",       // name
               "An output file name.",  // description
               "-o"                     // shorthand
    );
    parser.add("num_trials", "Number of trials.", "-n");
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

    std::cout << "perc: " << perc << std::endl;
    std::cout << "input_filename: " << input_filename << std::endl;

    try {
        auto val = parser.get<int>("bar");  // fail: no name 'bar' was specified
        (void)val;                          // shut up, compiler!
    } catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}