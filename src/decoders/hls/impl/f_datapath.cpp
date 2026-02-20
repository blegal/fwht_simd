//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "f_datapath.hpp"
#include "f_fwht.hpp"
#include "f_extend.hpp"
#include "f_mult.hpp"
#include "f_norm.hpp"
#include "f_argmax.hpp"
#include "f_argmax2.hpp"
#include "f_truncate.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
template<int W, int log2GF>
struct gresult{
	t_i_memo        result;
	ap_uint<log2GF> index;
	ap_int <W>      proba;
};
//
//
template<int W, int log2GF>
struct qresult{
	t_i_memo        result;
	ap_uint<log2GF> idx_1;
	ap_int <W>      prob_1;
	ap_uint<log2GF> idx_2;
	ap_int <W>      prob_2;
};
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
t_i_memo datapath(
	const t_i_memo lwht_in_a,
	const t_i_memo lwht_in_b,
	const uint8_t symbol,
	const bool en_lwth)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_b.value

	t_uint6b tab;
	const t_o_lwht mult_in_a = fwht( cast(lwht_in_a) );
	const t_o_lwht mult_in_b = fwht( cast(lwht_in_b) );

	const t_o_lwht mult_in_c = extend( lwht_in_a );
	const t_o_lwht mult_in_d = extend( lwht_in_b );

	const t_o_lwht mult_in_e = en_lwth ? mult_in_a : mult_in_c; //
	const t_o_lwht mult_in_f = en_lwth ? mult_in_b : mult_in_d; //

	const t_o_mult norm_in_a = vec_i_mul_g( cast(mult_in_e), cast(mult_in_f), symbol); // f_mode
	const t_o_norm norm_ou_a = vec_i_norm( cast(norm_in_a) );
	const t_i_memo memo_in_a = cast(norm_ou_a);

#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_b.value

#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_c.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_d.value

#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_e.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_f.value

#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=norm_in_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=norm_ou_a.value
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
template<int W = 12, int log2GF>
gresult<W, log2GF> full_datapath(
	const t_i_memo   lwht_in_a,       // input 1
	const t_i_memo   lwht_in_b,       // input 2
	const uint8_t    symbol_i,        // symbol for g computation
	const bool       en_lwth          // should we apply FWHT on  1 & 2
    )
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_b.value

	t_uint6b tab;
	const t_o_lwht mult_in_a = fwht( cast(lwht_in_a) );
	const t_o_lwht mult_in_b = fwht( cast(lwht_in_b) );

	const t_o_lwht mult_in_c = extend( lwht_in_a );
	const t_o_lwht mult_in_d = extend( lwht_in_b );

	const t_o_lwht mult_in_e= en_lwth ? mult_in_a : mult_in_c; //
	const t_o_lwht mult_in_f= en_lwth ? mult_in_b : mult_in_d; //

	const t_o_mult norm_in_a = vec_i_mul_g( cast(mult_in_e), cast(mult_in_f), symbol_i); // f_mode
	const t_o_norm norm_ou_a = vec_i_norm( cast(norm_in_a) );

	gresult<W, log2GF> resu;
	resu.result = cast(norm_ou_a);

	//
	//
	//
	const t_i_memo argm_in_a  = truncate(mult_in_e);
	const tuple tt            = vec_i_argmax2(argm_in_a);
	resu.proba = tt.value;
	resu.index = tt.index;

	return resu;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_b.value

#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_c.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_d.value

#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_e.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=mult_in_f.value

#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=norm_in_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=norm_ou_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=argm_in_a.value
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
