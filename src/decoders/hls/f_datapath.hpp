#pragma once
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "types.hpp"
#include "f_extend.hpp"
#include "f_fwht.hpp"
#include "f_mult.hpp"
#include "f_norm.hpp"
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

	const auto norm_in_a   = vec_i_mul_g(mult_in_e, mult_in_f, symbol); // f_mode
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
