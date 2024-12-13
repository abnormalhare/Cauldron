#include <iostream>
#include <algorithm>
#include <array>
#include <string>
#include <vector>

#define find(value, array) \
    std::find(array.begin(), array.end(), value)

#define isValInArray(value, array) \
    (find(value, array) != array.end())

#define mycopy(arr1, arr2) copy(arr1.begin(), arr1.end(), back_inserter(arr2))

#define clrscrn() std::cout << "\033[2J\033[1;1H"