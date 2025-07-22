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
	f_function_proba_in<16>(internal, channel, channel + 128, 128);
	middle_node_pruned_rep_after_f<16>(internal + 0, decoded + 0, symbols + 0, 128);
	g_function_proba_in<16>(internal, channel, channel + 128, symbols, 128);
	middle_node_pruned_spc_after_g<16>(internal + 0, decoded + 128, symbols + 128, 128);
}

template <int gf_size>
const int decoder_dedicated<gf_size>::N_gen = 256;

template <int gf_size>
const int decoder_dedicated<gf_size>::K_gen = 128;

