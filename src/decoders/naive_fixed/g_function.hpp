#pragma once

#include "f_argmax.hpp"
#include "f_normalize.hpp"

template <int gf_size>
void g_function(
    symbols_f * __restrict dst,   // the data to be computed for the left side of the graph
    symbols_f * __restrict src_a, // the upper value set from the right side of the graph
    symbols_f * __restrict src_b, // the lower value set from the right side of the graph
    uint32_t    src_c)    // the computed symbols coming from the left side of the graph
{
#ifndef _TEST_
    printf("(DD) g_function(--)\n");
#endif

#ifndef _TEST_
    if ( is_issue(src_a->value) > 1.f ) {
        printf("(EE) Error in %s %d\n", __FILE__, __LINE__);
        printf("(EE) value = %f\n", is_issue(src_a->value));
        exit(EXIT_FAILURE);
    }
#endif

    if (src_a->is_freq == true) {
        fwht_norm<gf_size>(src_a->value);
        src_a->is_freq = false;
    }

#ifndef _TEST_
    if ( is_issue(src_a->value) > 1.f ) {
        printf("(EE) Error in %s %d\n", __FILE__, __LINE__);
        printf("(EE) value = %f\n", is_issue(src_a->value));
        exit(EXIT_FAILURE);
    }
#endif

    if (src_b->is_freq == true) {
        fwht_norm<gf_size>(src_b->value);
        src_b->is_freq = false;
    }

#ifndef _TEST_
    if ( is_issue(src_b->value) > 1.f ) {
        printf("(EE) Error in %s %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
#endif

    for (size_t i = 0; i < gf_size; i++) {
        const int idx   = src_c ^ i;
        dst->value[idx] = src_a->value[i] * src_b->value[idx];
    }

#ifndef _TEST_
    if ( is_issue(dst->value) > 1.f ) {
        printf("(EE) Error in %s %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
#endif

    f_normalize<gf_size>(dst->value); // temporal

#ifndef _TEST_
    if ( is_issue(dst->value) > 1.f ) {
        printf("(EE) Error in %s %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
#endif

    dst->is_freq = false;
}
//
//
//
//
//
