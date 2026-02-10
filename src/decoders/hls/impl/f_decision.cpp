//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "f_decision.hpp"
#include "f_argmax.hpp"
#include "f_fwht.hpp"
#include "f_truncate.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
uint8_t vec_decision(const t_i_memo inp, const bool en_lwth)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE
	//
	//
	const t_o_lwht lwht_ou_a = fwht( cast(inp) );
	const t_i_memo argm_in_a = en_lwth ? truncate(lwht_ou_a) : inp;
	const uint8_t resultat   = vec_i_argmax(argm_in_a);
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
