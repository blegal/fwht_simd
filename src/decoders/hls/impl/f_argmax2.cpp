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
tuple vec_i_argmax2(const t_i_memo inp) {
#ifdef _COMPONENT_
	#pragma HLS PIPELINE
#else
	#pragma HLS INLINE
#endif
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
	return s6;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
template<int W = 12>
gtuple<W, 6> argmax2_gf64(const t_ram<W, 64> inp)
{
#ifdef _COMPONENT_
	#pragma HLS PIPELINE
#else
	#pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
	//
	gtuple<W, 6> s1[32];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
	for (int i = 0; i < 64; i += 2) // 64
	{
#pragma HLS UNROLL
		const gtuple<W, 6> A = {inp.value[  i], i  };
		const gtuple<W, 6> B = {inp.value[i+1], i+1};
		s1[i >> 1] = f_max(A, B);
	}
	//
	gtuple<W, 6> s2[16];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s2
	s2[ 0] = f_max(s1[ 0], s1[ 1]);
	s2[ 1] = f_max(s1[ 2], s1[ 3]);
	s2[ 2] = f_max(s1[ 4], s1[ 5]);
	s2[ 3] = f_max(s1[ 6], s1[ 7]);
	s2[ 4] = f_max(s1[ 8], s1[ 9]);
	s2[ 5] = f_max(s1[10], s1[11]);
	s2[ 6] = f_max(s1[12], s1[13]);
	s2[ 7] = f_max(s1[14], s1[15]);
	s2[ 8] = f_max(s1[16], s1[17]);
	s2[ 9] = f_max(s1[18], s1[19]);
	s2[10] = f_max(s1[20], s1[21]);
	s2[11] = f_max(s1[22], s1[23]);
	s2[12] = f_max(s1[24], s1[25]);
	s2[13] = f_max(s1[26], s1[27]);
	s2[14] = f_max(s1[28], s1[29]);
	s2[15] = f_max(s1[30], s1[31]);

	gtuple<W, 6> s3[8];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s3
	s3[0] = f_max(s2[ 0], s2[ 1]);
	s3[1] = f_max(s2[ 2], s2[ 3]);
	s3[2] = f_max(s2[ 4], s2[ 5]);
	s3[3] = f_max(s2[ 6], s2[ 7]);
	s3[4] = f_max(s2[ 8], s2[ 9]);
	s3[5] = f_max(s2[10], s2[11]);
	s3[6] = f_max(s2[12], s2[13]);
	s3[7] = f_max(s2[14], s2[15]);
	//
	gtuple<W, 6> s4[4];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s4
	s4[0] = f_max(s3[0], s3[1]);
	s4[1] = f_max(s3[2], s3[3]);
	s4[2] = f_max(s3[4], s3[5]);
	s4[3] = f_max(s3[6], s3[7]);
	//
	gtuple<W, 6> s5[2];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s5
	s5[0] = f_max(s4[0], s4[1]);
	s5[1] = f_max(s4[2], s4[3]);
	//
	gtuple<W, 6> s6;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s6
	s6 = f_max(s5[0], s5[1]);
	//
	return s6;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
template<int W = 12>
gtuple<W, 5> argmax2_gf32(const t_ram<W, 32> inp)
{
#ifdef _COMPONENT_
#pragma HLS PIPELINE
#else
#pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
	//
	gtuple<W, 5> s1[16];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
	for (int i = 0; i < 32; i += 2) // 64
	{
#pragma HLS UNROLL
		const gtuple<W, 5> A = {inp.value[  i], i  };
		const gtuple<W, 5> B = {inp.value[i+1], i+1};
		s1[i >> 1] = f_max(A, B);
	}
	//
	gtuple<W, 5> s2[8];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s2
	s2[0] = f_max(s1[ 0], s1[ 1]);
	s2[1] = f_max(s1[ 2], s1[ 3]);
	s2[2] = f_max(s1[ 4], s1[ 5]);
	s2[3] = f_max(s1[ 6], s1[ 7]);
	s2[4] = f_max(s1[ 8], s1[ 9]);
	s2[5] = f_max(s1[10], s1[11]);
	s2[6] = f_max(s1[12], s1[13]);
	s2[7] = f_max(s1[14], s1[15]);
	//
	gtuple<W, 5> s3[4];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s3
	s3[0] = f_max(s2[0], s2[1]);
	s3[1] = f_max(s2[2], s2[3]);
	s3[2] = f_max(s2[4], s2[5]);
	s3[3] = f_max(s2[6], s2[7]);
	//
	gtuple<W, 5> s4[2];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s4
	s4[0] = f_max(s3[0], s3[1]);
	s4[1] = f_max(s3[2], s3[3]);
	//
	gtuple<W, 5> s5;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s5
	s5 = f_max(s4[0], s4[1]);
	//
	return s5;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
template<int W = 12>
gtuple<W, 4> argmax2_gf16(const t_ram<W, 16> inp)
{
#ifdef _COMPONENT_
	#pragma HLS PIPELINE
#else
	#pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
	//
	//
	gtuple<W, 4> s1[8];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
	for (int i = 0; i < 16; i += 2)  // 32
	{
#pragma HLS UNROLL
		const gtuple<W, 4> A = {inp.value[  i], i  };
		const gtuple<W, 4> B = {inp.value[i+1], i+1};
		s1[i >> 1] = f_max(A, B);
	}
	//
	gtuple<W, 4> s2[4];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s2
	s2[0] = f_max(s1[0], s1[1]);
	s2[1] = f_max(s1[2], s1[3]);
	s2[2] = f_max(s1[4], s1[5]);
	s2[3] = f_max(s1[6], s1[7]);
	//
	gtuple<W, 4> s3[2];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s3
	s3[0] = f_max(s2[0], s2[1]);
	s3[1] = f_max(s2[2], s2[3]);
	//
	gtuple<W, 4> s4;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s4
	s4 = f_max(s3[0], s3[1]);
	//
	return s4;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
template<int W = 12>
gtuple<W, 3> argmax2_gf8(const t_ram<W, 8> inp)
{
#ifdef _COMPONENT_
	#pragma HLS PIPELINE
#else
	#pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
	//
	//
	gtuple<W, 3> s1[4];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
	for (int i = 0; i < 8; i += 2)  // 16
	{
#pragma HLS UNROLL
		const gtuple<W, 3> A = {inp.value[  i], i  };
		const gtuple<W, 3> B = {inp.value[i+1], i+1};
		s1[i >> 1] = f_max(A, B);
	}
	//
	//
	gtuple<W, 3> s2[2];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s2
	s2[0] = f_max(s1[0], s1[1]);
	s2[1] = f_max(s1[2], s1[3]);
	//
	gtuple<W, 3> s3;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s3
	s3 = f_max(s2[0], s2[1]);
	//
	//
	return s3;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
template<int W = 12>
gtuple<W, 2> argmax2_gf4(const t_ram<W, 4> inp)
{
#ifdef _COMPONENT_
	#pragma HLS PIPELINE
#else
	#pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
	//
	//
	gtuple<W, 2> s1[2];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s1
	for (int i = 0; i < 4; i += 2)  // 16
	{
#pragma HLS UNROLL
		const gtuple<W, 2> A = {inp.value[  i], i  };
		const gtuple<W, 2> B = {inp.value[i+1], i+1};
		s1[i >> 1] = f_max(A, B);
	}
	//
	//
	gtuple<W, 2> s2;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=s2
	s2 = f_max(s1[0], s1[1]);
	//
	//
	return s2;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
gtuple<12, 2> hls_argmax2_gf4(const t_ram<12, 4> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
	return argmax2_gf4<12>(src);
}
//
//
gtuple<12, 3> hls_argmax2_gf8(const t_ram<12, 8> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
	return argmax2_gf8<12>(src);
}
//
//
gtuple<12, 4> hls_argmax2_gf16(const t_ram<12, 16> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
	return argmax2_gf16<12>(src);
}
//
//
gtuple<12, 5> hls_argmax2_gf32(const t_ram<12, 32> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
	return argmax2_gf32<12>(src);
}
//
//
gtuple<12, 6> hls_argmax2_gf64(const t_ram<12, 64> src)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src.value complete
	return argmax2_gf64<12>(src);
}
//
//
//
////////////////////////////////////////////////////////////////////////////
//
//
//
