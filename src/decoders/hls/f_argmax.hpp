#pragma once
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "types.hpp"
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
inline tuple f_max(const tuple a, const tuple b)
{
#pragma HLS INLINE
	if( a.value > b.value ) return a;
	else                    return b;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
static uint8_t vec_i_unroll_argmax(const t_i_memo inp)
{
#pragma HLS INLINE
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
