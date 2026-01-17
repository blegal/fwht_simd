#include <cstdio>
#include <cstdlib>
#include <cmath>
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#include <cstdint>
//#include "ap_fixed.h"
#include "include/ap_fixed.h"

#define _GF_ 64
#define gf_size 64

#define _N_ 64
#define FWHT_NORM fwht_norm_64

#define uint6b ap_uint<6>
#define int18b ap_int<18>
#define int19b ap_int<19>
#define int20b ap_int<20>
#define int21b ap_int<21>
#define int22b ap_int<22>
#define int23b ap_int<23>
#define int24b ap_int<24>
#define int48b ap_int<48>

#define W 18
#define i_W W
//
//
//
#define gf_size      64
#define log2_gf_size  6
#define data_width   18
//
#define i_lwht_width   18
#define o_lwht_width   (i_lwht_width+log2_gf_size)
#define i_mult_width   (o_lwht_width)
#define o_mult_width   (i_lwht_width*i_lwht_width)
#define i_norm_width   (o_mult_width)
#define o_norm_width   (i_lwht_width)
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
typedef struct tuple{
	ap_int <data_width>   value;
    ap_uint<log2_gf_size> index;
} tuple;
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
tuple f_max(const tuple a, const tuple b)
{
#pragma HLS INLINE
    if( a.value > b.value ) { return a; }
	else                    { return b; }
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
typedef struct { uint6b value[gf_size]; } t_uint6b;
typedef struct { int18b value[gf_size]; } t_int18b;
typedef struct { int19b value[gf_size]; } t_int19b;
typedef struct { int20b value[gf_size]; } t_int20b;
typedef struct { int21b value[gf_size]; } t_int21b;
typedef struct { int22b value[gf_size]; } t_int22b;
typedef struct { int23b value[gf_size]; } t_int23b;
typedef struct { int24b value[gf_size]; } t_int24b;
typedef struct { int48b value[gf_size]; } t_int48b;
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
uint8_t vec_i_unroll_argmax(const t_int18b inp)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
    //
    // stage 0
    //
	tuple s1[gf_size/ 2];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
	for (int i = 0; i < gf_size; i += 2) // 64
    {
#pragma HLS UNROLL
        const tuple A = {inp.value[  i], i  };
        const tuple B = {inp.value[i+1], i+1};
        s1[i >> 1] = f_max(A, B);
    }
    //
    //
	tuple s2[gf_size/ 4];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s2
	for (int i = 0; i < gf_size/2; i += 2)  // 32
    {
#pragma HLS UNROLL
        s2[i >> 1] = f_max(s1[i], s1[i+1]);
    }
	//
	//
	tuple s3[gf_size/ 8];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s3
	for (int i = 0; i < gf_size/4; i += 2)  // 16
    {
#pragma HLS UNROLL
        s3[i >> 1] = f_max(s2[i], s2[i+1]);
    }
	//
	//
	tuple s4[gf_size/16];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s4
	for (int i = 0; i < gf_size/8; i += 2)  // 8
    {
#pragma HLS UNROLL
        s4[i >> 1] = f_max(s3[i], s3[i+1]);
    }
	//
	//
	tuple s5[gf_size/32];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s5
	for (int i = 0; i < gf_size/16; i += 2) // 4
    {
#pragma HLS UNROLL
        s5[i >> 1] = f_max(s4[i], s4[i+1]);
    }
	//
	//
	tuple s6;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s6
	s6 = f_max(s5[0], s5[1]);
	//
	//
	return s6.index;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
t_int18b vec_i_norm(const t_int48b src)
{
#pragma HLS INLINE
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value

	int48b absv[64];
    for (int i = 0; i < gf_size; i++)
    {
    	const int48b value = src.value[i];
    	if(value < 0)	absv[i] = -value;
    	else			absv[i] =  value;
    }

    int48b loc[4];
    // Traitement de 16 données par ligne
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=loc
    loc[0] = ((absv[ 0] + absv[ 1]) + (absv[ 2] + absv[ 3])) +
             ((absv[ 4] + absv[ 5]) + (absv[ 6] + absv[ 7])) +
             ((absv[ 8] + absv[ 9]) + (absv[10] + absv[11])) +
             ((absv[12] + absv[13]) + (absv[14] + absv[15]));

    loc[1] = ((absv[16] + absv[17]) + (absv[18] + absv[19])) +
             ((absv[20] + absv[21]) + (absv[22] + absv[23])) +
             ((absv[24] + absv[25]) + (absv[26] + absv[27])) +
             ((absv[28] + absv[29]) + (absv[30] + absv[31]));

    loc[2] = ((absv[32] + absv[33]) + (absv[34] + absv[35])) +
             ((absv[36] + absv[37]) + (absv[38] + absv[39])) +
             ((absv[40] + absv[41]) + (absv[42] + absv[43])) +
             ((absv[44] + absv[45]) + (absv[46] + absv[47]));

    loc[3] = ((absv[48] + absv[49]) + (absv[50] + absv[51])) +
             ((absv[52] + absv[53]) + (absv[54] + absv[55])) +
             ((absv[56] + absv[57]) + (absv[58] + absv[59])) +
             ((absv[60] + absv[61]) + (absv[62] + absv[63]));

    const int48b sum = loc[0] + loc[1] + loc[2] + loc[3];

    ap_uint<6> factor;
    if      (sum[i_W-1]  == 1) factor = 0;
    else if (sum[i_W-2]  == 1) factor = 1;
    else if (sum[i_W-3]  == 1) factor = 2;
    else if (sum[i_W-4]  == 1) factor = 3;
    else if (sum[i_W-5]  == 1) factor = 4;
    else if (sum[i_W-6]  == 1) factor = 5;
    else if (sum[i_W-7]  == 1) factor = 6;
    else if (sum[i_W-8]  == 1) factor = 7;
    else if (sum[i_W-9]  == 1) factor = 8;
    else if (sum[i_W-10] == 1) factor = 9;
    else if (sum[i_W-11] == 1) factor = 10;
    else if (sum[i_W-12] == 1) factor = 11;
    else if (sum[i_W-13] == 1) factor = 12;
    else if (sum[i_W-14] == 1) factor = 13;
    else if (sum[i_W-15] == 1) factor = 14;
    else if (sum[i_W-16] == 1) factor = 15;
    else if (sum[i_W-17] == 1) factor = 16;
    else if (sum[i_W-18] == 1) factor = 17;
    else if (sum[i_W-19] == 1) factor = 18;
    else if (sum[i_W-20] == 1) factor = 19;
    else if (sum[i_W-21] == 1) factor = 20;
    else if (sum[i_W-22] == 1) factor = 21;
    else if (sum[i_W-23] == 1) factor = 22;
    else if (sum[i_W-24] == 1) factor = 23;
    else if (sum[i_W-25] == 1) factor = 24;
    else if (sum[i_W-26] == 1) factor = 25;
    else if (sum[i_W-27] == 1) factor = 26;
    else if (sum[i_W-28] == 1) factor = 27;
    else if (sum[i_W-29] == 1) factor = 28;
    else if (sum[i_W-30] == 1) factor = 29;
    else if (sum[i_W-31] == 1) factor = 30;
    else if (sum[i_W-32] == 1) factor = 31;
    else if (sum[i_W-33] == 1) factor = 32;
    else if (sum[i_W-34] == 1) factor = 33;
    else if (sum[i_W-35] == 1) factor = 34;
    else if (sum[i_W-36] == 1) factor = 35;
    else if (sum[i_W-37] == 1) factor = 36;
    else if (sum[i_W-38] == 1) factor = 37;
    else if (sum[i_W-39] == 1) factor = 38;
    else if (sum[i_W-40] == 1) factor = 39;
    else if (sum[i_W-41] == 1) factor = 40;
    else if (sum[i_W-42] == 1) factor = 41;
    else if (sum[i_W-43] == 1) factor = 42;
    else if (sum[i_W-44] == 1) factor = 43;
    else if (sum[i_W-45] == 1) factor = 44;
    else if (sum[i_W-46] == 1) factor = 45;
    else if (sum[i_W-47] == 1) factor = 46;
    else factor = 47;

    t_int18b dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < gf_size; i++) {
    	int48b value = src.value[i] << factor;
        dst.value[i] = value.range(47, 30);
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
t_int24b extend(const t_int18b& src)
{
#pragma HLS INLINE
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value
	t_int24b dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < 64; i++) {
		dst.value[i] = src.value[i];
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
t_int24b fwht(const t_int18b src)
{
#pragma HLS INLINE
#pragma HLS PIPELINE
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
    t_int24b dst;
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
t_int48b vec_i_mul_f(const t_int24b src_1, const t_int24b src_2)
{
#pragma HLS INLINE
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value
	t_int48b dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < gf_size; i++)
	{
		dst.value[i] = src_1.value[i] * src_2.value[i];
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
t_int48b vec_i_mul_g(const t_int24b src_1, const t_int24b src_2, const uint8_t symbol)
{
#pragma HLS INLINE
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value

	t_uint6b tab;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab.value
	for (int i = 0; i < gf_size; i += 1)
	{
		tab.value[i] = (symbol ^ i);
	}
	t_int48b dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < gf_size; i += 1)
	{
		dst.value[i] = src_1.value[ tab.value[i] ] * src_2.value[i];
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
t_int18b datapath(const t_int18b lwht_in_a, const t_int18b lwht_in_b, const uint8_t symbol, const bool en_lwth)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_b.value

	t_uint6b tab;
	const auto mult_in_a   = fwht( lwht_in_a );
	const auto mult_in_b   = fwht( lwht_in_b );

	const auto mult_in_c   = extend( lwht_in_a );
	const auto mult_in_d   = extend( lwht_in_b );

	const auto mult_in_e   = en_lwth ? mult_in_a : mult_in_c;
	const auto mult_in_f   = en_lwth ? mult_in_b : mult_in_d;

	const auto norm_in_a   = vec_i_mul_g(mult_in_e, mult_in_f, 0); // f_mode
	const auto memo_in_a   = vec_i_norm( norm_in_a );

#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_b.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_c.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_d.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_e.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=norm_in_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=memo_in_a.value

	return memo_in_a;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
