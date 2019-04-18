#include <chrono>
#include "utils.h"
#include "input_parser.h"
#include "bitonic_merge.h"
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

    /*Total comunication time*/
    double communi_time = 0;

    /*Total time to sort with bitonic sort*/
    double total_time = 0;

    /*Latest time MPI_Wtime is called*/
    double t;

    /*Check if enough arguments were passed to the process.*/
    if (argc < 3) {
        /*If not print a warning message with the correct way to use the program and terminate the execution. */
        cout << "Invalid command line argument option! \n" << "Usage: " << argv[0] << " p q where: \n - p is the number of MPI processes to be spawned\n - q the number of elements to sort. \n ";
        exit(ARG_ERROR);
    }

    /*Initialize MPI processes*/
    MPI_Init(&argc, &argv);
    /*Get number of tasks running*/
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    /*Get the identifier of the process from the MPI daemon*/
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    /* Parse the command line arguments. */
    p = parse_input(argv[1]);
    q = parse_input(argv[2]);

    /* Print input value */
    cout << "The inserted values are p = " << p << " and q = " << q << ".\n";

    cout << "Number of task running: " << num_tasks << ".\n";

    /*The master process checks that the correct number of processes are working*/
    if (process_id == MASTER_PROCESS && num_tasks < 1) {
        cout << "There are not process running.\n" << "The sorting will stop!\n";

        /*Stop the MPI process*/
        MPI_Abort(MPI_COMM_WORLD, NTHREADS_ERROR);
        exit(NTHREADS_ERROR);
    }


    if (process_id == MASTER_PROCESS) {
        /*Memory allocation for the list of elements to sort*/
        array = (int*) calloc(q, sizeof(int));

        /*Check if allocation succeed*/
        if (array == nullptr) {
            cout << "Process " << process_id << " failed to allocate memory for the elements to sort.";

            /*Stop the MPI process*/
            MPI_Abort(MPI_COMM_WORLD, CALLOC_ERROR);
            free(array);
            exit(CALLOC_ERROR);
        }

        populate_array(array, q);

        total_time = MPI_Wtime();

        cout << "Initial array content:\n";
        print_array(array, q);

    }

    num_element = q / num_tasks;

    local_array = (int*) calloc(num_element, sizeof(int));

    /*Check if allocation succeed*/
    if (local_array == nullptr) {
        cout << "Process " << process_id << " failed to allocate memory for the elements to sort.";

        /*Stop the MPI process*/
        MPI_Abort(MPI_COMM_WORLD, CALLOC_ERROR);
        free(local_array);
        free(array);
        exit(CALLOC_ERROR);
    }

    //auto start_time = chrono::high_resolution_clock::now();

    t = MPI_Wtime();
    /* Sends data from master process to the slave processes */
    /* MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
     */
    MPI_Scatter(array, num_element, MPI_INT, local_array, num_element, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

    communi_time += MPI_Wtime() - t;
    //auto end_time = chrono::high_resolution_clock::now();

    //auto time_spent = static_cast<chrono::duration<double>>(end_time - start_time);

    //total_time += time_spent.count();

    //cout << "Communication operation took: " << time_spent.count() << " seconds.";

    cout << "Communication operation took: " << communi_time << " seconds.";

    /*Sort the local elements*/
    qsort(local_array, num_element, sizeof(int), compare);

    t = MPI_Wtime();

    /*Processes synchronization*/
    MPI_Barrier(MPI_COMM_WORLD);

    communi_time += MPI_Wtime() - t;

    for (int i = 0; i < log2(num_tasks); i++) {
        for (int j = i; j >= 0 ; j--) {
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
            if ((( process_id >> (i + 1)) & 1 ) == ((process_id >> j) & 1)) {
                bitonic_merge(&local_array, num_element, partner, ASCENDING, communi_time);
            } else {
                bitonic_merge(&local_array, num_element, partner, DESCENDING, communi_time);
            }
        }
    }

    //start_time = chrono::high_resolution_clock::now();

    t = MPI_Wtime();

    /*Processes synchronization*/
    MPI_Barrier(MPI_COMM_WORLD);

    communi_time += MPI_Wtime() - t;

    //end_time = chrono::high_resolution_clock::now();

    //time_spent = static_cast<chrono::duration<double>>(end_time - start_time);

    //total_time += time_spent.count();

    //cout << "Bitonic merge took: " << time_spent.count() << " seconds.";

    int* sorted_array = nullptr;

    if (process_id == MASTER_PROCESS) {
        sorted_array = (int*) calloc(q, sizeof(int));
    }

    //start_time = chrono::high_resolution_clock::now();

    t = MPI_Wtime();

    MPI_Gather(local_array, num_element, MPI_INT, sorted_array, num_element, MPI_INT, MASTER_PROCESS, MPI_COMM_WORLD);

    communi_time += MPI_Wtime() - t;

    //end_time = chrono::high_resolution_clock::now();

    //time_spent = static_cast<chrono::duration<double>>(end_time - start_time);

    //total_time += time_spent.count();

    //cout << "Final sorted array took: " << time_spent.count() << " seconds.";


    if (process_id == MASTER_PROCESS) {
        total_time = MPI_Wtime() - total_time;
        int sorted = correct_sorted(sorted_array, q);
        printf("Array is %s sort.\n", (sorted) ? "correctly" : "not correctly");
        cout << "Communication time: " << communi_time << " seconds.\n";
        cout << "Total time to sort the array: " << total_time << " seconds.\n";
        cout << "The sorted array:\n";
        print_array(sorted_array, q);
    }



    free(local_array);
    free(array);
    free(sorted_array);

    MPI_Finalize();

}