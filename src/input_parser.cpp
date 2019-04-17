#include <iostream>
#include "input_parser.h"

using namespace std;

int parse_input(char* argv) {
    int i = 0;

    string arg = argv;
    try {
        size_t pos;
        i = stoi(arg, &pos);
        if (pos < arg.size()) {
            cerr << "Trailing characters after number: " << arg << '\n';
        }
    } catch (std::invalid_argument const &ex) {
        cerr << "Invalid number: " << arg << '\n';
    } catch (std::out_of_range const &ex) {
        cerr << "Number out of range: " << arg << '\n';
    }

    return i;
}


