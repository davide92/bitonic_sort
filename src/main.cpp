#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/time.h>
#include "utils.h"

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
    int p, q;

    double startTime;
    double endTime ;

    /*Check if enough arguments were passed to the process.*/
    if (argc < 3) {
        /*If not print a warning message with the correct way to use*/
        /*the program and terminate the execution. */
        printf("Invalid command line argument option! \n");
        printf("Usage : %s p q where p is the number of MPI processes to be spawned and q the number of elements to sort. \n ", argv[0]);
        exit(ARG_ERROR);
    }

    /* Parse the command line arguments. */
    std::string arg1 = argv[1];
    try {
        std::size_t pos;
        p = std::stoi(arg1, &pos);
        if (pos < arg1.size()) {
            std::cerr << "Trailing characters after number: " << arg1 << '\n';
        }
    } catch (std::invalid_argument const &ex) {
        std::cerr << "Invalid number: " << arg1 << '\n';
    } catch (std::out_of_range const &ex) {
        std::cerr << "Number out of range: " << arg1 << '\n';
    }

    std::string arg2 = argv[2];
    try {
        std::size_t pos;
        q = std::stoi(arg2, &pos);
        if (pos < arg2.size()) {
            std::cerr << "Trailing characters after number: " << arg2 << '\n';
        }
    } catch (std::invalid_argument const &ex) {
        std::cerr << "Invalid number: " << arg2 << '\n';
    } catch (std::out_of_range const &ex) {
        std::cerr << "Number out of range: " << arg2 << '\n';
    }

}