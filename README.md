Command Line Parser for C++17
----

This is a simgle-header command line parser for C++17.

It offers, basically, the same funcionalities of other popular libraries, such as [cxxopts](https://github.com/jarro2783/cxxopts),
[cmdline](https://github.com/tanakh/cmdline) and
[argparse](https://github.com/hbristow/argparse),
but relies on the powerful `if  constexpr` construct of C++17 instead of dynamic casting.
This results in a very compact code (~140 sloc).

### Integration

Just add the file `include/parser.hpp` to your own project.

Or if you use git:

	$ git add submodule https://github.com/jermp/cmd_line_parser.git

### Example

```C++
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

```

To compile and run the example, do as follows.

	$ mkdir -p build; cd build
	$ cmake ..
	$ make
	$ ./example

Also run `./test_parse` for some testing.


### Interface

```C++
    parser(int argc, char** argv);                   // constructor

    bool parse();                                    // parse command line; return false on failure

    void help() const;                               // print help message

    bool add(std::string const& name,                // add argument (if shorthand is *not* empty, 
             std::string const& descr,               // then the argument is intended to be optional);
             std::string const& shorthand = empty);  // return false if an argument with the same name already exists

    template <typename T>
    T get(std::string const& name) const;            // get a variable of type T by name
```
