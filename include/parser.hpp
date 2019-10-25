#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <type_traits>
#include <cassert>

namespace cmd_line_parser {

struct parser {
    inline static const std::string empty = "";

    parser(int argc, char** argv)
        : m_argc(argc)
        , m_argv(argv)
        , m_required(0) {}

    struct cmd {
        std::string shorthand, value, descr;
    };

    bool parse() {
        if (size_t(m_argc - 1) < m_required) {
            std::cerr << "== error: expected " << m_required
                      << " mandatory arguments (got only " << size_t(m_argc - 1)
                      << ")" << std::endl;
            return abort();
        }

        size_t k = 0;
        for (int i = 1; i != m_argc; ++i, ++k) {
            std::string parsed(m_argv[i]);
            if (parsed == "-h" or parsed == "--help") return abort();
            size_t id = k;
            if (id >= m_required) {
                auto it = m_shorthands.find(parsed);
                if (it == m_shorthands.end()) {
                    std::cerr
                        << "== error: shorthand '" + parsed + "' not found"
                        << std::endl;
                    return abort();
                }
                id = (*it).second;
                parsed = m_argv[++i];
            }
            assert(id < m_names.size());
            auto const& name = m_names[id];
            auto& c = m_cmds[name];
            c.value = parsed;
        }

        return true;
    }

    void help() const {
        std::cerr << "usage: " << m_argv[0] << " [-h,--help]";

        auto print = [this](bool with_description) {
            for (size_t i = 0; i != m_names.size(); ++i) {
                auto const& c = m_cmds.at(m_names[i]);
                if (i < m_required) {
                    std::cerr << " " << m_names[i];
                } else {
                    std::cerr << " [" << c.shorthand << " " << m_names[i]
                              << "]";
                }
                if (with_description) std::cerr << "\n\t" << c.descr << "\n";
            }
        };

        print(false);
        std::cerr << "\n\n";
        print(true);
        std::cerr << " [-h,--help]\n\tPrint this help text and silently exits."
                  << std::endl;
    }

    bool add(std::string const& name, std::string const& descr,
             std::string const& shorthand = empty) {
        bool ret = m_cmds.emplace(name, cmd{shorthand, empty, descr}).second;
        if (ret) {
            m_names.push_back(name);
            if (shorthand != "") {
                m_shorthands.emplace(shorthand, m_names.size() - 1);
            } else {
                m_required += 1;
            }
        }
        return ret;
    }

    template <typename T>
    T get(std::string const& name) const {
        auto it = m_cmds.find(name);
        if (it == m_cmds.end()) {
            throw std::runtime_error("error: '" + name + "' not found");
        }
        auto const& value = (*it).second.value;
        if (value == empty) {
            std::cerr << "== warning: no value was given for '" + name + "' =="
                      << std::endl;
        }
        return parse<T>(value);
    }

    template <typename T>
    T parse(std::string const& value) const {
        if constexpr (std::is_same<T, std::string>::value) {
            return value;
        } else if constexpr (std::is_same<T, char>::value or
                             std::is_same<T, signed char>::value or
                             std::is_same<T, unsigned char>::value) {
            return value.front();
        } else if constexpr (std::is_same<T, unsigned int>::value or
                             std::is_same<T, int>::value or
                             std::is_same<T, unsigned short int>::value or
                             std::is_same<T, short int>::value) {
            return std::atoi(value.c_str());
        } else if constexpr (std::is_same<T, unsigned long int>::value or
                             std::is_same<T, long int>::value or
                             std::is_same<T, unsigned long long int>::value or
                             std::is_same<T, long long int>::value) {
            return std::atoll(value.c_str());
        } else if constexpr (std::is_same<T, float>::value or
                             std::is_same<T, double>::value or
                             std::is_same<T, long double>::value) {
            return std::atof(value.c_str());
        } else if constexpr (std::is_same<T, bool>::value) {
            std::istringstream stream(value);
            bool ret;
            if (value == "true" or value == "false") {
                stream >> std::boolalpha >> ret;
            } else {
                stream >> std::noboolalpha >> ret;
            }
            return ret;
        }
        assert(false);
        __builtin_unreachable();
    }

private:
    int m_argc;
    char** m_argv;
    size_t m_required;
    std::unordered_map<std::string, cmd> m_cmds;
    std::unordered_map<std::string, int> m_shorthands;
    std::vector<std::string> m_names;

    bool abort() const {
        help();
        return false;
    }
};

}  // namespace cmd_line_parser