#include <iostream>
#include <algorithm>
#include <array>
#include <string>
#include <vector>

#define find(value, array) \
    std::find(array.begin(), array.end(), value)

#define isValInArray(value, array) \
    (find(value, array) != array.end())