#include "bitonic_merge.h"

using namespace std;

void bitonic_merge(int **local_array, int num_element, int partner, int direction) {

    double t;

    MPI_Request reqs[2];
    MPI_Status stats[2];

    int *received_array = (int *) calloc(num_element, sizeof(int));

    if (direction == ASCENDING) {

        MPI_Isend(*local_array, num_element, MPI_INT, partner, 0, MPI_COMM_WORLD, &reqs[0]);

        MPI_Irecv(received_array, num_element, MPI_INT, partner, 0, MPI_COMM_WORLD, &reqs[1]);

        /*t = MPI_Wtime();

        MPI_Send(*local_array, num_element, MPI_INT, partner, 0, MPI_COMM_WORLD);

        communication_time = communication_time + (MPI_Wtime() - t);

        t = MPI_Wtime();

        MPI_Recv(received_array, num_element, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        communication_time = communication_time + MPI_Wtime() - t;*/

    } else {

        /* first receive then send */

        MPI_Irecv(received_array, num_element, MPI_INT, partner, 0, MPI_COMM_WORLD, &reqs[1]);

        MPI_Isend(*local_array, num_element, MPI_INT, partner, 0, MPI_COMM_WORLD, &reqs[0]);

        /*t = MPI_Wtime();

        MPI_Recv(received_array, num_element, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        communication_time = communication_time + MPI_Wtime() - t;

        t = MPI_Wtime();

        MPI_Send(*local_array, num_element, MPI_INT, partner, 0, MPI_COMM_WORLD);

        communication_time = communication_time + MPI_Wtime() - t;*/
    }

    t = MPI_Wtime();

    /* we shouldn't modify the local_array until our partner receives it, so we wait */
    MPI_Waitall(2, reqs, stats);

    communication_time = communication_time + MPI_Wtime() - t;

    /* merge the local and the received array.
     * Result on local_array pointer. */
    merge(local_array, received_array, num_element, direction);

    free(received_array);
}

vector<int> bitonic_merge_vector(std::vector<int> local_array, int partner, int direction, double *communi_time) {

    double t;

    int rank;

    int num_element = local_array.size();

    /*MPI_Request reqs[2];
    MPI_Status stats[2];*/

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int cantor_arg1 = (direction == ASCENDING)? rank : partner;
    int cantor_arg2 = (direction == ASCENDING)? partner : rank;

    int send_tag = (1 + (direction == ASCENDING ))  * cantor(cantor_arg1, cantor_arg2);
    int recv_tag = (1 + (direction == DESCENDING )) * cantor(cantor_arg1, cantor_arg2);

    vector<int> received_array;
    received_array.reserve(num_element);

    if (direction == ASCENDING ) {
        /* first send then receive */

        /* MPI_Isend(local_array  , num_element, MPI_INT, partner, send_tag, MPI_COMM_WORLD, &reqs[0]);

         MPI_Irecv(received_array, num_element, MPI_INT, partner, recv_tag, MPI_COMM_WORLD, &reqs[1]);*/

        t = MPI_Wtime();

        MPI_Send(local_array.data(), num_element, MPI_INT, partner, send_tag, MPI_COMM_WORLD);

        *communi_time += (MPI_Wtime() - t);

        t = MPI_Wtime();

        MPI_Recv(received_array.data(), num_element, MPI_INT, partner, recv_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        *communi_time += MPI_Wtime() - t;

    } else {
        /* first receive then send */
        t = MPI_Wtime();

        MPI_Recv(received_array.data(), num_element, MPI_INT, partner, recv_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        *communi_time += MPI_Wtime() - t;

        t = MPI_Wtime();

        MPI_Send(local_array.data(), num_element, MPI_INT, partner, send_tag, MPI_COMM_WORLD);

        *communi_time += MPI_Wtime() - t;

        /*MPI_Irecv(received_array, num_element, MPI_INT, partner, recv_tag, MPI_COMM_WORLD, &reqs[1]);

        MPI_Isend(local_array, num_element, MPI_INT, partner, send_tag, MPI_COMM_WORLD, &reqs[0]);*/
    }

    //t = MPI_Wtime();
    /* we shouldn't modify the local_array until our partner receives it, so we wait */
    //MPI_Waitall(2, reqs, stats);

    //communi_time += MPI_Wtime() - t;

    /* merge the local and the received array.
     * Result on local_array pointer. */

    local_array = merge_vector(local_array, received_array, direction);

    received_array.clear();
    received_array.shrink_to_fit();

    return local_array;
}