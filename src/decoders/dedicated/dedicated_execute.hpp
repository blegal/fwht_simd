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
	f_function_proba_in<64>(internal, channel, channel + 32, 32);
// NODE LEVEL (32)
	f_function_freq_in<64>(internal + 32, internal + 0, internal + 16, 16);
// NODE LEVEL (16)
	f_function_freq_in<64>(internal + 48, internal + 32, internal + 40, 8);
	middle_node_pruned_rep_after_f<64>(internal + 48, decoded + 0, symbols + 0, 8);
	g_function_freq_in<64>(internal + 48, internal + 32, internal + 40, symbols + 0, 8);
// NODE LEVEL (8)
	f_function_proba_in<64>(internal + 56, internal + 48, internal + 52, 4);
	middle_node_pruned_rep_after_f<64>(internal + 56, decoded + 8, symbols + 8, 4);
	g_function_proba_in<64>(internal + 56, internal + 48, internal + 52, symbols + 8, 4);
	middle_node_pruned_spc_after_g<64>(internal + 56, decoded + 12, symbols + 12, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[8 + i] ^= symbols[12 + i];
	}
	for(int i = 0; i < 8; i += 1){
	  symbols[0 + i] ^= symbols[8 + i];
	}
	g_function_freq_in<64>(internal + 32, internal + 0, internal + 16, symbols + 0, 16);
// NODE LEVEL (16)
	f_function_proba_in<64>(internal + 48, internal + 32, internal + 40, 8);
// NODE LEVEL (8)
	f_function_freq_in<64>(internal + 56, internal + 48, internal + 52, 4);
	middle_node_pruned_rep_after_f<64>(internal + 56, decoded + 16, symbols + 16, 4);
	g_function_freq_in<64>(internal + 56, internal + 48, internal + 52, symbols + 16, 4);
	middle_node_pruned_spc_after_g<64>(internal + 56, decoded + 20, symbols + 20, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[16 + i] ^= symbols[20 + i];
	}
	g_function_proba_in<64>(internal + 48, internal + 32, internal + 40, symbols + 16, 8);
	middle_node_pruned_rate_1_after_g<64>(internal + 48, decoded + 24, symbols + 24, 8);
	for(int i = 0; i < 8; i += 1){
	  symbols[16 + i] ^= symbols[24 + i];
	}
	for(int i = 0; i < 16; i += 1){
	  symbols[0 + i] ^= symbols[16 + i];
	}
	g_function_proba_in<64>(internal, channel, channel + 32, symbols, 32);
	middle_node_pruned_spc_after_g<64>(internal + 0, decoded + 32, symbols + 32, 32);
}

template <int gf_size>
const int decoder_dedicated<gf_size>::N_gen = 64;

template <int gf_size>
const int decoder_dedicated<gf_size>::K_gen = 48;

