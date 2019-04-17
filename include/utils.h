#ifndef BITONIC_SORT_UTILS_H
#define BITONIC_SORT_UTILS_H

#include <random>
#include <cmath>

#define MASTER_PROCESS 0

#define ASCENDING 1
#define DESCENDING -1

// Define enumerated types for errors started from 3
enum {
    ARG_ERROR = 3,
    CALLOC_ERROR,
    NTHREADS_ERROR
} errors;

void populate_array(int* array, int num_element);

int compare(const void * a, const void * b);

/**
 * Cantor pairing function
 * @param a First process
 * @param b Second process
 * @return Number that encode the two points
 */
int cantor(int a, int b);

/**
 * Inverting the Cantor pairing function
 * @param z Number that encode the two process
 * @param res Array where the mapped processes are stored
 */
void inverse_cantor(int z, int res[2]);

/**
 * Merge function
 * @param a The pointer to the first array
 * @param b The pointer to the second array
 * @param num_element Number of element to merge
 * @param direction Merge direction order
 */
void merge(int** a, int* b, int num_element, int direction);

#endif //BITONIC_SORT_UTILS_H
