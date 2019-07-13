#include "utils.h"

using namespace std;

int padding(int num_element, int num_process) {

    int n_elem = num_element;

    /*Generate random number for the array*/
    if (floor((1.0 * num_element) / num_process) != ceil((1.0 * num_element) / num_process)) {

       n_elem = (int) ceil((1.0 * num_element) / num_process) * num_process;

    }

    return n_elem;

}

void populate_array(int *array, int num_element_before_padding, int num_element_after_padding) {

    /*Generate random number for the array*/
    /* initialize random seed: */
    srand (time(NULL));

    for (int i = 0; i < num_element_after_padding; i++) {

        if (i < num_element_before_padding) {

            array[i] = (rand() % num_element_before_padding) + 1;

        } else {

            array[i] = INT32_MAX;

        }
    }

    /*cout << "Initial array: \n";
    print_array(array, num_element_after_padding);*/

}

int int_compare(const void *a, const void *b) {

    return ( *(int *) a - *(int *) b );
}

bool bool_compare(int i, int j) {

    return i < j;
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
void merge_low(int *a, int *b, int num_element) {

    int k = 0;
    int j = 0;

    /* merging the two sorted arrays
    * we have 2N elements and we need num_element,
    * so there is no way to have leftovers*/
    for (int i = 0; i < num_element; i++) {

        if (a[j] < b[k]) {

            result[i] = a[j++];

        } else {

            result[i] = b[k++];

        }
    }
}

/* keeps the larger elements of the 2 arrays.
 * returns result pointer of the sorted array */
void merge_high(int *a, int *b, int num_element, int *result) {

    int k = num_element - 1;
    int j = num_element - 1;

    for (int i = num_element - 1; i >= 0; i--) {

        if (a[j] > b[k]) {

            result[i] = a[j--];

        } else {

            result[i] = b[k--];

        }
    }
}

/* Main merge function that calls either merge low or merge high
 * depending on the value of the direction argument.
*/
void merge(int **a, int *b, int num_element, int direction) {

    int *result = (int *) calloc(num_element, sizeof(int));

    /* result of the requested merge is in a*/
    (direction == ASCENDING) ? merge_low(*a, b, num_element, result) : merge_high(*a, b, num_element, result);

    free(*a);

    *a = result;
}

int correct_sorted(int *sorted_array, int num_element) {

    int is_sorted = 1;

    for (int i = 1; i < num_element; i++) {

        if (sorted_array[i - 1] > sorted_array[i]) {
            
            is_sorted = 0;

        }
    }

    return is_sorted;
}

void print_array(int *array, int num_element) {

    for (int i = 0; i < num_element; i++) {

        cout << array[i] << " ";

    }

    cout << "\n";
}

vector<int> populate_vector(vector<int> array, int num_element, int num_process) {

    int new_size = num_element;
    /*Generate random number for the array*/
    if (floor((1.0 * num_element) / num_process) != ceil((1.0 * num_element) / num_process)) {
        new_size = (int) ceil((1.0 * num_element) / num_process) * num_process;
    }

    random_device rd;
    mt19937_64 mt(rd());
    uniform_int_distribution<int> dist(0, new_size);

    for (int i = 0; i < new_size; i++) {
        if (i < num_element) {
            array.push_back(dist(mt));
        } else {
            array.push_back(INT32_MAX);
        }

    }

    return array;
}

vector<int> merge_low_vector(vector<int> a, vector<int> b) {

    int j = 0;
    int k = 0;

    int num_element = a.size();

    vector<int> result;
    result.reserve(num_element);

    /* merging the two sorted arrays
    * we have 2N elements and we need num_element,
    * so there is no way to have leftovers*/
    for (int i = 0; i < num_element; i++) {
        if (a[j] < b[k]) result[i] = a[j++];
        else result[i] = b[k++];
    }

    return result;
}

/* keeps the larger elements of the 2 arrays.
 * returns result pointer of the sorted array */
vector<int> merge_high_vector(vector<int> a, vector<int> b) {

    int num_element = a.size();
    int k = num_element - 1;
    int j = num_element - 1;

    vector<int> result;
    result.reserve(num_element);

    for (int i = num_element - 1; i >= 0; --i) {
        if (a[j] > b[k]) result[i] = a[j--];
        else result[i] = b[k--];
    }

    return result;
}

vector<int> merge_vector(vector<int> a, vector<int> b, int direction) {
    /* result of the requested merge.  */
    vector<int> result;

    result = (direction == ASCENDING) ? merge_low_vector(a, b) : merge_high_vector(a, b);

    copy(result.begin(), result.end(), a.begin());

    return a;
}


int correct_sorted_vector(vector<int> sorted_array) {

    int is_sorted = 1;
    for (int i = 1; i < sorted_array.size(); i++) {
        if (sorted_array[i - 1] > sorted_array[i]) {
            is_sorted = 0;
        }
    }

    return is_sorted;
}


void print_vector(std::vector<int> array) {

    for (int i : array) {
        cout << i << " ";
    }

    cout << "\n";
}