// #pragma once

// #include "features/archi.hpp"

//

#pragma once

#include "definitions/custom_types.hpp"
#include "features/archi.hpp"


#if _GF_ == 8
#include "hadamard/Hadamard_8.hpp"
#elif _GF_ == 16
#include "hadamard/Hadamard_16.hpp"
#elif _GF_ == 32
#include "hadamard/Hadamard_32.hpp"
#elif _GF_ == 64
#include "hadamard/Hadamard_64.hpp"
#elif _GF_ == 128
#include "hadamard/Hadamard_128.hpp"
#elif _GF_ == 256
#include "hadamard/Hadamard_256.hpp"
#elif _GF_ == 512
#include "hadamard/Hadamard_512.hpp"
#elif _GF_ == 1024
#include "hadamard/Hadamard_1024.hpp"
#elif _GF_ == 2048
#include "hadamard/Hadamard_2048.hpp"
#elif _GF_ == 4096
#include "hadamard/Hadamard_4096.hpp"
#endif

template <int gf_size>
inline __attribute__((always_inline)) void g_function_freq_in(
    symbols_t * __restrict dst,   // the data to be computed for the left side of the graph
    symbols_t * __restrict src_a, // the upper value set from the right side of the graph
    symbols_t * __restrict src_b, // the lower value set from the right side of the graph
    const uint32_t src_c)         // the computed symbols coming from the left side of the graph
{
    for (size_t i = 0; i < gf_size; i++)
        dst->value[i] = src_a->value[i] * Hadamard[src_c][i];

    FWHT<gf_size>(dst->value);
    dst->is_freq = false;

    FWHT<gf_size>(src_b->value);
    src_b->is_freq = false;

    for (size_t i = 0; i < gf_size; i++)
        dst->value[i] = dst->value[i] * src_b->value[i];

    normalize<gf_size>(dst->value);
    dst->is_freq = false;
}
