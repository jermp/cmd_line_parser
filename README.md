[![CodeQL](https://github.com/jermp/cmd_line_parser/actions/workflows/codeql.yml/badge.svg)](https://github.com/jermp/cmd_line_parser/actions/workflows/codeql.yml)

Command Line Parser for C++17
============================

This is a single-header command line parser for C++17.

It offers, basically, the same funcionalities as other popular libraries, such as [cxxopts](https://github.com/jarro2783/cxxopts),
[cmdline](https://github.com/tanakh/cmdline) and
[argparse](https://github.com/hbristow/argparse),
but relies on the powerful `if constexpr` construct of C++17 instead of dynamic casting.
This results in a very compact code (~150 sloc).

### Integration

Just add the file `include/parser.hpp` to your own project.

Or if you use git:

	git submodule add https://github.com/jermp/cmd_line_parser.git

### Example

```C++
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
    } catch (std::runtime_error const& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
```

To compile and run the example, do as follows.

	cd src
	mkdir -p build
	cd build
	cmake ..
	make
	./example

Also run `./test_parse` for some testing.


### Interface

```C++
/* Constructor. */
parser(int argc, char** argv);

/* Parse command line; return false on failure. */
bool parse();

/* Print help message. */
void help() const;

/* Add an argument with a name, a description, and a shorthand.
   Return false if an argument with the same name already exists.
   The last two boolean parameters are used to specify if the argument
   is to be required and to be considered a boolean argument (no value after
   the shorthand). */
 bool add(std::string const& name, std::string const& descr,
          std::string const& shorthand,
          bool is_required, bool is_boolean = false);

/* Return true is an option with the given name was parsed; false otherwise. */
bool parsed(std::string const& name) const;

/* Get a variable of type T by name.
   It throws an exception if an argument with the specified name
   is not found. */
template <typename T>
T get(std::string const& name) const;
```
