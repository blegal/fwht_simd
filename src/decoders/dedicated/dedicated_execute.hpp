#pragma once

#include "decoder_dedicated.hpp"
#include "impl/f_function.hpp"
#include "impl/g_function.hpp"
#include "impl/node_rate_0.hpp"
#include "impl/node_rate_1.hpp"
#include "impl/node_rep.hpp"
#include "impl/node_spc.hpp"

template <int gf_size>
void decoder_dedicated<gf_size>::execute(void* s_channel, uint16_t * decoded)
{
        symbols_s<gf_size>* channel = static_cast< symbols_s<gf_size>* >(s_channel);
// NODE LEVEL (128)
	f_function_proba_in<128>(internal, channel, channel + 64, 64);
// NODE LEVEL (64)
	f_function_freq_in<128>(internal + 64, internal + 0, internal + 32, 32);
// NODE LEVEL (32)
	f_function_freq_in<128>(internal + 96, internal + 64, internal + 80, 16);
// NODE LEVEL (16)
	f_function_freq_in<128>(internal + 112, internal + 96, internal + 104, 8);
// NODE LEVEL (8)
	f_function_freq_in<128>(internal + 120, internal + 112, internal + 116, 4);
// NODE LEVEL (4)
	f_function_freq_in<128>(internal + 124, internal + 120, internal + 122, 2);
	middle_node_pruned_rep_after_f<128>(internal + 124, decoded + 0, symbols + 0, 2);
	g_function_freq_in<128>(internal + 124, internal + 120, internal + 122, symbols + 0, 2);
	middle_node_pruned_spc_after_g<128>(internal + 124, decoded + 2, symbols + 2, 2);
	for(int i = 0; i < 2; i += 1){
	  symbols[0 + i] ^= symbols[2 + i];
	}
	g_function_freq_in<128>(internal + 120, internal + 112, internal + 116, symbols + 0, 4);
	middle_node_pruned_spc_after_g<128>(internal + 120, decoded + 4, symbols + 4, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[0 + i] ^= symbols[4 + i];
	}
	g_function_freq_in<128>(internal + 112, internal + 96, internal + 104, symbols + 0, 8);
// NODE LEVEL (8)
	f_function_proba_in<128>(internal + 120, internal + 112, internal + 116, 4);
// NODE LEVEL (4)
	f_function_freq_in<128>(internal + 124, internal + 120, internal + 122, 2);
	middle_node_pruned_rep_after_f<128>(internal + 124, decoded + 8, symbols + 8, 2);
	g_function_freq_in<128>(internal + 124, internal + 120, internal + 122, symbols + 8, 2);
	middle_node_pruned_spc_after_g<128>(internal + 124, decoded + 10, symbols + 10, 2);
	for(int i = 0; i < 2; i += 1){
	  symbols[8 + i] ^= symbols[10 + i];
	}
	g_function_proba_in<128>(internal + 120, internal + 112, internal + 116, symbols + 8, 4);
	middle_node_pruned_spc_after_g<128>(internal + 120, decoded + 12, symbols + 12, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[8 + i] ^= symbols[12 + i];
	}
	for(int i = 0; i < 8; i += 1){
	  symbols[0 + i] ^= symbols[8 + i];
	}
	g_function_freq_in<128>(internal + 96, internal + 64, internal + 80, symbols + 0, 16);
// NODE LEVEL (16)
	f_function_proba_in<128>(internal + 112, internal + 96, internal + 104, 8);
// NODE LEVEL (8)
	f_function_freq_in<128>(internal + 120, internal + 112, internal + 116, 4);
// NODE LEVEL (4)
	f_function_freq_in<128>(internal + 124, internal + 120, internal + 122, 2);
	middle_node_pruned_rep_after_f<128>(internal + 124, decoded + 16, symbols + 16, 2);
	g_function_freq_in<128>(internal + 124, internal + 120, internal + 122, symbols + 16, 2);
	middle_node_pruned_spc_after_g<128>(internal + 124, decoded + 18, symbols + 18, 2);
	for(int i = 0; i < 2; i += 1){
	  symbols[16 + i] ^= symbols[18 + i];
	}
	g_function_freq_in<128>(internal + 120, internal + 112, internal + 116, symbols + 16, 4);
	middle_node_pruned_spc_after_g<128>(internal + 120, decoded + 20, symbols + 20, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[16 + i] ^= symbols[20 + i];
	}
	g_function_proba_in<128>(internal + 112, internal + 96, internal + 104, symbols + 16, 8);
	middle_node_pruned_spc_after_g<128>(internal + 112, decoded + 24, symbols + 24, 8);
	for(int i = 0; i < 8; i += 1){
	  symbols[16 + i] ^= symbols[24 + i];
	}
	for(int i = 0; i < 16; i += 1){
	  symbols[0 + i] ^= symbols[16 + i];
	}
	g_function_freq_in<128>(internal + 64, internal + 0, internal + 32, symbols + 0, 32);
// NODE LEVEL (32)
	f_function_proba_in<128>(internal + 96, internal + 64, internal + 80, 16);
// NODE LEVEL (16)
	f_function_freq_in<128>(internal + 112, internal + 96, internal + 104, 8);
// NODE LEVEL (8)
	f_function_freq_in<128>(internal + 120, internal + 112, internal + 116, 4);
	middle_node_pruned_spc_after_f<128>(internal + 120, decoded + 32, symbols + 32, 4);
	g_function_freq_in<128>(internal + 120, internal + 112, internal + 116, symbols + 32, 4);
	middle_node_pruned_spc_after_g<128>(internal + 120, decoded + 36, symbols + 36, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[32 + i] ^= symbols[36 + i];
	}
	g_function_freq_in<128>(internal + 112, internal + 96, internal + 104, symbols + 32, 8);
	middle_node_pruned_rate_1_after_g<128>(internal + 112, decoded + 40, symbols + 40, 8);
	for(int i = 0; i < 8; i += 1){
	  symbols[32 + i] ^= symbols[40 + i];
	}
	g_function_proba_in<128>(internal + 96, internal + 64, internal + 80, symbols + 32, 16);
	middle_node_pruned_rate_1_after_g<128>(internal + 96, decoded + 48, symbols + 48, 16);
	for(int i = 0; i < 16; i += 1){
	  symbols[32 + i] ^= symbols[48 + i];
	}
	for(int i = 0; i < 32; i += 1){
	  symbols[0 + i] ^= symbols[32 + i];
	}
	g_function_proba_in<128>(internal, channel, channel + 64, symbols, 64);
	middle_node_pruned_spc_after_g<128>(internal + 0, decoded + 64, symbols + 64, 64);
}

template <int gf_size>
const int decoder_dedicated<gf_size>::N_gen = 128;

template <int gf_size>
const int decoder_dedicated<gf_size>::K_gen = 115;

