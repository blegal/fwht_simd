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
void i_normalize(int32_t *tab)
{
    int32_t sum = 1; // la valeur la plus basse !
    for (int i = 0; i < gf_size; i += 1)
    {
        sum += tab[i];
    }
    const int32_t zero = 0.f;
    if (zero == sum)
    {
        sum = 1.f;
    }
    for (int i = 0; i < gf_size; i++)
    {
        tab[i] /= sum;
    }
}
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
template <int gf_size>
void i_normalize(int32_t * tab, const int32_t norm) {
    const int64_t factor = norm;
    for (int i = 0; i < gf_size; i++)
    {
        const int64_t ff = factor * (int64_t)tab[i];    // 64 bits mulitplication
        tab[i] = (ff) >> 32;                            // fixed-point scaling
    }
}
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
