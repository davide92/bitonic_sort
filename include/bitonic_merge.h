#ifndef BITONIC_SORT_MERGE_SORT_H
#define BITONIC_SORT_MERGE_SORT_H

#include "mpi.h"
#include "utils.h"

void bitonic_merge(int **local_array, int num_element, int partner, int direction, double communi_time);

#endif //BITONIC_SORT_MERGE_SORT_H
