#pragma once

#include "f_function_freq_in.hpp"
#include "f_function_proba_in.hpp"
#include "g_function_freq_in.hpp"
#include "g_function_proba_in.hpp"
#include "decoder_dedicated.hpp"
#include "middle_node_pruned_rep_after_f.hpp"
#include "middle_node_pruned_rep_after_g.hpp"
#include "middle_node_pruned_rate_0.hpp"
#include "middle_node_pruned_rate_1_after_f.hpp"
#include "middle_node_pruned_rate_1_after_g.hpp"
#include "middle_node_pruned_spc_after_f.hpp"
#include "middle_node_pruned_spc_after_g.hpp"

template <int gf_size>
void decoder_dedicated<gf_size>::execute(symbols_t * channel, uint16_t * decoded)
{
// NODE LEVEL (64)
	f_function_proba_in<512>(internal, channel, channel + 32, 32);
// NODE LEVEL (32)
	// f_function_freq_in<512>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<512>(decoded + 0, symbols + 0, 16);
	g_function_freq_in_after_rate_0<512>(internal + 32, internal + 0, internal + 16, 16);
// NODE LEVEL (16)
	f_function_proba_in<512>(internal + 48, internal + 32, internal + 40, 8);
	middle_node_pruned_rep_after_f<512>(internal + 48, decoded + 16, symbols + 16, 8);
	g_function_proba_in<512>(internal + 48, internal + 32, internal + 40, symbols + 16, 8);
// NODE LEVEL (8)
	f_function_proba_in<512>(internal + 56, internal + 48, internal + 52, 4);
// NODE LEVEL (4)
	// f_function_freq_in<512>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<512>(decoded + 24, symbols + 24, 2);
	g_function_freq_in_after_rate_0<512>(internal + 60, internal + 56, internal + 58, 2);
	middle_node_pruned_rate_1_after_g<512>(internal + 60, decoded + 26, symbols + 26, 2);
	for(int i = 0; i < 2; i += 1){
	  symbols[24 + i] ^= symbols[26 + i];
	}
	g_function_proba_in<512>(internal + 56, internal + 48, internal + 52, symbols + 24, 4);
	middle_node_pruned_rate_1_after_g<512>(internal + 56, decoded + 28, symbols + 28, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[24 + i] ^= symbols[28 + i];
	}
	for(int i = 0; i < 8; i += 1){
	  symbols[16 + i] ^= symbols[24 + i];
	}
	for(int i = 0; i < 16; i += 1){
	  symbols[0 + i] ^= symbols[16 + i];
	}
	g_function_proba_in<512>(internal, channel, channel + 32, symbols, 32);
// NODE LEVEL (32)
	f_function_proba_in<512>(internal + 32, internal + 0, internal + 16, 16);
// NODE LEVEL (16)
	f_function_freq_in<512>(internal + 48, internal + 32, internal + 40, 8);
// NODE LEVEL (8)
	// f_function_freq_in<512>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<512>(decoded + 32, symbols + 32, 4);
	g_function_freq_in_after_rate_0<512>(internal + 56, internal + 48, internal + 52, 4);
	middle_node_pruned_spc_after_g<512>(internal + 56, decoded + 36, symbols + 36, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[32 + i] ^= symbols[36 + i];
	}
	g_function_freq_in<512>(internal + 48, internal + 32, internal + 40, symbols + 32, 8);
	middle_node_pruned_spc_after_g<512>(internal + 48, decoded + 40, symbols + 40, 8);
	for(int i = 0; i < 8; i += 1){
	  symbols[32 + i] ^= symbols[40 + i];
	}
	g_function_proba_in<512>(internal + 32, internal + 0, internal + 16, symbols + 32, 16);
	middle_node_pruned_spc_after_g<512>(internal + 32, decoded + 48, symbols + 48, 16);
	for(int i = 0; i < 16; i += 1){
	  symbols[32 + i] ^= symbols[48 + i];
	}
}

template <int gf_size>
const int decoder_dedicated<gf_size>::N_gen = 64;

template <int gf_size>
const int decoder_dedicated<gf_size>::K_gen = 32;

