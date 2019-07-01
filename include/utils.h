#ifndef BITONIC_SORT_UTILS_H
#define BITONIC_SORT_UTILS_H

#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <algorithm>

#define MASTER_PROCESS 0

#define ASCENDING 1
#define DESCENDING -1

// Define enumerated types for errors started from 3
enum {
    ARG_ERROR = 3,
    CALLOC_ERROR,
    NTHREADS_ERROR
} errors;


/*Input data structure*/
typedef struct {
    int p; //number of processors
    int n; //number of elements to sort
} input;


/**
 * Populate an array of integer
 * @param array The pointer to the array
 * @param num_element The number of elements in the array
 * @return The size of the populated array
 */
int populate_array(int *array, int num_element, int num_process);


int int_compare(const void *a, const void *b);

bool bool_compare(int i, int j);

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
void merge(int **a, int *b, int num_element, int direction);

/**
 * Verify if the array is correctly sorted
 * @param sorted_array The pointer to the sorted array
 * @param num_element The number of elements in the array
 * @return 1 if the array is correctly sort, 0 otherwise
 */
int correct_sorted(int *sorted_array, int num_element);

/**
 * Print on the standard output the array
 * @param array The pointer to the array to print
 * @param num_element The number of elements in the array
 */
void print_array(int *array, int num_element);

std::vector<int> populate_vector(std::vector<int> array, int num_element, int num_process);

std::vector<int> merge_vector(std::vector<int> a, std::vector<int> b, int direction);

int correct_sorted_vector(std::vector<int> sorted_array);

void print_vector(std::vector<int> array);

#endif //BITONIC_SORT_UTILS_H
