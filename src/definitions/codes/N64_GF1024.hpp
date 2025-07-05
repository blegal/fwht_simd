#ifndef CONFIG_CONSTANTS_H
#define CONFIG_CONSTANTS_H

constexpr int GF        = 1024;
constexpr int logGF     = 10;

constexpr int N         = 64;
constexpr int logN      = 6;

constexpr int reliab_seq[N] = {63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 15, 14, 13, 12, 11, 10, 9, 7, 6, 5, 3, 32, 16, 8, 2, 4, 1, 0};

#endif