//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "f_argmax3.hpp"
#include "f_argmax2.hpp"
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
template<int W, int B>
inline gtuple<W, B> f_max(const gtuple<W, B> a, const gtuple<W, B> b)
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
template<int W = 12>
qtuple<W, 2> argmax3_gf4(const t_ram<W, 4> inp)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
	//
	const gtuple<W, 2> stg1 = argmax2_gf4(inp);
	t_ram<W, 4> upd;
	for (int i = 0; i < 4; i += 1)
		upd.value[i] = (stg1.index == i) ? (ap_int<W>)0 : inp.value[i];
	const gtuple<W, 2> stg2 = argmax2_gf4(upd);
	//
	const qtuple<W, 2> r = {stg1.index, stg1.value, stg2.index, stg2.value};
	//
	return r;
}
//
//
template<int W = 12>
qtuple<W, 3> argmax3_gf8(const t_ram<W, 8> inp)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
	//
	const gtuple<W, 3> stg1 = argmax2_gf8(inp);
	t_ram<W, 8> upd;
	for (int i = 0; i < 8; i += 1)
		upd.value[i] = (stg1.index == i) ? (ap_int<W>)0 : inp.value[i];
	const gtuple<W, 3> stg2 = argmax2_gf8(upd);
	//
	const qtuple<W, 3> r = {stg1.index, stg1.value, stg2.index, stg2.value};
	//
	return r;
}
//
//
template<int W = 12>
qtuple<W, 4> argmax3_gf16(const t_ram<W, 16> inp)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
	//
	const gtuple<W, 4> stg1 = argmax2_gf16(inp);
	t_ram<W, 16> upd;
	for (int i = 0; i < 16; i += 1)
		upd.value[i] = (stg1.index == i) ? (ap_int<W>)0 : inp.value[i];
	const gtuple<W, 4> stg2 = argmax2_gf16(upd);
	//
	const qtuple<W, 4> r = {stg1.index, stg1.value, stg2.index, stg2.value};
	//
	return r;
}
//
//
template<int W = 12>
qtuple<W, 5> argmax3_gf32(const t_ram<W, 32> inp)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
	//
	const gtuple<W, 5> stg1 = argmax2_gf32(inp);
	t_ram<W, 32> upd;
	for (int i = 0; i < 32; i += 1)
		upd.value[i] = (stg1.index == i) ? (ap_int<W>)0 : inp.value[i];
	const gtuple<W, 5> stg2 = argmax2_gf32(upd);
	//
	const qtuple<W, 5> r = {stg1.index, stg1.value, stg2.index, stg2.value};
	//
	return r;
}
//
//
template<int W = 12>
qtuple<W, 6> argmax3_gf64(const t_ram<W, 64> inp)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
	//
	const gtuple<W, 6> stg1 = argmax2_gf64(inp);
	t_ram<W, 64> upd;
	for (int i = 0; i < 64; i += 1)
		upd.value[i] = (stg1.index == i) ? (ap_int<W>)0 : inp.value[i];
	const gtuple<W, 6> stg2 = argmax2_gf64(upd);
	//
	const qtuple<W, 6> r = {stg1.index, stg1.value, stg2.index, stg2.value};
	//
	return r;
}
//
//
template<int W = 12>
qtuple<W, 7> argmax3_gf128(const t_ram<W, 128> inp)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
	//
	const gtuple<W, 7> stg1 = argmax2_gf128(inp);
	t_ram<W, 128> upd;
	for (int i = 0; i < 128; i += 1)
		upd.value[i] = (stg1.index == i) ? 0 : inp.value[i];
	const gtuple<W, 7> stg2 = argmax2_gf128(upd);
	//
	const qtuple<W, 7> r = {stg1.index, stg1.value, stg2.index, stg2.value};
	//
	return r;
}
//
//
template<int W = 12>
qtuple<W, 8> argmax3_gf256(const t_ram<W, 256> inp)
{
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
	//
	const gtuple<W, 8> stg1 = argmax2_gf256(inp);
	t_ram<W, 256> upd;
	for (int i = 0; i < 256; i += 1)
		upd.value[i] = (stg1.index == i) ? 0 : inp.value[i];
	const gtuple<W, 8> stg2 = argmax2_gf256(upd);
	//
	const qtuple<W, 8> r = {stg1.index, stg1.value, stg2.index, stg2.value};
	//
	return r;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
qtuple<12, 2> hls_argmax3_gf4(const t_ram<12, 4> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
	return argmax3_gf4<12>(src);
}
//
//
qtuple<12, 3> hls_argmax3_gf8(const t_ram<12, 8> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
	return argmax3_gf8<12>(src);
}
//
//
qtuple<12, 4> hls_argmax3_gf16(const t_ram<12, 16> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
	return argmax3_gf16<12>(src);
}
//
//
qtuple<12, 5> hls_argmax3_gf32(const t_ram<12, 32> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
	return argmax3_gf32<12>(src);
}
//
//
qtuple<12, 6> hls_argmax3_gf64(const t_ram<12, 64> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
	return argmax3_gf64<12>(src);
}
//
//
//
////////////////////////////////////////////////////////////////////////////
//
//
//
