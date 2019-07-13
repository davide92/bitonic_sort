#include <chrono>
#include "utils.h"
#include "input_parser.h"
#include "bitonic_merge.h"
#include "mpi.h"

using namespace std;

/*Process identifier*/
int process_id;

/*Number of elements of the initial array given by hte user*/
int n = 0;

/*Number of elements of the initial array after padding*/
int initial_array_size = 0;

/*Local array of elements for each process*/
int *local_array;     

/*Number of tasks running*/
int num_tasks;

/*Total communication time*/
double communication_time;

nt main(int argc , char **argv) {

    /*Process identifier*/
    int process_id;

    /*Initial array*/
    int *initial_array = NULL;

    /*Number of processor and number of elements to sort passed as input arguments*/
    int p = 0;

    communication_time = 0;

    input input_data;

    /*Starting time to sort with bitonic sort*/
    double t_start = 0;

    /*Stopping time to sort with bitonic sort*/
    double t_stop = 0;

    /*Latest time MPI_Wtime is called*/
    double t;

    /*Number of elements in the array of elements*/
    int num_element;

    parse_command_line(argc, argv, &input_data);

    /*Initialize MPI processes*/
    MPI_Init(&argc, &argv);
    /*Get number of tasks running*/
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    /*Get the identifier of the process from the MPI daemon*/
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    if (argc > 3) {

        p = input_data.p;

    } else {

        p = num_tasks;

    }
    
    n = input_data.n;

    /*The master process checks that the correct number of processes are working*/
    if (process_id == MASTER_PROCESS) {

        /* Print input value */
        cout << "The inserted values are p = " << p << " and n = " << n << ".\n";
        cout << "Number of task running: " << num_tasks << ".\n";

        if (num_tasks != p) {

            cout << "There are not process running.\n" << "The sorting will stop!\n";

            /*Stop the MPI process*/
            MPI_Abort(MPI_COMM_WORLD, NTHREADS_ERROR);
            exit(NTHREADS_ERROR);
        }
    }

    initial_array_size = padding(n, num_tasks);


    if (process_id == MASTER_PROCESS) {

        /*Memory allocation for the list of elements to sort*/
        initial_array = (int *) calloc(initial_array_size, sizeof(int));

        /*Check if allocation succeed*/
        if (initial_array == NULL) {

            cout << "Process " << process_id << " failed to allocate memory for the elements to sort.\n";

            /*Stop the MPI process*/
            MPI_Abort(MPI_COMM_WORLD, CALLOC_ERROR);
            free(initial_array);
            exit(CALLOC_ERROR);
        }

        populate_array(initial_array, n, initial_array_size);

        cout << "Initial array size: " << initial_array_size << "\n";

        t_start = MPI_Wtime();

    }

    num_element = initial_array_size / num_tasks;

    //cout << "Number of elements for each task: " << num_element << "\n";

    local_array = (int *) calloc(num_element, sizeof(int));

    /*Check if allocation succeed*/
    if (local_array == NULL) {

        cout << "Process " << process_id << " failed to allocate memory for the elements to sort.\n";

        /*Stop the MPI process*/
        MPI_Abort(MPI_COMM_WORLD, CALLOC_ERROR);
        free(local_array);
        free(initial_array);
        exit(CALLOC_ERROR);
    }

    t = MPI_Wtime();

    /* Sends data from master process to the slave processes */
    /* MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
     */
    MPI_Scatter(initial_array, num_element, MPI_INT, local_array, num_element, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

    communication_time = communication_time + MPI_Wtime() - t;
    
    //cout << "Communication operation took: " << communication_time << " seconds. \n";

    /*Sort the local elements*/
    qsort(local_array, num_element, sizeof(int), compare);

    t = MPI_Wtime();

    /*Processes synchronization*/
    MPI_Barrier(MPI_COMM_WORLD);

    communication_time = communication_time + MPI_Wtime() - t;

    free(initial_array);

    for (int i = 0; i < log2(num_tasks); i++) {
    
        for (int j = i; j >= 0; j--) {

            /* The next process we will be paired with is the one
             * whose j-th bit is the complement of ours. So we toggle
             * our j-th bit using the XOR operation to find our partner
            */
            int partner = process_id ^ (1 << j);

            /* Check if we have to keep the smallest out of our elements
             * and our partner's or the largest.
             * We do this by checking the (i+1) bit of our id and comparing it
             * to the #j bit. If they are equal then we keep the small
             * elements , if not the larger ones.
            */

            if (((process_id >> (i + 1)) & 1) == ((process_id >> j) & 1)) {

                bitonic_merge(&local_array, num_element, partner, ASCENDING);

            } else {

                bitonic_merge(&local_array, num_element, partner, DESCENDING);

            }
        }
    }

    t = MPI_Wtime();

    /*Processes synchronization*/
    MPI_Barrier(MPI_COMM_WORLD);

    communication_time = communication_time + MPI_Wtime() - t;

    int *sorted_array = NULL;

    if (process_id == MASTER_PROCESS) {

        sorted_array = (int *) calloc(initial_array_size, sizeof(int));

        /*Check if allocation succeed*/
        if (sorted_array == NULL) {
            cout << "Process " << process_id << " failed to allocate memory for the elements to sort.\n";

            /*Stop the MPI process*/
            MPI_Abort(MPI_COMM_WORLD, CALLOC_ERROR);
            free(sorted_array);
            free(local_array);
            exit(CALLOC_ERROR);
        }

    }

    t = MPI_Wtime();

    MPI_Gather(local_array, num_element, MPI_INT, sorted_array, num_element, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

    communication_time = communication_time + MPI_Wtime() - t;

    if (process_id == MASTER_PROCESS && sorted_array != NULL) {
        cout<<"Enter sorting verification.\n";
        t_stop = MPI_Wtime();
        double total_time = t_stop - t_start;
        int sorted = correct_sorted(sorted_array, n);
        printf("Array is %s sort.\n", (sorted) ? "correctly" : "not correctly");
        cout << "Communication time: " << communication_time << " seconds.\n";
        cout << "Total time to sort the array: " << total_time << " seconds.\n";
        //cout << "The sorted array:\n";
        //print_array(sorted_array, n);
    }

    free(local_array);
    free(sorted_array);

    MPI_Finalize();

}