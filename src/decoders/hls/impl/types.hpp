#pragma once
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstdint>
//
//
//#include "ap_fixed.h"
#include "include/ap_fixed.h"
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#define gf_size      64
#define log2_gf_size  6
#define data_width   12
//
//
//
#define i_lwht_width   data_width
#define o_lwht_width   (i_lwht_width+log2_gf_size)
#define i_mult_width   (o_lwht_width)
#define o_mult_width   (o_lwht_width+o_lwht_width)
#define i_norm_width   (o_mult_width)
#define o_norm_width   (data_width)
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
typedef struct { ap_uint<log2_gf_size> value[gf_size]; } t_uint6b;
typedef struct { ap_int <data_width  > value[gf_size]; } t_i_memo;
typedef struct { ap_int <i_lwht_width> value[gf_size]; } t_i_lwht;
typedef struct { ap_int <o_lwht_width> value[gf_size]; } t_o_lwht;
typedef struct { ap_int <i_mult_width> value[gf_size]; } t_i_mult;
typedef struct { ap_int <o_mult_width> value[gf_size]; } t_o_mult;
typedef struct { ap_int <i_norm_width> value[gf_size]; } t_i_norm;
typedef struct { ap_int <o_norm_width> value[gf_size]; } t_o_norm;
//
//
//
inline t_i_lwht cast(const t_i_memo src) {
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value
    t_i_lwht dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < gf_size; i++) {
        dst.value[i] = src.value[i];
    }
    return dst;
}
//
//
//
inline t_i_mult cast(const t_o_lwht src) {
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value
    t_i_mult dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < gf_size; i++) {
        dst.value[i] = src.value[i];
    }
    return dst;
}
//
//
//
inline t_i_norm cast(const t_o_mult src) {
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value
    t_i_norm dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < gf_size; i++) {
        dst.value[i] = src.value[i];
    }
    return dst;
}
//
//
//
inline t_i_memo cast(const t_o_norm src) {
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value
    t_i_memo dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < gf_size; i++) {
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
inline void show(const ap_int<data_width>* value)
{
    for (int i = 0; i < 64; i++) {
        if (i     == 0) printf("%3d :", i);
        else if (i % 8 == 0) printf("\n%3d :", i);
        printf("%+12d ", value[i].to_int());
    }
    printf("\n");
}
//
inline void show(const ap_int<o_lwht_width>* value)
{
    for (int i = 0; i < 64; i++) {
        if (i     == 0) printf("%3d :", i);
        else if (i % 8 == 0) printf("\n%3d :", i);
        printf("%+12d ", value[i].to_int());
    }
    printf("\n");
}
//
inline void show(const ap_int<o_mult_width>* value)
{
    for (int i = 0; i < 64; i++) {
        if (i     == 0) printf("%3d :", i);
        else if (i % 8 == 0) printf("\n%3d :", i);
        printf("%+12d ", value[i].to_int());
    }
    printf("\n");
}
//
/*
#define i_lwht_width   data_width
#define o_lwht_width   (i_lwht_width+log2_gf_size)
#define i_mult_width   (o_lwht_width)
#define o_mult_width   (o_lwht_width+o_lwht_width)
#define i_norm_width   (o_mult_width)
#define o_norm_width   (data_width)
*/
