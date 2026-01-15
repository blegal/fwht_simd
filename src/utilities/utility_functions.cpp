#include "utilities/utility_functions.hpp"

#include "definitions/custom_types.hpp"

void local_remove_xors(uint16_t * values, int size) {
    if (size == 1)
        return;
    for (int i = 0; i < size / 2; i += 1)
        values[i] ^= values[i + size / 2];
    local_remove_xors(values, size / 2);
    local_remove_xors(values + size / 2, size / 2);
}

// int countLeadingZeros(uint32_t x) {
//     if (x == 0)
//         return 32;
//     int n = 0;
//     if ((x >> 16) == 0) {
//         n += 16;
//         x <<= 16;
//     }
//     if ((x >> 24) == 0) {
//         n += 8;
//         x <<= 8;
//     }
//     if ((x >> 28) == 0) {
//         n += 4;
//         x <<= 4;
//     }
//     if ((x >> 30) == 0) {
//         n += 2;
//         x <<= 2;
//     }
//     if ((x >> 31) == 0) {
//         n += 1;
//     }
//     return n;
// }
