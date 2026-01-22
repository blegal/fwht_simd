//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "types.hpp"
#include "f_argmax.hpp"
#include "f_fwht.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
inline t_i_memo truncate(const t_o_lwht src) {
#pragma HLS INLINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value
	t_i_memo dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < gf_size; i++) {
		dst.value[i] = src.value[i].range(o_lwht_width-1, o_lwht_width-data_width-1);
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
static uint8_t vec_decision(const t_i_memo inp, const bool en_lwth)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE
	//
	//
	const t_o_lwht lwht_ou_a = fwht( cast(inp) );
	const t_i_memo argm_in_a = en_lwth ? truncate(lwht_ou_a) : inp;
	const uint8_t resultat   = vec_i_unroll_argmax(argm_in_a);
	//
	//
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=inp.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_ou_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=argm_in_a.value
	//
	//
	return resultat;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
