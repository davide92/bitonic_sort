#include "time.h"
#include <random>
#include <initializer_list>

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