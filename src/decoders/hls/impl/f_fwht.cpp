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
t_ram<W+8, 256> fwht_gf256(const t_ram<W, 256> src)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value

    /*
     * ========= ÉTAGE 1 (distance 32) =========
     */
	ap_int<W+1> si[256];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=si
    for (int i = 0; i < 128; i++) {
        si[i]       = (ap_int<W+1>)src.value[i] + (ap_int<W+1>)src.value[i + 128];
        si[i + 128] = (ap_int<W+1>)src.value[i] - (ap_int<W+1>)src.value[i + 128];
    }
    /* ========= ÉTAGE 2 : distance 16 ========= */
    ap_int<W+2> s0[256];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s0
    for (int i = 0; i < 256; i++) {
        int g = (i & 64);
        int j =  i ^ 64;
        s0[i] = (g == 0) ? ((ap_int<W+2>)si[i] + (ap_int<W+2>)si[j]) : ((ap_int<W+2>)si[j] - (ap_int<W+2>)si[i]);
    }
    /* ========= ÉTAGE 2 : distance 16 ========= */
    ap_int<W+3> s1[256];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
    for (int i = 0; i < 256; i++) {
        int g = (i & 32);
        int j = i ^ 32;
        s1[i] = (g == 0) ? ((ap_int<W+3>)s0[i] + (ap_int<W+3>)s0[j]) : ((ap_int<W+3>)s0[j] - (ap_int<W+3>)s0[i]);
    }
    /* ========= ÉTAGE 2 : distance 16 ========= */
    ap_int<W+4> s2[256];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s2
    for (int i = 0; i < 256; i++) {
        int g = (i & 16);
        int j = i ^ 16;
        s2[i] = (g == 0) ? ((ap_int<W+4>)s1[i] + (ap_int<W+4>)s1[j]) : ((ap_int<W+4>)s1[j] - (ap_int<W+4>)s1[i]);
    }

    /* ========= ÉTAGE 3 : distance 8 ========= */
    ap_int<W+5> s3[256];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s3
    for (int i = 0; i < 256; i++) {
        int g = (i & 8);
        int j = i ^ 8;
        s3[i] = (g == 0) ? ((ap_int<W+5>)s2[i] + (ap_int<W+5>)s2[j]) : ((ap_int<W+5>)s2[j] - (ap_int<W+5>)s2[i]);
    }

    /* ========= ÉTAGE 4 : distance 4 ========= */
    ap_int<W+6> s4[256];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s4
    for (int i = 0; i < 256; i++) {
        int g = (i & 4);
        int j = i ^ 4;
        s4[i] = (g == 0) ? ((ap_int<W+6>)s3[i] + (ap_int<W+6>)s3[j]) : ((ap_int<W+6>)s3[j] - (ap_int<W+6>)s3[i]);
    }

    /* ========= ÉTAGE 5 : distance 2 ========= */
    ap_int<W+7> s5[256];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s5
    for (int i = 0; i < 256; i++) {
        int g = (i & 2);
        int j = i ^ 2;
        s5[i] = (g == 0) ? ((ap_int<W+7>)s4[i] + (ap_int<W+7>)s4[j]) : ((ap_int<W+7>)s4[j] - (ap_int<W+7>)s4[i]);
    }

    /* ========= ÉTAGE 6 : distance 1 ========= */
    t_ram<W+8, 256> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < 256; i++) {
        int g = (i & 1);
        int j = i ^ 1;
        dst.value[i] = (g == 0) ? ((ap_int<W+8>)s5[i] + (ap_int<W+8>)s5[j]) : ((ap_int<W+8>)s5[j] - (ap_int<W+8>)s5[i]);
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
t_ram<W+7, 128> fwht_gf128(const t_ram<W, 128> src)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value

    /*
     * ========= ÉTAGE 1 (distance 32) =========
     */
	ap_int<W+1> s0[128];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s0
    for (int i = 0; i < 64; i++) {
        s0[i]      = (ap_int<W+1>)src.value[i] + (ap_int<W+1>)src.value[i + 64];
        s0[i + 64] = (ap_int<W+1>)src.value[i] - (ap_int<W+1>)src.value[i + 64];
    }
    /* ========= ÉTAGE 2 : distance 16 ========= */
    ap_int<W+2> s1[128];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
    for (int i = 0; i < 128; i++) {
        int g = (i & 32);
        int j = i ^ 32;
        s1[i] = (g == 0) ? ((ap_int<W+2>)s0[i] + (ap_int<W+2>)s0[j]) : ((ap_int<W+2>)s0[j] - (ap_int<W+2>)s0[i]);
    }
    /* ========= ÉTAGE 2 : distance 16 ========= */
    ap_int<W+3> s2[128];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s2
    for (int i = 0; i < 128; i++) {
        int g = (i & 16);
        int j = i ^ 16;
        s2[i] = (g == 0) ? ((ap_int<W+3>)s1[i] + (ap_int<W+3>)s1[j]) : ((ap_int<W+3>)s1[j] - (ap_int<W+3>)s1[i]);
    }

    /* ========= ÉTAGE 3 : distance 8 ========= */
    ap_int<W+4> s3[128];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s3
    for (int i = 0; i < 128; i++) {
        int g = (i & 8);
        int j = i ^ 8;
        s3[i] = (g == 0) ? ((ap_int<W+4>)s2[i] + (ap_int<W+4>)s2[j]) : ((ap_int<W+4>)s2[j] - (ap_int<W+4>)s2[i]);
    }

    /* ========= ÉTAGE 4 : distance 4 ========= */
    ap_int<W+5> s4[128];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s4
    for (int i = 0; i < 128; i++) {
        int g = (i & 4);
        int j = i ^ 4;
        s4[i] = (g == 0) ? ((ap_int<W+5>)s3[i] + (ap_int<W+5>)s3[j]) : ((ap_int<W+5>)s3[j] - (ap_int<W+5>)s3[i]);
    }

    /* ========= ÉTAGE 5 : distance 2 ========= */
    ap_int<W+6> s5[128];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s5
    for (int i = 0; i < 128; i++) {
        int g = (i & 2);
        int j = i ^ 2;
        s5[i] = (g == 0) ? ((ap_int<W+6>)s4[i] + (ap_int<W+6>)s4[j]) : ((ap_int<W+6>)s4[j] - (ap_int<W+6>)s4[i]);
    }

    /* ========= ÉTAGE 6 : distance 1 ========= */
    t_ram<W+7, 128> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < 128; i++) {
        int g = (i & 1);
        int j = i ^ 1;
        dst.value[i] = (g == 0) ? ((ap_int<W+7>)s5[i] + (ap_int<W+7>)s5[j]) : ((ap_int<W+7>)s5[j] - (ap_int<W+7>)s5[i]);
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
#pragma HLS INLINE
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
#pragma HLS INLINE
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
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s2
    for (int i = 0; i < 32; i++) {
        int g = (i & 8);
        int j = i ^ 8;
        s2[i] = (g == 0) ? ((ap_int<W+2>)s1[i] + (ap_int<W+2>)s1[j]) : ((ap_int<W+2>)s1[j] - (ap_int<W+2>)s1[i]);
    }

    /* ========= ÉTAGE 4 : distance 4 ========= */
    ap_int<W+3> s3[32];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s3
    for (int i = 0; i < 32; i++) {
        int g = (i & 4);
        int j = i ^ 4;
        s3[i] = (g == 0) ? ((ap_int<W+3>)s2[i] + (ap_int<W+3>)s2[j]) : ((ap_int<W+3>)s2[j] - (ap_int<W+3>)s2[i]);
    }

    /* ========= ÉTAGE 5 : distance 2 ========= */
    ap_int<W+4> s4[32];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s4
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
//#pragma HLS INLINE off
//#pragma HLS PIPELINE II=1
#pragma HLS INLINE
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
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s2
    for (int i = 0; i < 16; i++) {
        int g = (i & 4);
        int j = i ^ 4;
        s2[i] = (g == 0) ? ((ap_int<W+2>)s1[i] + (ap_int<W+2>)s1[j]) : ((ap_int<W+2>)s1[j] - (ap_int<W+2>)s1[i]);
    }

    /* ========= ÉTAGE 5 : distance 2 ========= */
    ap_int<W+3> s3[16];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s3
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
template<int W = 12>
t_ram<W+3, 8> fwht_gf8(const t_ram<W, 8> src)
{
//#pragma HLS INLINE off
//#pragma HLS PIPELINE II=1
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value

    /* ========= ÉTAGE 1 : distance 16 ========= */
	ap_int<W+1> s1[8];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
    for (int i = 0; i < 8; i++) {
        s1[i]     = (ap_int<W+1>)src.value[i] + (ap_int<W+1>)src.value[i + 4];
        s1[i + 4] = (ap_int<W+1>)src.value[i] - (ap_int<W+1>)src.value[i + 4];
    }

    /* ========= ÉTAGE 5 : distance 2 ========= */
    ap_int<W+2> s2[8];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s2
    for (int i = 0; i < 8; i++) {
        int g = (i & 2);
        int j = i ^ 2;
        s2[i] = (g == 0) ? ((ap_int<W+2>)s1[i] + (ap_int<W+2>)s1[j]) : ((ap_int<W+2>)s1[j] - (ap_int<W+2>)s1[i]);
    }

    /* ========= ÉTAGE 6 : distance 1 ========= */
    t_ram<W+3, 8> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < 8; i++) {
        int g = (i & 1);
        int j = i ^ 1;
        dst.value[i] = (g == 0) ? ((ap_int<W+3>)s2[i] + (ap_int<W+3>)s2[j]) : ((ap_int<W+3>)s2[j] - (ap_int<W+3>)s2[i]);
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
t_ram<W+2, 4> fwht_gf4(const t_ram<W, 4> src)
{
//#pragma HLS INLINE off
//#pragma HLS PIPELINE II=1
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value

    /* ========= ÉTAGE 1 : distance 16 ========= */
	ap_int<W+1> s1[4];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
    for (int i = 0; i < 4; i++) {
        s1[i]     = (ap_int<W+1>)src.value[i] + (ap_int<W+1>)src.value[i + 2];
        s1[i + 2] = (ap_int<W+1>)src.value[i] - (ap_int<W+1>)src.value[i + 2];
    }

    /* ========= ÉTAGE 6 : distance 1 ========= */
    t_ram<W+2, 4> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < 4; i++) {
        int g = (i & 1);
        int j = i ^ 1;
        dst.value[i] = (g == 0) ? ((ap_int<W+2>)s1[i] + (ap_int<W+2>)s1[j]) : ((ap_int<W+2>)s1[j] - (ap_int<W+2>)s1[i]);
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
t_ram<14, 4> hls_fwht_gf4(const t_ram<12, 4> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<14, 4> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = fwht_gf4<12>(src);
    return dst;
}
//
//
t_ram<15, 8> hls_fwht_gf8(const t_ram<12, 8> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<15, 8> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = fwht_gf8<12>(src);
    return dst;
}
//
//
t_ram<16, 16> hls_fwht_gf16(const t_ram<12, 16> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<16, 16> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = fwht_gf16<12>(src);
    return dst;
}
//
//
t_ram<17, 32> hls_fwht_gf32(const t_ram<12, 32> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<17, 32> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = fwht_gf32<12>(src);
    return dst;
}
//
//
t_ram<18, 64> hls_fwht_gf64(const t_ram<12, 64> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<18, 64> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = fwht_gf64<12>(src);
    return dst;
}
//
//
t_ram<19, 128> hls_fwht_gf128(const t_ram<12, 128> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<19, 128> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = fwht_gf128<12>(src);
    return dst;
}
//
//
t_ram<20, 256> hls_fwht_gf256(const t_ram<12, 256> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<20, 256> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = fwht_gf256<12>(src);
    return dst;
}
//
//
//
////////////////////////////////////////////////////////////////////////////
//
//
//
