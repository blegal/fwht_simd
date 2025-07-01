#include "utilities/utility_functions.hpp"
#include "definitions/const_config_GF64_N64.hpp"

void show_symbols(const symbols_t * symbols) {
    if (symbols->is_freq) {
        printf("(DD) Symbols [freq]:");
    } else {
        printf("(DD) Symbols [prob]:");
    }

    float sum = 0.f;
    for (int i = 0; i < GF; i += 1) {
        if ((i % 16) == 0)
            printf("\n(DD)  %2d : ", i);
        if (symbols->value[i] >= 0.2f)
            printf("\e[1;32m%+6.4f\e[0m ", symbols->value[i]);
        else
            printf("%+6.4f ", symbols->value[i]);
        sum += symbols->value[i];
    }
    printf("\n(DD)     : sum = %f\n", sum);
}

void show_symbols(const float * symbols) {
    float sum = 0.f;
    for (int i = 0; i < GF; i += 1) {
        if ((i % 16) == 0)
            printf("\n(DD)  %2d : ", i);
        if (symbols[i] >= 0.2f)
            printf("\e[1;32m%+6.4f\e[0m ", symbols[i]);
        else
            printf("%+6.4f ", symbols[i]);
        sum += symbols[i];
    }
    printf("\n(DD)     : sum = %f\n", sum);
}

