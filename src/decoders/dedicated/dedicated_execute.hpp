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
	f_function_proba_in<1024>(internal, channel, channel + 32, 32);
	middle_node_pruned_rep_after_f<1024>(internal + 0, decoded + 0, symbols + 0, 32);
	g_function_proba_in<1024>(internal, channel, channel + 32, symbols, 32);
	middle_node_pruned_spc_after_g<1024>(internal + 0, decoded + 32, symbols + 32, 32);
}

template <int gf_size>
const int decoder_dedicated<gf_size>::N_gen = 64;

template <int gf_size>
const int decoder_dedicated<gf_size>::K_gen = 32;

