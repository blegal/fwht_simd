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
// NODE LEVEL (1024)
	f_function_proba_in<64>(internal, channel, channel + 512, 512);
// NODE LEVEL (512)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<64>(decoded + 0, symbols + 0, 256);
	g_function_freq_in_after_rate_0<64>(internal + 512, internal + 0, internal + 256, 256);
// NODE LEVEL (256)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<64>(decoded + 256, symbols + 256, 128);
	g_function_proba_in_after_rate_0<64>(internal + 768, internal + 512, internal + 640, 128);
// NODE LEVEL (128)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<64>(decoded + 384, symbols + 384, 64);
	g_function_proba_in_after_rate_0<64>(internal + 896, internal + 768, internal + 832, 64);
// NODE LEVEL (64)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<64>(decoded + 448, symbols + 448, 32);
	g_function_proba_in_after_rate_0<64>(internal + 960, internal + 896, internal + 928, 32);
// NODE LEVEL (32)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<64>(decoded + 480, symbols + 480, 16);
	g_function_proba_in_after_rate_0<64>(internal + 992, internal + 960, internal + 976, 16);
// NODE LEVEL (16)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<64>(decoded + 496, symbols + 496, 8);
	g_function_proba_in_after_rate_0<64>(internal + 1008, internal + 992, internal + 1000, 8);
// NODE LEVEL (8)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<64>(decoded + 504, symbols + 504, 4);
	g_function_proba_in_after_rate_0<64>(internal + 1016, internal + 1008, internal + 1012, 4);
// NODE LEVEL (4)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<64>(decoded + 508, symbols + 508, 2);
	g_function_proba_in_after_rate_0<64>(internal + 1020, internal + 1016, internal + 1018, 2);
	middle_node_pruned_rate_1_after_g<64>(internal + 1020, decoded + 510, symbols + 510, 2);
	for(int i = 0; i < 2; i += 1){
	  symbols[508 + i] ^= symbols[510 + i];
	}
	for(int i = 0; i < 4; i += 1){
	  symbols[504 + i] ^= symbols[508 + i];
	}
	for(int i = 0; i < 8; i += 1){
	  symbols[496 + i] ^= symbols[504 + i];
	}
	for(int i = 0; i < 16; i += 1){
	  symbols[480 + i] ^= symbols[496 + i];
	}
	for(int i = 0; i < 32; i += 1){
	  symbols[448 + i] ^= symbols[480 + i];
	}
	for(int i = 0; i < 64; i += 1){
	  symbols[384 + i] ^= symbols[448 + i];
	}
	for(int i = 0; i < 128; i += 1){
	  symbols[256 + i] ^= symbols[384 + i];
	}
	for(int i = 0; i < 256; i += 1){
	  symbols[0 + i] ^= symbols[256 + i];
	}
	g_function_proba_in<64>(internal, channel, channel + 512, symbols, 512);
// NODE LEVEL (512)
	f_function_proba_in<64>(internal + 512, internal + 0, internal + 256, 256);
	middle_node_pruned_spc_after_f<64>(internal + 512, decoded + 512, symbols + 512, 256);
	g_function_proba_in<64>(internal + 512, internal + 0, internal + 256, symbols + 512, 256);
// NODE LEVEL (256)
	f_function_proba_in<64>(internal + 768, internal + 512, internal + 640, 128);
// NODE LEVEL (128)
	f_function_freq_in<64>(internal + 896, internal + 768, internal + 832, 64);
	middle_node_pruned_spc_after_f<64>(internal + 896, decoded + 768, symbols + 768, 64);
	g_function_freq_in<64>(internal + 896, internal + 768, internal + 832, symbols + 768, 64);
	middle_node_pruned_spc_after_g<64>(internal + 896, decoded + 832, symbols + 832, 64);
	for(int i = 0; i < 64; i += 1){
	  symbols[768 + i] ^= symbols[832 + i];
	}
	g_function_proba_in<64>(internal + 768, internal + 512, internal + 640, symbols + 768, 128);
	middle_node_pruned_rate_1_after_g<64>(internal + 768, decoded + 896, symbols + 896, 128);
	for(int i = 0; i < 128; i += 1){
	  symbols[768 + i] ^= symbols[896 + i];
	}
	for(int i = 0; i < 256; i += 1){
	  symbols[512 + i] ^= symbols[768 + i];
	}
}

template <int gf_size>
const int decoder_dedicated<gf_size>::N_gen = 1024;

template <int gf_size>
const int decoder_dedicated<gf_size>::K_gen = 511;

