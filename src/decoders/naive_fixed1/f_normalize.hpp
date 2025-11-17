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
void f_normalize(int64_t *tab)
{
    int64_t sum = 1e-24f;
    for (int i = 0; i < gf_size; i += 1)
    {
        sum += tab[i];
    }
    const int64_t zero = 0.f;
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
void f_normalize(int64_t  * tab) {
    double sum = 0.0;
    for (int i = 0; i < gf_size; i += 1) {
        sum += tab[i].to_double();
    }
    sum = ( sum == 0.0 ) ? 1.f : sum;

    for (int i = 0; i < gf_size; i++) {
        int64_t  vv = tab[i].to_double() / sum;
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
void f_normalize(int64_t *tab, const float norm)
{
    const int64_t factor = norm;
    for (int i = 0; i < gf_size; i++)
    {
        const int64_t ff = norm * tab[i];
        tab[i]           = ff;
    }
}
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
