#pragma once
#include <cstdio>

#include "definitions/custom_types.hpp"
#include "features/archi.hpp"

void local_remove_xors(uint16_t * values, int size);

template <int gf_size>
void show_symbols(const symbols_t * symbols) {
    if (symbols->is_freq) {
        printf("(DD) Symbols [freq]:");
    } else {
        printf("(DD) Symbols [prob]:");
    }

    float sum = 0.f;
    for (int i = 0; i < gf_size; i += 1) {
        if ((i % 16) == 0)
            printf("\n(DD)  %2d : ", i);
        if (symbols->value[i] >= 0.2f)
            printf("\e[1;32m%+6.4f\e[0m ", symbols->value[i]);
        else
            printf("%+6.4f ", symbols->value[i]);
        sum += symbols->value[i];
    }
    const int symb = argmax<gf_size>(symbols->value);
    printf("\n(DD)     : sum = %f | argmax = %d\n", sum, symb);
}

template <int gf_size>
void show_symbols(const float * symbols) {
    float sum = 0.f;
    for (int i = 0; i < gf_size; i += 1) {
        if ((i % 16) == 0)
            printf("\n(DD)  %2d : ", i);
        if (symbols[i] >= 0.1f)
            printf("\e[1;32m%+6.4f\e[0m ", symbols[i]);
        else
            printf("%+6.4f ", symbols[i]);
        sum += symbols[i];
    }
    const int symb = argmax<gf_size>(symbols);
    printf("\n(DD)     : sum = %f | argmax = %d\n", sum, symb);
}

template <int gf_size>
void show_symbols(const double * symbols) {
    double sum = 0.f;
    for (int i = 0; i < gf_size; i += 1) {
        if ((i % 16) == 0)
            printf("\n(DD)  %2d : ", i);
        if (symbols[i] >= 0.2f)
            printf("\e[1;32m%+6.4lf\e[0m ", symbols[i]);
        else
            printf("%+6.4f ", symbols[i]);
        sum += symbols[i];
    }
    const int symb = argmax<gf_size>(symbols);
    printf("\n(DD)     : sum = %lf | argmax = %d\n", sum, symb);
}
