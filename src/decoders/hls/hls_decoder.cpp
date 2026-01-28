//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "impl/types.hpp"
#include "impl/f_datapath.hpp"
#include "impl/f_decision.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
void the_decoder(
			t_i_memo channel[16],
			uint16_t decoded[16])
{
	t_i_memo internal[16];
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=internal

	uint8_t  symbols [16];

	t_i_memo lwht_in_a, lwht_in_b;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_b.value

	t_i_memo memo_in_a;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=memo_in_a.value

	// operation (Fn, Gn, Fn+LWHT, Gn+Fn+LWHT, F0, G1, G1+Fn+LWHT, XOR)
	// counter
	// - n symbols
	// - Channel  RAM (read 1, read 2)
	// - Internal RAM (read 1, read 2, write 1)
	// - Symbol   RAM (read 1, read 2, write 1)
	// - Decoded  RAM (read 1, read 2, write 1)

	uint8_t s;
	uint8_t i;

	//
	// f_function_proba_in<64>(internal, channel, channel + 8, 8);
	loop1 : for (s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = channel[s    ];
    	lwht_in_b   = channel[s + 8];
    	const auto memo_in_a = datapath(lwht_in_a, lwht_in_b, 0, true);
        internal[s] = memo_in_a;
    }

	//
	// middle_node_pruned_rate_0<gf_size>(decoded + 0, symbols + 0, 4);
    // TO DO !
	loop2 : for (s = 0; s < 4; s++) {
#pragma HLS PIPELINE
		symbols[s + 0] = 0;
		decoded[s + 0] = 0;
    }

	//
	// g_function_freq_in_after_rate_0<gf_size>(internal + 8, internal + 0, internal + 4, 4);
	loop3 : for (s = 0; s < 4; s++) {
#pragma HLS PIPELINE
		lwht_in_a   = internal[s + 0];
		lwht_in_b   = internal[s + 4];
		memo_in_a = datapath(lwht_in_a, lwht_in_b, 0, true);
        internal[s + 8] = memo_in_a;
    }


	//
	// middle_node_pruned_rate_0<gf_size>(decoded + 4, symbols + 4, 2);
    // TO DO !
	loop4 : for (s = 0; s < 2; s++) {
#pragma HLS PIPELINE
		symbols[s + 4] = 0;
		decoded[s + 4] = 0;
    }

	//
	// g_function_proba_in_after_rate_0<gf_size>(internal + 12, internal + 8, internal + 10, 2);
	loop5 : for (s = 0; s < 2; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s +  8];
    	lwht_in_b   = internal[s + 10];
    	const auto memo_in_a = datapath(lwht_in_a, lwht_in_b, 0, false);
    	internal[s + 12] = memo_in_a;
    }

	//
	// leaf_node_rate_0<gf_size>(decoded + 6, symbols + 6);
    symbols[6] = 0;
    decoded[6] = 0;

    //
	// g_function_proba_in_after_rate_0<gf_size>(internal + 14, internal + 12, internal + 13, 1);
    loop6 : for (s = 0; s < 1; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 12];
    	lwht_in_b   = internal[s + 13];
    	const auto memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    	internal[s + 14] = memo_in_a;
    }

	//
	// leaf_node_after_g<gf_size>(internal + 14, decoded + 7, symbols + 7);
    uint8_t e = vec_decision( internal[14], false );
    symbols[7] = e;
    decoded[7] = e;

    loop7 : for(i = 0; i < 1; i += 1){
#pragma HLS PIPELINE
	  symbols[6 + i] ^= symbols[7 + i];
	}
    loop8 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
	  symbols[4 + i] ^= symbols[6 + i];
	}
    loop9 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
	  symbols[0 + i] ^= symbols[4 + i];
	}

	//
	// g_function_proba_in<gf_size>(internal, channel, channel + 8, symbols, 8);
    loop10 : for (s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = channel[s    ];
    	lwht_in_b   = channel[s + 8];
    	const auto memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    	internal[s] = memo_in_a;
    }

	//
	// f_function_proba_in<gf_size>(internal + 8, internal + 0, internal + 4, 4); // dst, src_1, src_2
    loop11 : for (s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s    ];
    	lwht_in_b   = internal[s + 4];
    	const auto memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    	internal[s + 8] = memo_in_a;
    }

	//
	// f_function_freq_in<64>(internal + 12, internal + 8, internal + 10, 2);
    loop12 : for (s = 0; s < 2; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s +  8];
    	lwht_in_b   = internal[s + 10];
    	const auto memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    	internal[s + 12] = memo_in_a;
    }

	//
	// leaf_node_rate_0<gf_size>(decoded + 8, symbols + 8);
	symbols[8] = 0;
	decoded[8] = 0;

	//
	// g_function_freq_in_after_rate_0<gf_size>(internal + 14, internal + 12, internal + 13, 1);
	loop13 : for (s = 0; s < 1; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 12];
    	lwht_in_b   = internal[s + 13];
    	const auto memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    	internal[s + 14] = memo_in_a;
    }

	//
	// leaf_node_after_g<gf_size>(internal + 14, decoded + 9, symbols + 9);
    e = vec_decision( internal[14], false );
    symbols[9] = e;
    decoded[9] = e;

    loop14 : for(i = 0; i < 1; i += 1){
#pragma HLS PIPELINE
	  symbols[8 + i] ^= symbols[9 + i];
	}

	//
	// g_function_freq_in<gf_size>(internal + 12, internal + 8, internal + 10, symbols + 8, 2);
    loop15 : for (s = 0; s < 2; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s +  8];
    	lwht_in_b   = internal[s + 10];
    	const auto memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[s + 8], true);
    	internal[s + 12] = memo_in_a;
    }

	//
	// middle_node_pruned_rate_1_after_g<gf_size>(internal + 12, decoded + 10, symbols + 10, 2);
    loop16 : for (s = 0; s < 2; s++) {
#pragma HLS PIPELINE
    	e = vec_decision( internal[s + 12], false );
    	symbols[s + 10] = e;
    	decoded[s + 10] = e;
    }

    loop17 : for(i = 0; i < 2; i += 1){
	  symbols[8 + i] ^= symbols[10 + i];
	}

	//
	// g_function_proba_in<gf_size>(internal + 8, internal + 0, internal + 4, symbols + 8, 4);
    loop18 : for (s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 0];
    	lwht_in_b   = internal[s + 4];
    	const auto memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[s + 8], false);
    	internal[s + 8] = memo_in_a;
    }

	//
	// middle_node_pruned_rate_1_after_g<gf_size>(internal + 8, decoded + 12, symbols + 12, 4);
    loop19 : for (s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    	e = vec_decision( internal[s + 8], false );
    	symbols[s + 12] = e;
    	decoded[s + 12] = e;
    }
}

