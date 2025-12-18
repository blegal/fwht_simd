#pragma once
//
//
#include "features/archi.hpp"
#include "definitions/custom_types.hpp"
//
//
template <int gf_size> void f_function_freq_in(
          symbols_s<gf_size>* __restrict dst,
    const symbols_s<gf_size>* __restrict src_a,
    const symbols_s<gf_size>* __restrict src_b,
    const int n_symbols
){
    for (int s = 0; s < n_symbols; s++)
        for (int i = 0; i < gf_size; i++)
            dst[s].value[i] = src_a[s].value[i] * src_b[s].value[i];
}
//
//
//
//
//
template <int gf_size>
void f_function_proba_in(
          symbols_s<gf_size>* __restrict dst,
    const symbols_s<gf_size>* __restrict src_a,
    const symbols_s<gf_size>* __restrict src_b,
    const int n_symbols)
{
    for (int s = 0; s < n_symbols; s++)
    {
        symbols_s<gf_size> tmp_a;
        for (int i = 0; i < gf_size; i++)
            tmp_a.value[i] = src_a[s].value[i];
        FWHT<gf_size>(tmp_a.value);

        symbols_s<gf_size> tmp_b;
        for (int i = 0; i < gf_size; i++)
            tmp_b.value[i] = src_b[s].value[i];
        FWHT<gf_size>(tmp_b.value);

        //
        // Element-wise multiplication of the two input symbols because we are in frequency domain !
        //
        for (size_t i = 0; i < gf_size; i++)
        {
            dst[s].value[i] = tmp_a.value[i] * tmp_b.value[i]; // TODO : attention au facteur 10x qui est magique !!!
        }
    }
}
//
//
//
//
//
