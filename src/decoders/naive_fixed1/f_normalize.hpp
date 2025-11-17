//
// Created by legal on 03/07/2025.
//
#pragma once
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
template <int gf_size>
void f_normalize(ap_fixed<NBITS, 1> *tab)
{
    ap_fixed<NBITS, 1> sum = 1e-24f;
    for (int i = 0; i < gf_size; i += 1)
    {
        sum += tab[i];
    }
    const ap_fixed<NBITS, 1> zero = 0.f;
    if (zero == sum)
    {
        sum = 1.f;
    }
    for (int i = 0; i < gf_size; i++)
    {
        tab[i] /= sum;
    }
}
#if 0
template <int gf_size>
void f_normalize(ap_fixed<NBITS, 1>  * tab) {
    double sum = 0.0;
    for (int i = 0; i < gf_size; i += 1) {
        sum += tab[i].to_double();
    }
    sum = ( sum == 0.0 ) ? 1.f : sum;

    for (int i = 0; i < gf_size; i++) {
        ap_fixed<NBITS, 1>  vv = tab[i].to_double() / sum;
        tab[i] = vv;
    }
}
#endif
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
template <int gf_size>
void f_normalize(ap_fixed<NBITS, 1> *tab, const float norm)
{
    const ap_fixed<NBITS, 1> factor = norm;
    for (int i = 0; i < gf_size; i++)
    {
        const ap_fixed<NBITS, 1> ff = norm * tab[i].to_double();
        tab[i]                      = ff;
    }
}
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
