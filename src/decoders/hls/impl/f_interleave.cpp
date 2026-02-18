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
template<int W, int q, int log2q>
t_ram<W+log2q, q> vec_interleave(const t_ram<W+log2q, q> src, const ap_uint<log2q> symbol)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value
	ap_uint<log2q> tab;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=tab
	for (int i = 0; i < q; i += 1)
		tab[i] = (symbol ^ i);
	t_ram<W+log2q, q> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < q; i += 1)
		dst.value[i] = src.value[ tab[i] ];
	return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
t_ram<14, 4> hls_interleaver_gf4(const t_ram<14, 4> src, const ap_uint<2> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<14, 4> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave<12, 4, 2>(src, symbol);
    return dst;
}
//
//
t_ram<15, 8> hls_interleaver_gf8(const t_ram<15, 8> src, const ap_uint<3> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<15, 8> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave<12, 8, 3>(src, symbol);
    return dst;
}
//
//
t_ram<16, 16> hls_interleaver_gf16(const t_ram<16, 16> src, const ap_uint<4> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<16, 16> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave<12, 16, 4>(src, symbol);
    return dst;
}
//
//
t_ram<17, 32> hls_interleaver_gf32(const t_ram<17, 32> src, const ap_uint<5> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<17, 32> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave<12, 32, 5>(src, symbol);
    return dst;
}
//
//
t_ram<18, 64> hls_interleaver_gf64(const t_ram<18, 64> src, const ap_uint<6> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<18, 64> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave<12, 64, 6>(src, symbol);
    return dst;
}
//
//
t_ram<19, 128> hls_interleaver_gf128(const t_ram<19, 128> src, const ap_uint<7> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<19, 128> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave<12, 128, 7>(src, symbol);
    return dst;
}
//
//
t_ram<20, 256> hls_interleaver_gf256(const t_ram<20, 256> src, const ap_uint<8> symbol)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
    t_ram<20, 256> dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    dst = vec_interleave<12, 256, 8>(src, symbol);
    return dst;
}
//
//
//
////////////////////////////////////////////////////////////////////////////
//
//
//
