#include <iostream>
#include <stdexcept>
#include <string>
#include <time.h>
#include "utils.h"
#include "input_parser.h"

int main(int argc , char** argv) {

    /*Processor ID*/
    int processID;

    /*Local array of elements for each processor*/
    int array;

    /*Number of elements in the array*/
    int n_array;

    /*Number of tasks running*/
    int num_tasks;

    /*Number of processor and number of elements to sort passed as input arguments*/
    int p = 0, q = 0;

    double startTime;
    double endTime ;

    /*Check if enough arguments were passed to the process.*/
    if (argc < 3) {
        /*If not print a warning message with the correct way to use the program and terminate the execution. */
        std::cout << "Invalid command line argument option! \n";
        std::cout << "Usage: ";
        std::cout << argv[0];
        std::cout << " p q where: \n - p is the number of MPI processes to be spawned\n - q the number of elements to sort. \n ";
        exit(ARG_ERROR);
    }

    /* Parse the command line arguments. */
    p = parse_input(argv[1]);
    q = parse_input(argv[2]);

    /* Print input value */
    std::cout << "The inserted values are p = ";
    std::cout << p;
    std::cout << " and q = ";
    std::cout << q;
    std::cout << ".\n";

}