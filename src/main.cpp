#include <iostream>
#include <stdexcept>
#include <string>
#include <time.h>
#include "utils.h"
#include "input_parser.h"
#include "mpi.h"


using namespace std;

int main(int argc , char** argv) {

    /*Process identifier*/
    int process_id;

    /*Initial array*/
    int* array = nullptr;

    /*Local array of elements for each process*/
    int* local_array;

    /*Number of elements in the array of elements*/
    int num_element;

    /*Number of tasks running*/
    int num_tasks;

    /*Number of processor and number of elements to sort passed as input arguments*/
    int p = 0, q = 0;

    double startTime;
    double endTime ;

    /*Check if enough arguments were passed to the process.*/
    if (argc < 3) {
        /*If not print a warning message with the correct way to use the program and terminate the execution. */
        cout << "Invalid command line argument option! \n" << "Usage: " << argv[0] << " p q where: \n - p is the number of MPI processes to be spawned\n - q the number of elements to sort. \n ";
        exit(ARG_ERROR);
    }

    /* Parse the command line arguments. */
    p = parse_input(argv[1]);
    q = parse_input(argv[2]);

    /* Print input value */
    cout << "The inserted values are p = " << p << " and q = " << q << ".\n";

    /*Initialize MPI processes*/
    MPI_Init(&argc, &argv);

    /*Get number of tasks running*/
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    /*Get the identifier of the process from the MPI daemon*/
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    /*The master process checks that the correct number of processes are working*/
    if (process_id == 0 && num_tasks == 0) {
        cout << "There are not process running.\n" << "The sorting will stop!\n";

        /*Stop the MPI process*/
        MPI_Abort(MPI_COMM_WORLD, NTHREADS_ERROR);
        exit(NTHREADS_ERROR);
    }


    if (process_id == 0) {
        /*Memory allocation for the list of elements to sort*/
        array = (int*) calloc(q, sizeof(int));

        /*Check if allocation succeed*/
        if (array == nullptr) {
            cout << "Process " << process_id << " failed to allocate memory for the elements to sort";

            /*Stop the MPI process*/
            MPI_Abort(MPI_COMM_WORLD, CALLOC_ERROR);
            free(array);
            exit(CALLOC_ERROR);
        }

        populate_array(array, q);

        for (int i = 0; i < q; i++) {
            cout << array[i] << " ";
        }

    }

    //num_element = q / num_tasks;

    //local_array = (int*) calloc(num_element, sizeof(int));

    /*Check if allocation succeed*/
    /*if (array == nullptr) {
        cout << "Process ";
        cout << process_id;
        cout << " failed to allocate memory for the elements to sort";*/

        /*Stop the MPI process*/
        /*MPI_Abort(MPI_COMM_WORLD, CALLOC_ERROR);
        free(local_array);
        free(array);
        exit(CALLOC_ERROR);
    }

    MPI_Scatter(array, num_element, MPI_INT, local_array, num_element, MPI_INT, 0, MPI_COMM_WORLD);


    free(local_array);*/
    free(array);

}