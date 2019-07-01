#include "input_parser.h"

using namespace std;

void parse_command_line(int argc, char **argv, input *input_data) {

    /*Check if enough arguments were passed to the process.*/
    if (argc <= 3) {
        /*If not print a warning message with the correct way to use the program and terminate the execution. */
        cout << "Invalid command line argument option! \n" << "Usage: " << argv[0] << " -p -n where: \n - p is the number of MPI processes to be spawned\n - n the number of elements to sort. \n ";
        exit(ARG_ERROR);
    }

    for ( int i = 1; i < argc; i++ )
    {

        if (strcmp(argv[i], "-p") == 0) { // number of processors
            input_data->p = atoi(argv[++i]);
            continue;
        }

        if (strcmp(argv[i], "-n") == 0) { //number of elements to sort
            input_data->n = atoi(argv[++i]);
            continue;
        }
    }

    /*string arg = argv;
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

    return i;*/
}