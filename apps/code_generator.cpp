#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "../src/definitions/code.hpp"

// Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

#include "frozen_tree_generator.hpp"

//
//
//
// In frozen symbol array, the value -1 means the symbol is frozen => (symbol = 0)
//
int main(int, char *[]) {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // initialize the frozen symbols array
    //
    const int _K_ = 3 * _N_ / 4;

    int* frozen_symbols = new int[_N_];
    for (int i = 0; i < _N_; i += 1)
        frozen_symbols[i] = true;

    for (int i = 0; i <  _K_; i += 1)
        frozen_symbols[reliab_seq[i]] = false; // i c'est pour le DEBUG, on pourrait mettre 0
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    printf("\nFrozen matrix:\n");
    for (int i = 0; i < _N_; i += 1) {
        if ((i % 8) == 0)
            printf(" | ");
        if ((i % 16) == 0)
            printf("\n | ");
        printf("%2d ", frozen_symbols[i]);
    }
    printf(" |\n");
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    frozen_tree_generator pruned_tree(_N_, _GF_);
    pruned_tree.analyze(frozen_symbols, _N_);

    delete[] frozen_symbols;

    return EXIT_SUCCESS;
}
