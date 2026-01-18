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
#define gf_size 64
//
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
#define i_lwht_width   data_width
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