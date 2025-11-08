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
// NODE LEVEL (256)
	f_function_proba_in<64>(internal, channel, channel + 128, 128);
// NODE LEVEL (128)
	f_function_freq_in<64>(internal + 128, internal + 0, internal + 64, 64);
	middle_node_pruned_rep_after_f<64>(internal + 128, decoded + 0, symbols + 0, 64);
	g_function_freq_in<64>(internal + 128, internal + 0, internal + 64, symbols + 0, 64);
// NODE LEVEL (64)
<<<<<<< HEAD
	f_function_proba_in<64>(internal + 192, internal + 128, internal + 160, 32);
// NODE LEVEL (32)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<64>(decoded + 64, symbols + 64, 16);
	g_function_freq_in_after_rate_0<64>(internal + 224, internal + 192, internal + 208, 16);
// NODE LEVEL (16)
	f_function_proba_in<64>(internal + 240, internal + 224, internal + 232, 8);
	middle_node_pruned_rep_after_f<64>(internal + 240, decoded + 80, symbols + 80, 8);
	g_function_proba_in<64>(internal + 240, internal + 224, internal + 232, symbols + 80, 8);
// NODE LEVEL (8)
	f_function_proba_in<64>(internal + 248, internal + 240, internal + 244, 4);
	middle_node_pruned_rep_after_f<64>(internal + 248, decoded + 88, symbols + 88, 4);
	g_function_proba_in<64>(internal + 248, internal + 240, internal + 244, symbols + 88, 4);
	middle_node_pruned_spc_after_g<64>(internal + 248, decoded + 92, symbols + 92, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[88 + i] ^= symbols[92 + i];
	}
	for(int i = 0; i < 8; i += 1){
	  symbols[80 + i] ^= symbols[88 + i];
	}
	for(int i = 0; i < 16; i += 1){
	  symbols[64 + i] ^= symbols[80 + i];
	}
	g_function_proba_in<64>(internal + 192, internal + 128, internal + 160, symbols + 64, 32);
// NODE LEVEL (32)
	f_function_proba_in<64>(internal + 224, internal + 192, internal + 208, 16);
// NODE LEVEL (16)
	f_function_freq_in<64>(internal + 240, internal + 224, internal + 232, 8);
	middle_node_pruned_rep_after_f<64>(internal + 240, decoded + 96, symbols + 96, 8);
	g_function_freq_in<64>(internal + 240, internal + 224, internal + 232, symbols + 96, 8);
	middle_node_pruned_spc_after_g<64>(internal + 240, decoded + 104, symbols + 104, 8);
	for(int i = 0; i < 8; i += 1){
	  symbols[96 + i] ^= symbols[104 + i];
	}
	g_function_proba_in<64>(internal + 224, internal + 192, internal + 208, symbols + 96, 16);
	middle_node_pruned_spc_after_g<64>(internal + 224, decoded + 112, symbols + 112, 16);
	for(int i = 0; i < 16; i += 1){
	  symbols[96 + i] ^= symbols[112 + i];
	}
	for(int i = 0; i < 32; i += 1){
	  symbols[64 + i] ^= symbols[96 + i];
	}
	for(int i = 0; i < 64; i += 1){
	  symbols[0 + i] ^= symbols[64 + i];
	}
	g_function_proba_in<64>(internal, channel, channel + 128, symbols, 128);
// NODE LEVEL (128)
	f_function_proba_in<64>(internal + 128, internal + 0, internal + 64, 64);
// NODE LEVEL (64)
	f_function_freq_in<64>(internal + 192, internal + 128, internal + 160, 32);
// NODE LEVEL (32)
	f_function_freq_in<64>(internal + 224, internal + 192, internal + 208, 16);
// NODE LEVEL (16)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<64>(decoded + 128, symbols + 128, 8);
	g_function_freq_in_after_rate_0<64>(internal + 240, internal + 224, internal + 232, 8);
// NODE LEVEL (8)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<64>(decoded + 136, symbols + 136, 4);
	g_function_proba_in_after_rate_0<64>(internal + 248, internal + 240, internal + 244, 4);
// NODE LEVEL (4)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<64>(decoded + 140, symbols + 140, 2);
	g_function_proba_in_after_rate_0<64>(internal + 252, internal + 248, internal + 250, 2);
	middle_node_pruned_rate_1_after_g<64>(internal + 252, decoded + 142, symbols + 142, 2);
	for(int i = 0; i < 2; i += 1){
	  symbols[140 + i] ^= symbols[142 + i];
	}
	for(int i = 0; i < 4; i += 1){
	  symbols[136 + i] ^= symbols[140 + i];
	}
	for(int i = 0; i < 8; i += 1){
	  symbols[128 + i] ^= symbols[136 + i];
	}
	g_function_freq_in<64>(internal + 224, internal + 192, internal + 208, symbols + 128, 16);
// NODE LEVEL (16)
	f_function_proba_in<64>(internal + 240, internal + 224, internal + 232, 8);
// NODE LEVEL (8)
	f_function_freq_in<64>(internal + 248, internal + 240, internal + 244, 4);
	middle_node_pruned_rep_after_f<64>(internal + 248, decoded + 144, symbols + 144, 4);
	g_function_freq_in<64>(internal + 248, internal + 240, internal + 244, symbols + 144, 4);
	middle_node_pruned_spc_after_g<64>(internal + 248, decoded + 148, symbols + 148, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[144 + i] ^= symbols[148 + i];
	}
	g_function_proba_in<64>(internal + 240, internal + 224, internal + 232, symbols + 144, 8);
	middle_node_pruned_spc_after_g<64>(internal + 240, decoded + 152, symbols + 152, 8);
	for(int i = 0; i < 8; i += 1){
	  symbols[144 + i] ^= symbols[152 + i];
	}
	for(int i = 0; i < 16; i += 1){
	  symbols[128 + i] ^= symbols[144 + i];
	}
	g_function_freq_in<64>(internal + 192, internal + 128, internal + 160, symbols + 128, 32);
// NODE LEVEL (32)
	f_function_proba_in<64>(internal + 224, internal + 192, internal + 208, 16);
// NODE LEVEL (16)
	f_function_freq_in<64>(internal + 240, internal + 224, internal + 232, 8);
// NODE LEVEL (8)
	f_function_freq_in<64>(internal + 248, internal + 240, internal + 244, 4);
	middle_node_pruned_rep_after_f<64>(internal + 248, decoded + 160, symbols + 160, 4);
	g_function_freq_in<64>(internal + 248, internal + 240, internal + 244, symbols + 160, 4);
	middle_node_pruned_spc_after_g<64>(internal + 248, decoded + 164, symbols + 164, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[160 + i] ^= symbols[164 + i];
	}
	g_function_freq_in<64>(internal + 240, internal + 224, internal + 232, symbols + 160, 8);
	middle_node_pruned_spc_after_g<64>(internal + 240, decoded + 168, symbols + 168, 8);
	for(int i = 0; i < 8; i += 1){
	  symbols[160 + i] ^= symbols[168 + i];
	}
	g_function_proba_in<64>(internal + 224, internal + 192, internal + 208, symbols + 160, 16);
	middle_node_pruned_rate_1_after_g<64>(internal + 224, decoded + 176, symbols + 176, 16);
	for(int i = 0; i < 16; i += 1){
	  symbols[160 + i] ^= symbols[176 + i];
	}
	for(int i = 0; i < 32; i += 1){
	  symbols[128 + i] ^= symbols[160 + i];
	}
	g_function_proba_in<64>(internal + 128, internal + 0, internal + 64, symbols + 128, 64);
// NODE LEVEL (64)
	f_function_proba_in<64>(internal + 192, internal + 128, internal + 160, 32);
// NODE LEVEL (32)
	f_function_freq_in<64>(internal + 224, internal + 192, internal + 208, 16);
// NODE LEVEL (16)
	f_function_freq_in<64>(internal + 240, internal + 224, internal + 232, 8);
// NODE LEVEL (8)
	f_function_freq_in<64>(internal + 248, internal + 240, internal + 244, 4);
	middle_node_pruned_rep_after_f<64>(internal + 248, decoded + 192, symbols + 192, 4);
	g_function_freq_in<64>(internal + 248, internal + 240, internal + 244, symbols + 192, 4);
	middle_node_pruned_rate_1_after_g<64>(internal + 248, decoded + 196, symbols + 196, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[192 + i] ^= symbols[196 + i];
	}
	g_function_freq_in<64>(internal + 240, internal + 224, internal + 232, symbols + 192, 8);
	middle_node_pruned_rate_1_after_g<64>(internal + 240, decoded + 200, symbols + 200, 8);
	for(int i = 0; i < 8; i += 1){
	  symbols[192 + i] ^= symbols[200 + i];
	}
	g_function_freq_in<64>(internal + 224, internal + 192, internal + 208, symbols + 192, 16);
	middle_node_pruned_rate_1_after_g<64>(internal + 224, decoded + 208, symbols + 208, 16);
	for(int i = 0; i < 16; i += 1){
	  symbols[192 + i] ^= symbols[208 + i];
	}
	g_function_proba_in<64>(internal + 192, internal + 128, internal + 160, symbols + 192, 32);
	middle_node_pruned_rate_1_after_g<64>(internal + 192, decoded + 224, symbols + 224, 32);
	for(int i = 0; i < 32; i += 1){
	  symbols[192 + i] ^= symbols[224 + i];
	}
	for(int i = 0; i < 64; i += 1){
	  symbols[128 + i] ^= symbols[192 + i];
=======
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
>>>>>>> upstream/devel-cm
	}
	g_function_proba_in<64>(internal, channel, channel + 32, symbols, 32);
	middle_node_pruned_spc_after_g<64>(internal + 0, decoded + 32, symbols + 32, 32);
}

template <int gf_size>
const int decoder_dedicated<gf_size>::N_gen = 256;

template <int gf_size>
<<<<<<< HEAD
const int decoder_dedicated<gf_size>::K_gen = 130;
=======
const int decoder_dedicated<gf_size>::K_gen = 48;
>>>>>>> upstream/devel-cm

