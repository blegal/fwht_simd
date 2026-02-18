//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "f_mult.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
ap_int<12+8> vec_interleave_gf256(const ap_int<12+8> src[256], const ap_uint<8> symbol)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src
	ap_uint<8> tab;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab
	for (int i = 0; i < 256; i += 1)
		tab[i] = (symbol ^ i);
	ap_int<12+8> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst
	for (int i = 0; i < 256; i += 1)
		dst = src[ tab[i] ];
	return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
ap_int<12+7> vec_interleave_gf128(const ap_int<12+7> src[128], const ap_uint<7> symbol)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src
	ap_uint<7> tab;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab
	for (int i = 0; i < 128; i += 1)
		tab[i] = (symbol ^ i);
	ap_int<12+7> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst
	for (int i = 0; i < 128; i += 1)
		dst = src[ tab[i] ];
	return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
ap_int<12+6> vec_interleave_gf64(const ap_int<12+6> src[64], const ap_uint<6> symbol)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src
	ap_uint<6> tab;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab
	for (int i = 0; i < 64; i += 1)
		tab[i] = (symbol ^ i);
	ap_int<12+6> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst
	for (int i = 0; i < 64; i += 1)
		dst = src[ tab[i] ];
	return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
ap_int<12+5> vec_interleave_gf32(const ap_int<12+5> src[32], const ap_uint<5> symbol)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src
	ap_uint<5> tab;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab
	for (int i = 0; i < 32; i += 1)
		tab[i] = (symbol ^ i);
	ap_int<12+5> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst
	for (int i = 0; i < 32; i += 1)
		dst = src[ tab[i] ];
	return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
ap_int<12+4> vec_interleave_gf16(const ap_int<12+4> src[16], const ap_uint<4> symbol)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src
	ap_uint<4> tab;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab
	for (int i = 0; i < 16; i += 1)
		tab[i] = (symbol ^ i);
	ap_int<12+4> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst
	for (int i = 0; i < 16; i += 1)
		dst = src[ tab[i] ];
	return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
ap_int<12+3> vec_interleave_gf8(const ap_int<12+3> src[8], const ap_uint<3> symbol)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src
	ap_uint<3> tab;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab
	for (int i = 0; i < 8; i += 1)
		tab[i] = (symbol ^ i);
	ap_int<12+3> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst
	for (int i = 0; i < 8; i += 1)
		dst = src[ tab[i] ];
	return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
ap_int<12+2> vec_interleave_gf4(const ap_int<12+2> src[4], const ap_uint<2> symbol)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src
	ap_uint<2> tab;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab
	for (int i = 0; i < 4; i += 1)
		tab[i] = (symbol ^ i);
	ap_int<12+2> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst
	for (int i = 0; i < 4; i += 1)
		dst = src[ tab[i] ];
	return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
t_ram<14, 4> hls_fwht_gf4(const t_ram<14, 4> src, const ap_uint<2> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<14, 4> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave_gf4(src.value, symbol);
    return dst;
}
//
//
t_ram<15, 8> hls_fwht_gf8(const t_ram<15, 8> src, const ap_uint<3> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<15, 8> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave_gf8(src.value, symbol);
    return dst;
}
//
//
t_ram<16, 16> hls_fwht_gf16(const t_ram<16, 16> src, const ap_uint<4> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<16, 16> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave_gf16(src.value, symbol);
    return dst;
}
//
//
t_ram<17, 32> hls_fwht_gf32(const t_ram<17, 32> src, const ap_uint<5> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<17, 32> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave_gf32(src.value, symbol);
    return dst;
}
//
//
t_ram<18, 64> hls_fwht_gf64(const t_ram<18, 64> src, const ap_uint<6> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<18, 64> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave_gf64(src.value, symbol);
    return dst;
}
//
//
t_ram<19, 128> hls_fwht_gf128(const t_ram<19, 128> src, const ap_uint<7> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<19, 128> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave_gf128(src.value, symbol);
    return dst;
}
//
//
t_ram<20, 256> hls_fwht_gf256(const t_ram<20, 256> src, const ap_uint<8> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<20, 256> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave_gf256(src.value, symbol);
    return dst;
}
//
//
//
////////////////////////////////////////////////////////////////////////////
//
//
//
