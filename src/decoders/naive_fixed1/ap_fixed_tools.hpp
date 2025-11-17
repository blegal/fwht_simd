//!
//!     Copyright (c) 2020-2023, Bertrand LE GAL
//!     All rights reserved.
//!
//!     Redistribution and use in source and binary forms, with or without
//!     modification, are not permitted with written authorization.
//!
//!
#pragma once
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
#ifndef AP_FIXED_TOOLS
#define AP_FIXED_TOOLS
#include "definitions/custom_types.hpp"
#include "f_type.hpp"
#include <cstdint>
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//

// template <int GF_SIZE>
// void LZC_shift_at_input(const ap_fixed<NBITS + 1, 2> *s,
//                         ap_fixed<NBITS, 1>           *s1)
// {
//     const int W_in = NBITS + 1;
//     const int I_in = 2;
//     const int F_in = W_in - I_in;

//     bool any_nonzero = false;
//     int  k_max       = -999;

//     for (int j = 0; j < GF_SIZE; j++)
//     {
//         ap_fixed<NBITS + 1, 2> v = s[j];

//         if (v != 0)
//         {
//             any_nonzero            = true;
//             ap_uint<NBITS> bits    = v.range(NBITS - 1, 0);
//             int            lz      = bits.countLeadingZeros();
//             int            msb_pos = (NBITS - 1) - lz;
//             int            k       = msb_pos - F_in;
//             if (k > k_max)
//                 k_max = k;
//         }
//     }

//     if (!any_nonzero)
//     {
//         for (int j = 0; j < GF_SIZE; j++)
//             s1[j] = 0;
//         return;
//     }

//     int shift = -1 - k_max;

//     for (int j = 0; j < GF_SIZE; j++)
//     {
//         ap_fixed<NBITS + 1, 2> v = s[j];
//         ap_fixed<NBITS + 1, 2> v_scaled;

//         if (shift > 0)
//             v_scaled = v << shift;
//         else if (shift < 0)
//             v_scaled = v >> (-shift);
//         else
//             v_scaled = v;

//         s1[j] = (ap_fixed<NBITS, 1>)v_scaled;
//     }
// }

template <int GF_SIZE>
void LZC_shift_after_fwht(const ap_fixed<NBITS + _logGF_, 1 + _logGF_> *s, ap_fixed<NBITS, 1> *s1)
{
    const int W_in = NBITS + _logGF_;
    const int I_in = 1 + _logGF_;
    const int F_in = W_in - I_in; // = NBITS - 1 fractional bits

    bool any_nonzero = false;
    int  k_max       = -999;

    for (int j = 0; j < GF_SIZE; j++)
    {
        ap_fixed<W_in, I_in> v     = s[j];
        ap_fixed<W_in, I_in> neg_v = -v;

        bool                 sgn    = v[W_in - 1];
        ap_fixed<W_in, I_in> temp_v = sgn ? neg_v : v;

        if (v != 0)
        {
            any_nonzero           = true;
            ap_uint<W_in> bits    = temp_v.range(W_in - 1, 0);
            int           lz      = bits.countLeadingZeros();
            int           msb_pos = (W_in - 1) - lz;

            int k = msb_pos - F_in;
            if (k > k_max)
                k_max = k;
        }
    }
    if (!any_nonzero)
    {
        for (int j = 0; j < GF_SIZE; j++)
            s1[j] = 0;
        return;
    }
    int shift = -1 - k_max;
    for (int j = 0; j < GF_SIZE; j++)
    {
        ap_fixed<W_in, I_in> v     = s[j];
        ap_fixed<W_in, I_in> neg_v = -v;
        bool                 sgn   = v[W_in - 1];
        ap_fixed<W_in, I_in> v_abs = sgn ? neg_v : v;
        ap_fixed<W_in, I_in> v_abs_shifted;
        if (shift > 0)
            v_abs_shifted = v_abs << shift;
        else if (shift < 0)
            v_abs_shifted = v_abs >> (-shift);
        else
            v_abs_shifted = v_abs;
        ap_fixed<W_in, I_in> neg_v_shifted = (ap_fixed<W_in, I_in>)(-v_abs_shifted);
        ap_fixed<W_in, I_in> v_norm        = sgn ? neg_v_shifted : v_abs_shifted;
        s1[j]                              = (ap_fixed<NBITS, 1>)v_norm;
    }
}

template <int GF_SIZE>
void LZC_shift_after_fwht_mult(const ap_fixed<2 * NBITS, 2> *s, ap_fixed<NBITS, 1> *s1)
{
    const int W_in = NBITS * 2;
    const int I_in = 2;
    const int F_in = W_in - I_in; // = NBITS - 1 fractional bits

    bool any_nonzero = false;
    int  k_max       = -999;

    for (int j = 0; j < GF_SIZE; j++)
    {
        ap_fixed<W_in, I_in> v     = s[j];
        ap_fixed<W_in, I_in> neg_v = -v;

        bool                 sgn    = v[W_in - 1];
        ap_fixed<W_in, I_in> temp_v = sgn ? neg_v : v;

        if (v != 0)
        {
            any_nonzero           = true;
            ap_uint<W_in> bits    = temp_v.range(W_in - 1, 0);
            int           lz      = bits.countLeadingZeros();
            int           msb_pos = (W_in - 1) - lz;

            int k = msb_pos - F_in;
            if (k > k_max)
                k_max = k;
        }
    }
    if (!any_nonzero)
    {
        for (int j = 0; j < GF_SIZE; j++)
            s1[j] = 0;
        return;
    }
    int shift = -1 - k_max;
    for (int j = 0; j < GF_SIZE; j++)
    {
        ap_fixed<W_in, I_in> v     = s[j];
        ap_fixed<W_in, I_in> neg_v = -v;
        bool                 sgn   = v[W_in - 1];
        ap_fixed<W_in, I_in> v_abs = sgn ? neg_v : v;
        ap_fixed<W_in, I_in> v_abs_shifted;
        if (shift > 0)
            v_abs_shifted = v_abs << shift;
        else if (shift < 0)
            v_abs_shifted = v_abs >> (-shift);
        else
            v_abs_shifted = v_abs;
        ap_fixed<W_in, I_in> neg_v_shifted = (ap_fixed<W_in, I_in>)(-v_abs_shifted);
        ap_fixed<W_in, I_in> v_norm        = sgn ? neg_v_shifted : v_abs_shifted;
        s1[j]                              = (ap_fixed<NBITS, 1>)v_norm;
    }
}

#endif