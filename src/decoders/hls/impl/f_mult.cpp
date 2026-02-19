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
// Multiplication terme à terme sans prendre en compte l'entrelacement (PI)
//
template<int W = 18, int GF>
t_ram<W+W, GF> vec_mul(const t_ram<W, GF> src_1, const t_ram<W, GF> src_2)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value
	t_ram<W+W, GF> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < GF; i += 1)
		dst.value[i] = src_1.value[i] * src_2.value[i];
	return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
t_o_mult vec_i_mul_g(const t_i_mult src_1, const t_i_mult src_2, const uint8_t symbol)
{
#ifdef _COMPONENT_
#pragma HLS PIPELINE
#else
#pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value

	t_uint6b tab;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab.value
	for (int i = 0; i < gf_size; i += 1)
	{
		tab.value[i] = (symbol ^ i);
	}
	t_o_mult dst;
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
template<int W = 18>
t_ram<W+W, 64> mul_g_gf64(const t_ram<W, 64> src_1, const t_ram<W, 64> src_2, const ap_uint<6> symbol)
{
	const int GF_SIZE = 64;
	const int LOG2_GF =  6;
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value

	ap_uint<LOG2_GF> tab[GF_SIZE];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab
	for (int i = 0; i < GF_SIZE; i += 1)
	{
		tab[i] = (symbol ^ i);
	}
	t_ram<W+W, 64> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < GF_SIZE; i += 1)
	{
		dst.value[i] = src_1.value[ tab[i] ] * src_2.value[i];
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
template<int W = 17>
t_ram<W+W, 32> mul_g_gf32(const t_ram<W, 32> src_1, const t_ram<W, 32> src_2, const ap_uint<5> symbol)
{
	const int GF_SIZE = 32;
	const int LOG2_GF =  5;
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value

	ap_uint<LOG2_GF> tab[GF_SIZE];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab
	for (int i = 0; i < GF_SIZE; i += 1)
	{
		tab[i] = (symbol ^ i);
	}
	t_ram<W+W, 32> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < GF_SIZE; i += 1)
	{
		dst.value[i] = src_1.value[ tab[i] ] * src_2.value[i];
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
template<int W = 16>
t_ram<W+W, 16> mul_g_gf16(const t_ram<W, 16> src_1, const t_ram<W, 16> src_2, const ap_uint<4> symbol)
{
	const int GF_SIZE = 16;
	const int LOG2_GF =  4;
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value

	ap_uint<LOG2_GF> tab[GF_SIZE];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab
	for (int i = 0; i < GF_SIZE; i += 1)
	{
		tab[i] = (symbol ^ i);
	}
	t_ram<W+W, 16> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < GF_SIZE; i += 1)
	{
		dst.value[i] = src_1.value[ tab[i] ] * src_2.value[i];
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
template<int W = 15>
t_ram<W+W, 8> mul_g_gf8(const t_ram<W, 8> src_1, const t_ram<W, 8> src_2, const ap_uint<3> symbol)
{
	const int GF_SIZE = 8;
	const int LOG2_GF = 3;
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value

	ap_uint<LOG2_GF> tab[GF_SIZE];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab
	for (int i = 0; i < GF_SIZE; i += 1)
	{
		tab[i] = (symbol ^ i);
	}
	t_ram<W+W, 8> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < GF_SIZE; i += 1)
	{
		dst.value[i] = src_1.value[ tab[i] ] * src_2.value[i];
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
template<int W = 14>
t_ram<W+W, 4> mul_g_gf4(const t_ram<W, 4> src_1, const t_ram<W, 4> src_2, const ap_uint<2> symbol)
{
	const int GF_SIZE = 4;
	const int LOG2_GF = 2;
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value

	ap_uint<LOG2_GF> tab[GF_SIZE];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab
	for (int i = 0; i < GF_SIZE; i += 1)
	{
		tab[i] = (symbol ^ i);
	}
	t_ram<W+W, 4> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < GF_SIZE; i += 1)
	{
		dst.value[i] = src_1.value[ tab[i] ] * src_2.value[i];
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
t_ram<40, 256> hls_mul_gf256(const t_ram<20, 256> src_1, const t_ram<20, 256> src_2)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value
	t_ram<40, 256> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	dst = vec_mul<20, 256>(src_1, src_2);
	return dst;
}
//
//
t_ram<38, 128> hls_mul_gf128(const t_ram<19, 128> src_1, const t_ram<19, 128> src_2)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value
	t_ram<38, 128> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	dst = vec_mul<19, 128>(src_1, src_2);
	return dst;
}
//
//
t_ram<36, 64> hls_mul_gf64(const t_ram<18, 64> src_1, const t_ram<18, 64> src_2)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value
	t_ram<36, 64> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	dst = vec_mul<18, 64>(src_1, src_2);
	return dst;
}
//
//
t_ram<34, 32> hls_mul_gf32(const t_ram<17, 32> src_1, const t_ram<17, 32> src_2, const ap_uint<5> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value
    t_ram<34, 32> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_mul<17, 32>(src_1, src_2);
    return dst;
}
//
//
t_ram<32, 16> hls_mul_gf16(const t_ram<16, 16> src_1, const t_ram<16, 16> src_2)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value
    t_ram<32, 16> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_mul<16, 16>(src_1, src_2);
    return dst;
}
//
//
t_ram<30, 8> hls_mul_gf8(const t_ram<15, 8> src_1, const t_ram<15, 8> src_2)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value
    t_ram<30, 8> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_mul<15, 8>(src_1, src_2);
    return dst;
}
//
//
t_ram<28, 4> hls_mul_gf4(const t_ram<14, 4> src_1, const t_ram<14, 4> src_2)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_1.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src_2.value
    t_ram<28, 4> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_mul<14, 4>(src_1, src_2);
    return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
