//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "f_truncate.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
t_i_memo truncate(const t_o_lwht src) {
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
