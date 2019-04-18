#include "utils.h"

using namespace std;

void populate_array(int* array, int num_element) {

    /*Generate random number for the array*/
    random_device rd;
    mt19937_64 mt(rd());
    uniform_int_distribution<int> dist(0, 100);

    for (int i = 0; i < num_element; i++) {
        array[i] = dist(mt);
    }
}

int compare (const void* a, const void* b)
{
    return ( *(int*) a - *(int*) b );
}

int cantor(int a, int b) {

    return ((a + b) * (a + b + 1) * 1/2) + b;
}

void inverse_cantor(int z, int res[2]) {
    int w = floor((sqrt(8 * z + 1) - 1) / 2);
    int t = (w * (w + 1)) / 2;
    res[1] = z - t; //second process
    res[0] = w - res[1]; //first process
}

/* keeps the smaller elements of the 2 arrays.
 * returns result pointer of the sorted array */
int* merge_low(int* a, int* b, int num_element)
{
    int k = 0;
    int j = 0;

    int *result = (int*) calloc(num_element, sizeof(int));

    /* merging the two sorted arrays
    * we have 2N elements and we need num_element,
    * so there is no way to have leftovers*/
    for (size_t i = 0; i < num_element; ++i) {
        if (a[j] < b[k]) result[i] = a[j++];
        else result[i] = b[k++];
    }

    return result;
}

/* keeps the larger elements of the 2 arrays.
 * returns result pointer of the sorted array */
int* merge_high(int* a, int* b, int num_element)
{
    int k = num_element - 1;
    int j = num_element - 1;

    int *result = (int*) calloc(num_element, sizeof(int));

    for (int i = num_element - 1; i >= 0; --i) {
        if (a[j] > b[k]) result[i] = a[j--];
        else result[i] = b[k--];
    }

    return result;
}

/* Main merge function that calls either merge low or merge high
 * depending on the value of the direction argument.
*/
void merge(int** a, int* b, int num_element, int direction) {

    /* result of the requested merge.  */
    int* res = (direction == ASCENDING) ? merge_low(*a, b, num_element) : merge_high(*a, b, num_element);
    free(*a);
    *a = res;
}

int correct_sorted(int* sorted_array, int num_element) {
    int is_sorted = 1;
    for (int i = 1; i < num_element; i++) {
        if (sorted_array[i - 1] > sorted_array[i]) {
            is_sorted = 0;
        }
    }

    return is_sorted;
}

