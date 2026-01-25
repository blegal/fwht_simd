//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "f_extend.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
t_o_lwht extend(const t_i_memo src)
{
#ifdef _COMPONENT_
    #pragma HLS PIPELINE
#else
    #pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value
	t_o_lwht dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
	for (int i = 0; i < gf_size; i++) {
		dst.value[i] = src.value[i];
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
