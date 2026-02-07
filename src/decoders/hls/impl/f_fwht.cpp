//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "f_fwht.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
t_o_lwht fwht(const t_i_lwht src)
{
#ifdef _COMPONENT_
    #pragma HLS PIPELINE
#else
    #pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value
    /*
     * ========= ÉTAGE 1 (distance 32) =========
     */
	ap_int<i_lwht_width+1> s1[64];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
    for (int i = 0; i < 32; i++) {
        s1[i]      = (ap_int<i_lwht_width+1>)src.value[i] + (ap_int<i_lwht_width+1>)src.value[i + 32];
        s1[i + 32] = (ap_int<i_lwht_width+1>)src.value[i] - (ap_int<i_lwht_width+1>)src.value[i + 32];
    }
    /* ========= ÉTAGE 2 : distance 16 ========= */
    ap_int<i_lwht_width+2> s2[64];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s2
    for (int i = 0; i < 64; i++) {
        int g = (i & 16);
        int j = i ^ 16;
        s2[i] = (g == 0) ? ((ap_int<i_lwht_width+2>)s1[i] + (ap_int<i_lwht_width+2>)s1[j]) : ((ap_int<i_lwht_width+2>)s1[j] - (ap_int<i_lwht_width+2>)s1[i]);
    }

    /* ========= ÉTAGE 3 : distance 8 ========= */
    ap_int<i_lwht_width+3> s3[64];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s3
    for (int i = 0; i < 64; i++) {
        int g = (i & 8);
        int j = i ^ 8;
        s3[i] = (g == 0) ? ((ap_int<i_lwht_width+3>)s2[i] + (ap_int<i_lwht_width+3>)s2[j]) : ((ap_int<i_lwht_width+3>)s2[j] - (ap_int<i_lwht_width+3>)s2[i]);
    }

    /* ========= ÉTAGE 4 : distance 4 ========= */
    ap_int<i_lwht_width+4> s4[64];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s4
    for (int i = 0; i < 64; i++) {
        int g = (i & 4);
        int j = i ^ 4;
        s4[i] = (g == 0) ? ((ap_int<i_lwht_width+4>)s3[i] + (ap_int<i_lwht_width+4>)s3[j]) : ((ap_int<i_lwht_width+4>)s3[j] - (ap_int<i_lwht_width+4>)s3[i]);
    }

    /* ========= ÉTAGE 5 : distance 2 ========= */
    ap_int<i_lwht_width+5> s5[64];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s5
    for (int i = 0; i < 64; i++) {
        int g = (i & 2);
        int j = i ^ 2;
        s5[i] = (g == 0) ? ((ap_int<i_lwht_width+5>)s4[i] + (ap_int<i_lwht_width+5>)s4[j]) : ((ap_int<i_lwht_width+5>)s4[j] - (ap_int<i_lwht_width+5>)s4[i]);
    }

    /* ========= ÉTAGE 6 : distance 1 ========= */
    t_o_lwht dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < 64; i++) {
        int g = (i & 1);
        int j = i ^ 1;
        dst.value[i] = (g == 0) ? ((ap_int<i_lwht_width+6>)s5[i] + (ap_int<i_lwht_width+6>)s5[j]) : ((ap_int<i_lwht_width+6>)s5[j] - (ap_int<i_lwht_width+6>)s5[i]);
    }
    return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
template<int W = 12>
t_ram<W+6, 64> fwht_gf64(const t_ram<W, 64> src)
{
#ifdef _COMPONENT_
    #pragma HLS PIPELINE
#else
    #pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value
    /*
     * ========= ÉTAGE 1 (distance 32) =========
     */
	ap_int<W+1> s1[64];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
    for (int i = 0; i < 32; i++) {
        s1[i]      = (ap_int<W+1>)src.value[i] + (ap_int<W+1>)src.value[i + 32];
        s1[i + 32] = (ap_int<W+1>)src.value[i] - (ap_int<W+1>)src.value[i + 32];
    }
    /* ========= ÉTAGE 2 : distance 16 ========= */
    ap_int<W+2> s2[64];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s2
    for (int i = 0; i < 64; i++) {
        int g = (i & 16);
        int j = i ^ 16;
        s2[i] = (g == 0) ? ((ap_int<W+2>)s1[i] + (ap_int<W+2>)s1[j]) : ((ap_int<W+2>)s1[j] - (ap_int<W+2>)s1[i]);
    }

    /* ========= ÉTAGE 3 : distance 8 ========= */
    ap_int<W+3> s3[64];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s3
    for (int i = 0; i < 64; i++) {
        int g = (i & 8);
        int j = i ^ 8;
        s3[i] = (g == 0) ? ((ap_int<W+3>)s2[i] + (ap_int<W+3>)s2[j]) : ((ap_int<W+3>)s2[j] - (ap_int<W+3>)s2[i]);
    }

    /* ========= ÉTAGE 4 : distance 4 ========= */
    ap_int<W+4> s4[64];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s4
    for (int i = 0; i < 64; i++) {
        int g = (i & 4);
        int j = i ^ 4;
        s4[i] = (g == 0) ? ((ap_int<W+4>)s3[i] + (ap_int<W+4>)s3[j]) : ((ap_int<W+4>)s3[j] - (ap_int<W+4>)s3[i]);
    }

    /* ========= ÉTAGE 5 : distance 2 ========= */
    ap_int<W+5> s5[64];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s5
    for (int i = 0; i < 64; i++) {
        int g = (i & 2);
        int j = i ^ 2;
        s5[i] = (g == 0) ? ((ap_int<W+5>)s4[i] + (ap_int<W+5>)s4[j]) : ((ap_int<W+5>)s4[j] - (ap_int<W+5>)s4[i]);
    }

    /* ========= ÉTAGE 6 : distance 1 ========= */
    t_ram<W+6, 64> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < 64; i++) {
        int g = (i & 1);
        int j = i ^ 1;
        dst.value[i] = (g == 0) ? ((ap_int<W+6>)s5[i] + (ap_int<W+6>)s5[j]) : ((ap_int<W+6>)s5[j] - (ap_int<W+6>)s5[i]);
    }
    return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
template<int W = 12>
t_ram<W+5, 32> fwht_gf32(const t_ram<W, 32> src)
{
#ifdef _COMPONENT_
    #pragma HLS PIPELINE
#else
    #pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value

    /* ========= ÉTAGE 1 : distance 16 ========= */
	ap_int<W+1> s1[32];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
    for (int i = 0; i < 16; i++) {
        s1[i]      = (ap_int<W+1>)src.value[i] + (ap_int<W+1>)src.value[i + 16];
        s1[i + 16] = (ap_int<W+1>)src.value[i] - (ap_int<W+1>)src.value[i + 16];
    }

    /* ========= ÉTAGE 2 : distance 8 ========= */
    ap_int<W+2> s2[32];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s3
    for (int i = 0; i < 32; i++) {
        int g = (i & 8);
        int j = i ^ 8;
        s2[i] = (g == 0) ? ((ap_int<W+2>)s1[i] + (ap_int<W+2>)s1[j]) : ((ap_int<W+2>)s1[j] - (ap_int<W+2>)s1[i]);
    }

    /* ========= ÉTAGE 4 : distance 4 ========= */
    ap_int<W+3> s3[32];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s4
    for (int i = 0; i < 32; i++) {
        int g = (i & 4);
        int j = i ^ 4;
        s3[i] = (g == 0) ? ((ap_int<W+3>)s2[i] + (ap_int<W+3>)s2[j]) : ((ap_int<W+3>)s2[j] - (ap_int<W+3>)s2[i]);
    }

    /* ========= ÉTAGE 5 : distance 2 ========= */
    ap_int<W+4> s4[32];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s5
    for (int i = 0; i < 32; i++) {
        int g = (i & 2);
        int j = i ^ 2;
        s4[i] = (g == 0) ? ((ap_int<W+4>)s3[i] + (ap_int<W+4>)s3[j]) : ((ap_int<W+4>)s3[j] - (ap_int<W+4>)s3[i]);
    }

    /* ========= ÉTAGE 6 : distance 1 ========= */
    t_ram<W+5, 32> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < 32; i++) {
        int g = (i & 1);
        int j = i ^ 1;
        dst.value[i] = (g == 0) ? ((ap_int<W+5>)s4[i] + (ap_int<W+5>)s4[j]) : ((ap_int<W+5>)s4[j] - (ap_int<W+5>)s4[i]);
    }
    return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
template<int W = 12>
t_ram<W+4, 16> fwht_gf16(const t_ram<W, 16> src)
{
#ifdef _COMPONENT_
    #pragma HLS PIPELINE
#else
    #pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value

    /* ========= ÉTAGE 1 : distance 16 ========= */
	ap_int<W+1> s1[16];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
    for (int i = 0; i < 8; i++) {
        s1[i]     = (ap_int<W+1>)src.value[i] + (ap_int<W+1>)src.value[i + 8];
        s1[i + 8] = (ap_int<W+1>)src.value[i] - (ap_int<W+1>)src.value[i + 8];
    }

    /* ========= ÉTAGE 4 : distance 4 ========= */
    ap_int<W+2> s2[16];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s4
    for (int i = 0; i < 16; i++) {
        int g = (i & 4);
        int j = i ^ 4;
        s2[i] = (g == 0) ? ((ap_int<W+2>)s1[i] + (ap_int<W+2>)s1[j]) : ((ap_int<W+2>)s1[j] - (ap_int<W+2>)s1[i]);
    }

    /* ========= ÉTAGE 5 : distance 2 ========= */
    ap_int<W+3> s3[16];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s5
    for (int i = 0; i < 16; i++) {
        int g = (i & 2);
        int j = i ^ 2;
        s3[i] = (g == 0) ? ((ap_int<W+3>)s2[i] + (ap_int<W+3>)s2[j]) : ((ap_int<W+3>)s2[j] - (ap_int<W+3>)s2[i]);
    }

    /* ========= ÉTAGE 6 : distance 1 ========= */
    t_ram<W+4, 16> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < 16; i++) {
        int g = (i & 1);
        int j = i ^ 1;
        dst.value[i] = (g == 0) ? ((ap_int<W+4>)s3[i] + (ap_int<W+4>)s3[j]) : ((ap_int<W+4>)s3[j] - (ap_int<W+4>)s3[i]);
    }
    return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
