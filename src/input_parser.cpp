#include "input_parser.h"

using namespace std;

void parse_command_line(int argc, char **argv, input *input_data) {

    /*Check if enough arguments were passed to the process.*/
    if (argc < 3) {

        /*If not print a warning message with the correct way to use the program and terminate the execution. */
        cout << "Invalid command line argument option! \n" << "Usage:\n" << argv[0] << " -p -n or " << argv[0] << " -n where: \n - p is the number of MPI processes to be spawned \n - n the number of elements to sort. \n ";
        exit(ARG_ERROR);

    } else if(argc == 3) {
        
        int i = 1;

        if (strcmp(argv[i], "-p") == 0) { // number of processors

            /*If not print a warning message with the correct way to use the program and terminate the execution. */
            cout << "Invalid command line argument option! \n" << "Usage:\n" << argv[0] << " -p -n or " << argv[0] << " -n where: \n - p is the number of MPI processes to be spawned \n - n the number of elements to sort. \n ";
            exit(ARG_ERROR);

        } else if (strcmp(argv[i], "-n") == 0) { //number of elements to sort
            
            input_data->n = atoi(argv[++i]);
        }
    } else {

        for (int i = 1; i < argc; i++) {

            if (strcmp(argv[i], "-p") == 0) { // number of processors
                
                input_data->p = atoi(argv[++i]);
                continue;
            }

            if (strcmp(argv[i], "-n") == 0) { //number of elements to sort
                
                input_data->n = atoi(argv[++i]);
                continue;
            }
        }        
    }
}