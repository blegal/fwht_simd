#pragma once

#include "definitions/custom_types.hpp"
#include "features/archi.hpp"

#include "hadamard/Hadamard.hpp"

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
        const float* H = get_Hadamard_line<gf_size>(idx);
        for (size_t i = 0; i < gf_size; i++)
            dst[s].value[i] = src_a[s].value[i] * H[i];
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
