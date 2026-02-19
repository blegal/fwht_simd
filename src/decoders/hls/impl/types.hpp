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
#ifdef _SYNTHESIS_
    #include "ap_fixed.h"
#else
    #include "include/ap_fixed.h"
#endif
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

template<int W, int N>
struct t_ram{
    ap_int <W> value[N];
};

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
template<int W, int log2N>
struct gtuple{
    ap_int <W>     value;
    ap_uint<log2N> index;
};
//
//
//
template<int W, int log2N>
struct qtuple{
    ap_uint<log2N> idx_1;
    ap_int <W>     prob_1;
    ap_uint<log2N> idx_2;
    ap_int <W>     prob_2;
};
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
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
inline void local_remove_xors(uint8_t * values, int size, const int offset = 0) {
    if (size == 1)
        return;
    for (int i = 0; i < size / 2; i += 1) {
//        printf("< values[%d] ^= values[%d];\n", offset + i, offset + i + size / 2);
        values[i] ^= values[i + size / 2];
    }
    local_remove_xors(values,            size / 2, offset);
    local_remove_xors(values + size / 2, size / 2, offset + size / 2);
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
template<int N> inline void local_remove_xors(uint8_t * dst, uint8_t * src, const uint16_t offset)
{
    if( dst == nullptr ) exit( EXIT_FAILURE );
    if( src == nullptr ) exit( EXIT_FAILURE );
    if( offset > 64    ) exit( EXIT_FAILURE );
    exit( EXIT_FAILURE );
}
//
//
template<> inline void local_remove_xors<2>(uint8_t * dst, uint8_t * src, const uint16_t offset)
{
#pragma HLS INLINE
    int cnt_rw = offset;
    int cnt_rd = offset + 1;
    uint8_t resu = dst[cnt_rw] ^ src[cnt_rd];
    dst[cnt_rw] = resu;
    src[cnt_rw] = resu;
    //printf("> dst[%d] = dst[%d] ^ src[%d]\n", cnt_rw, cnt_rw, cnt_rd);
}
//
//
template<> inline void local_remove_xors<4>(uint8_t * dst, uint8_t * src, const uint16_t offset)
{
#pragma HLS INLINE
    int cnt_rw = offset;
    int cnt_rd = offset + 2;
    for (int i = 0; i < 2; i += 1) {
#pragma HLS PIPELINE
        uint8_t resu = dst[cnt_rw] ^ src[cnt_rd];
        dst[cnt_rw] = resu;
        src[cnt_rw] = resu;
        cnt_rw += 1;
        cnt_rd += 1;
    }
    local_remove_xors<2>(dst, src, offset);
    local_remove_xors<2>(dst, src, offset + 2);
}
//
//
template<> inline void local_remove_xors<8>(uint8_t * dst, uint8_t * src, const uint16_t offset)
{
#pragma HLS INLINE
    int cnt_rw = offset;
    int cnt_rd = offset + 4;
    for (int i = 0; i < 4; i += 1) {
#pragma HLS PIPELINE
        uint8_t resu = dst[cnt_rw] ^ src[cnt_rd];
        dst[cnt_rw] = resu;
        src[cnt_rw] = resu;
        cnt_rw += 1;
        cnt_rd += 1;
    }
    local_remove_xors<4>(dst, src, offset    );
    local_remove_xors<4>(dst, src, offset + 4);
}
//
//
template<> inline void local_remove_xors<16>(uint8_t * dst, uint8_t * src, const uint16_t offset)
{
#pragma HLS INLINE
    int cnt_rw = offset;
    int cnt_rd = offset + 8;
    for (int i = 0; i < 8; i += 1) {
#pragma HLS PIPELINE
        uint8_t resu = dst[cnt_rw] ^ src[cnt_rd];
        dst[cnt_rw] = resu;
        src[cnt_rw] = resu;
        cnt_rw += 1;
        cnt_rd += 1;
    }
    local_remove_xors<8>(dst, src, offset);
    local_remove_xors<8>(dst, src, offset + 8);
}
//
//
template<> inline void local_remove_xors<32>(uint8_t * dst, uint8_t * src, const uint16_t offset)
{
#pragma HLS INLINE
    int cnt_rw = offset;
    int cnt_rd = offset + 16;
    for (int i = 0; i < 16; i += 1) {
#pragma HLS PIPELINE
        uint8_t resu = dst[cnt_rw] ^ src[cnt_rd];
        dst[cnt_rw] = resu;
        src[cnt_rw] = resu;
        cnt_rw += 1;
        cnt_rd += 1;
    }
    local_remove_xors<16>(dst, src, offset);
    local_remove_xors<16>(dst, src, offset + 16);
}
//
//
template<> inline void local_remove_xors<64>(uint8_t * dst, uint8_t * src, const uint16_t offset)
{
#pragma HLS INLINE
    int cnt_rw = offset;
    int cnt_rd = offset + 32;
    for (int i = 0; i < 32; i += 1) {
#pragma HLS PIPELINE
        uint8_t resu = dst[cnt_rw] ^ src[cnt_rd];
        dst[cnt_rw] = resu;
        src[cnt_rw] = resu;
        cnt_rw += 1;
        cnt_rd += 1;
    }
    local_remove_xors<32>(dst, src, offset);
    local_remove_xors<32>(dst, src, offset + 32);
}
//
//
template<> inline void local_remove_xors<128>(uint8_t * dst, uint8_t * src, const uint16_t offset)
{
#pragma HLS INLINE
    int cnt_rw = offset;
    int cnt_rd = offset + 64;
    for (int i = 0; i < 64; i += 1) {
#pragma HLS PIPELINE
        uint8_t resu = dst[cnt_rw] ^ src[cnt_rd];
        dst[cnt_rw] = resu;
        src[cnt_rw] = resu;
        cnt_rw += 1;
        cnt_rd += 1;
    }
    local_remove_xors<64>(dst, src, offset);
    local_remove_xors<64>(dst, src, offset + 64);
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
