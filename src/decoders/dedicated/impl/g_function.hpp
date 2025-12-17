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
void g_function_freq_in(
    symbols_s<gf_size>* __restrict dst,
    symbols_s<gf_size>* __restrict src_a,
    symbols_s<gf_size>* __restrict src_b,
    const uint16_t * __restrict src_c,
    const int n_symbols)
{
    for (int s = 0; s < n_symbols; s++)
    {
        const int idx = src_c[s];
        for (size_t i = 0; i < gf_size; i++)
            dst[s].value[i] = src_a[s].value[i] * Hadamard[idx][i];
        FWHT<gf_size>(dst[s].value);
        FWHT<gf_size>(src_b[s].value);
        for (size_t i = 0; i < gf_size; i++)
            dst[s].value[i] = dst[s].value[i] * src_b[s].value[i];
        normalize<gf_size>(dst[s].value);
    }
}
//
//
//
//
//
template <int gf_size>  void g_function_proba_in(
          symbols_s<gf_size>* __restrict dst,
    const symbols_s<gf_size>* __restrict src_a,
    const symbols_s<gf_size>* __restrict src_b,
    const uint16_t*  __restrict src_c,
    const int n_symbols)
{
    for (int s = 0; s < n_symbols; s++)
    {
        for (int i = 0; i < gf_size; i += 1)
        {
            const int idx   = src_c[s] ^ i;
            dst[s].value[idx] = src_a[s].value[i] * src_b[s].value[idx];
        }
        normalize<gf_size>(dst[s].value); // temporal
    }
}
//
//
//
//
//
template <int gf_size> void g_function_freq_in_after_rate_0(
    symbols_s<gf_size> * __restrict dst,
    symbols_s<gf_size> * __restrict src_a,
    symbols_s<gf_size> * __restrict src_b,
    const int n_symbols)
{
    for (int s = 0; s < n_symbols; s++)
    {
        FWHT<gf_size>(src_a[s].value);
        FWHT<gf_size>(src_b[s].value);

        for (int i = 0; i < gf_size; i++)
        {
            const float val = src_a[s].value[i] * src_b[s].value[/*idx*/ i];
            dst[s].value[i] = val;
        }
        normalize<gf_size>(dst[s].value);
    }
}
//
//
//
//
//
template <int gf_size> void g_function_proba_in_after_rate_0(
          symbols_s<gf_size>* __restrict dst,
    const symbols_s<gf_size>* __restrict src_a,
    const symbols_s<gf_size>* __restrict src_b,
    const int n_symbols)
{
    for (int s = 0; s < n_symbols; s++)
    {
        for (int i = 0; i < gf_size; i += 1)
        {
            dst[s].value[i] = src_a[s].value[i] * src_b[s].value[i];
        }
        normalize<gf_size>(dst[s].value);
    }
}
//
//
//
//
//
