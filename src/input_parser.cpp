#include <iostream>
//#include "input_parser.h"

int parse_input(char* argv) {
    int i = 0;

    std::string arg = argv;
    try {
        std::size_t pos;
        i = std::stoi(arg, &pos);
        if (pos < arg.size()) {
            std::cerr << "Trailing characters after number: " << arg << '\n';
        }
    } catch (std::invalid_argument const &ex) {
        std::cerr << "Invalid number: " << arg << '\n';
    } catch (std::out_of_range const &ex) {
        std::cerr << "Number out of range: " << arg << '\n';
    }

    return i;
}


