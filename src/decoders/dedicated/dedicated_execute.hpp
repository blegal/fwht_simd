/*
 * Copyright Université Rennes and Université Bretagne Sud
 * contributor(s) : Bertrand Le Gal   (2025-2026),
 *                  Abdallah Abdallah (2025-2026),
 *                  Camille  Monière  (2025-2026)
 *
 * bertrand.le-gal@univ-rennes.fr,
 * abdallah.abdallah@univ-ubs.fr,
 * camille.moniere@univ-ubs.fr
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
#pragma once
//
//
#include "decoder_dedicated.hpp"
#include "impl/f_function.hpp"
#include "impl/g_function.hpp"
#include "impl/node_rate_0.hpp"
#include "impl/node_rate_1.hpp"
#include "impl/node_rep.hpp"
#include "impl/node_spc.hpp"
//
//
template <int gf_size>
void decoder_dedicated<gf_size>::execute(void* s_channel, uint16_t * decoded)
{
        symbols_s<gf_size>* channel = static_cast< symbols_s<gf_size>* >(s_channel);
// NODE LEVEL (1024)
	f_function_proba_in<64>(internal, channel, channel + 512, 512);
// NODE LEVEL (512)
	f_function_freq_in<64>(internal + 512, internal + 0, internal + 256, 256);
// NODE LEVEL (256)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<gf_size>(decoded + 0, symbols + 0, 128);
	g_function_freq_in_after_rate_0<gf_size>(internal + 768, internal + 512, internal + 640, 128);
// NODE LEVEL (128)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<gf_size>(decoded + 128, symbols + 128, 64);
	g_function_proba_in_after_rate_0<gf_size>(internal + 896, internal + 768, internal + 832, 64);
// NODE LEVEL (64)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<gf_size>(decoded + 192, symbols + 192, 32);
	g_function_proba_in_after_rate_0<gf_size>(internal + 960, internal + 896, internal + 928, 32);
// NODE LEVEL (32)
	f_function_proba_in<gf_size>(internal + 992, internal + 960, internal + 976, 16);
// NODE LEVEL (16)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<gf_size>(decoded + 224, symbols + 224, 8);
	g_function_freq_in_after_rate_0<gf_size>(internal + 1008, internal + 992, internal + 1000, 8);
// NODE LEVEL (8)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<gf_size>(decoded + 232, symbols + 232, 4);
	g_function_proba_in_after_rate_0<gf_size>(internal + 1016, internal + 1008, internal + 1012, 4);
	middle_node_pruned_rate_1_after_g<gf_size>(internal + 1016, decoded + 236, symbols + 236, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[232 + i] ^= symbols[236 + i];
	}
	for(int i = 0; i < 8; i += 1){
	  symbols[224 + i] ^= symbols[232 + i];
	}
	g_function_proba_in<gf_size>(internal + 992, internal + 960, internal + 976, symbols + 224, 16);
	middle_node_pruned_rate_1_after_g<gf_size>(internal + 992, decoded + 240, symbols + 240, 16);
	for(int i = 0; i < 16; i += 1){
	  symbols[224 + i] ^= symbols[240 + i];
	}
	for(int i = 0; i < 32; i += 1){
	  symbols[192 + i] ^= symbols[224 + i];
	}
	for(int i = 0; i < 64; i += 1){
	  symbols[128 + i] ^= symbols[192 + i];
	}
	for(int i = 0; i < 128; i += 1){
	  symbols[0 + i] ^= symbols[128 + i];
	}
	g_function_freq_in<gf_size>(internal + 512, internal + 0, internal + 256, symbols + 0, 256);
// NODE LEVEL (256)
	f_function_proba_in<gf_size>(internal + 768, internal + 512, internal + 640, 128);
// NODE LEVEL (128)
	f_function_freq_in<64>(internal + 896, internal + 768, internal + 832, 64);
// NODE LEVEL (64)
	f_function_freq_in<64>(internal + 960, internal + 896, internal + 928, 32);
// NODE LEVEL (32)
	f_function_freq_in<64>(internal + 992, internal + 960, internal + 976, 16);
// NODE LEVEL (16)
	f_function_freq_in<64>(internal + 1008, internal + 992, internal + 1000, 8);
// NODE LEVEL (8)
	// f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
	middle_node_pruned_rate_0<gf_size>(decoded + 256, symbols + 256, 4);
	g_function_freq_in_after_rate_0<gf_size>(internal + 1016, internal + 1008, internal + 1012, 4);
	middle_node_pruned_spc_after_g<gf_size>(internal + 1016, decoded + 260, symbols + 260, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[256 + i] ^= symbols[260 + i];
	}
	g_function_freq_in<gf_size>(internal + 1008, internal + 992, internal + 1000, symbols + 256, 8);
	middle_node_pruned_spc_after_g<gf_size>(internal + 1008, decoded + 264, symbols + 264, 8);
	for(int i = 0; i < 8; i += 1){
	  symbols[256 + i] ^= symbols[264 + i];
	}
	g_function_freq_in<gf_size>(internal + 992, internal + 960, internal + 976, symbols + 256, 16);
	middle_node_pruned_spc_after_g<gf_size>(internal + 992, decoded + 272, symbols + 272, 16);
	for(int i = 0; i < 16; i += 1){
	  symbols[256 + i] ^= symbols[272 + i];
	}
	g_function_freq_in<gf_size>(internal + 960, internal + 896, internal + 928, symbols + 256, 32);
	middle_node_pruned_spc_after_g<gf_size>(internal + 960, decoded + 288, symbols + 288, 32);
	for(int i = 0; i < 32; i += 1){
	  symbols[256 + i] ^= symbols[288 + i];
	}
	g_function_freq_in<gf_size>(internal + 896, internal + 768, internal + 832, symbols + 256, 64);
	middle_node_pruned_spc_after_g<gf_size>(internal + 896, decoded + 320, symbols + 320, 64);
	for(int i = 0; i < 64; i += 1){
	  symbols[256 + i] ^= symbols[320 + i];
	}
	g_function_proba_in<gf_size>(internal + 768, internal + 512, internal + 640, symbols + 256, 128);
	middle_node_pruned_spc_after_g<gf_size>(internal + 768, decoded + 384, symbols + 384, 128);
	for(int i = 0; i < 128; i += 1){
	  symbols[256 + i] ^= symbols[384 + i];
	}
	for(int i = 0; i < 256; i += 1){
	  symbols[0 + i] ^= symbols[256 + i];
	}
	g_function_proba_in<gf_size>(internal, channel, channel + 512, symbols, 512);
// NODE LEVEL (512)
	f_function_proba_in<gf_size>(internal + 512, internal + 0, internal + 256, 256);
// NODE LEVEL (256)
	f_function_freq_in<64>(internal + 768, internal + 512, internal + 640, 128);
// NODE LEVEL (128)
	f_function_freq_in<64>(internal + 896, internal + 768, internal + 832, 64);
// NODE LEVEL (64)
	f_function_freq_in<64>(internal + 960, internal + 896, internal + 928, 32);
// NODE LEVEL (32)
	f_function_freq_in<64>(internal + 992, internal + 960, internal + 976, 16);
// NODE LEVEL (16)
	f_function_freq_in<64>(internal + 1008, internal + 992, internal + 1000, 8);
// NODE LEVEL (8)
	f_function_freq_in<64>(internal + 1016, internal + 1008, internal + 1012, 4);
	middle_node_pruned_rep_after_f<gf_size>(internal + 1016, decoded + 512, symbols + 512, 4);
	g_function_freq_in<gf_size>(internal + 1016, internal + 1008, internal + 1012, symbols + 512, 4);
	middle_node_pruned_spc_after_g<gf_size>(internal + 1016, decoded + 516, symbols + 516, 4);
	for(int i = 0; i < 4; i += 1){
	  symbols[512 + i] ^= symbols[516 + i];
	}
	g_function_freq_in<gf_size>(internal + 1008, internal + 992, internal + 1000, symbols + 512, 8);
	middle_node_pruned_spc_after_g<gf_size>(internal + 1008, decoded + 520, symbols + 520, 8);
	for(int i = 0; i < 8; i += 1){
	  symbols[512 + i] ^= symbols[520 + i];
	}
	g_function_freq_in<gf_size>(internal + 992, internal + 960, internal + 976, symbols + 512, 16);
	middle_node_pruned_spc_after_g<gf_size>(internal + 992, decoded + 528, symbols + 528, 16);
	for(int i = 0; i < 16; i += 1){
	  symbols[512 + i] ^= symbols[528 + i];
	}
	g_function_freq_in<gf_size>(internal + 960, internal + 896, internal + 928, symbols + 512, 32);
	middle_node_pruned_spc_after_g<gf_size>(internal + 960, decoded + 544, symbols + 544, 32);
	for(int i = 0; i < 32; i += 1){
	  symbols[512 + i] ^= symbols[544 + i];
	}
	g_function_freq_in<gf_size>(internal + 896, internal + 768, internal + 832, symbols + 512, 64);
	middle_node_pruned_spc_after_g<gf_size>(internal + 896, decoded + 576, symbols + 576, 64);
	for(int i = 0; i < 64; i += 1){
	  symbols[512 + i] ^= symbols[576 + i];
	}
	g_function_freq_in<gf_size>(internal + 768, internal + 512, internal + 640, symbols + 512, 128);
	middle_node_pruned_spc_after_g<gf_size>(internal + 768, decoded + 640, symbols + 640, 128);
	for(int i = 0; i < 128; i += 1){
	  symbols[512 + i] ^= symbols[640 + i];
	}
	g_function_proba_in<gf_size>(internal + 512, internal + 0, internal + 256, symbols + 512, 256);
	middle_node_pruned_spc_after_g<gf_size>(internal + 512, decoded + 768, symbols + 768, 256);
	for(int i = 0; i < 256; i += 1){
	  symbols[512 + i] ^= symbols[768 + i];
	}
}

template <int gf_size>
const int decoder_dedicated<gf_size>::N_gen = 1024;

template <int gf_size>
const int decoder_dedicated<gf_size>::K_gen = 768;

