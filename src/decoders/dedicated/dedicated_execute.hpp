#pragma once

#include "decoder_dedicated.hpp"
#include "f_function_freq_in.hpp"
#include "f_function_proba_in.hpp"
#include "g_function_freq_in.hpp"
#include "g_function_proba_in.hpp"
#include "middle_node_pruned_rate_0.hpp"
#include "middle_node_pruned_rate_1_after_f.hpp"
#include "middle_node_pruned_rate_1_after_g.hpp"
#include "middle_node_pruned_rep_after_f.hpp"
#include "middle_node_pruned_rep_after_g.hpp"
#include "middle_node_pruned_spc_after_f.hpp"
#include "middle_node_pruned_spc_after_g.hpp"

template <int gf_size>
void decoder_dedicated<gf_size>::execute(symbols_t * channel, uint16_t * decoded) {
    // NODE LEVEL (256)
    f_function_proba_in<64>(internal, channel, channel + 128, 128);
    // NODE LEVEL (128)
    f_function_freq_in<64>(internal + 128, internal + 0, internal + 64, 64);
    // NODE LEVEL (64)
    f_function_freq_in<64>(internal + 192, internal + 128, internal + 160, 32);
    middle_node_pruned_rep_after_f<64>(internal + 192, decoded + 0, symbols + 0, 32);
    g_function_freq_in<64>(internal + 192, internal + 128, internal + 160, symbols + 0, 32);
    // NODE LEVEL (32)
    f_function_proba_in<64>(internal + 224, internal + 192, internal + 208, 16);
    // NODE LEVEL (16)
    // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
    middle_node_pruned_rate_0<64>(decoded + 32, symbols + 32, 8);
    g_function_freq_in_after_rate_0<64>(internal + 240, internal + 224, internal + 232, 8);
    // NODE LEVEL (8)
    // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
    middle_node_pruned_rate_0<64>(decoded + 40, symbols + 40, 4);
    g_function_proba_in_after_rate_0<64>(internal + 248, internal + 240, internal + 244, 4);
    middle_node_pruned_spc_after_g<64>(internal + 248, decoded + 44, symbols + 44, 4);
    for (int i = 0; i < 4; i += 1) {
        symbols[40 + i] ^= symbols[44 + i];
    }
    for (int i = 0; i < 8; i += 1) {
        symbols[32 + i] ^= symbols[40 + i];
    }
    g_function_proba_in<64>(internal + 224, internal + 192, internal + 208, symbols + 32, 16);
    // NODE LEVEL (16)
    f_function_proba_in<64>(internal + 240, internal + 224, internal + 232, 8);
    // NODE LEVEL (8)
    f_function_freq_in<64>(internal + 248, internal + 240, internal + 244, 4);
    middle_node_pruned_rep_after_f<64>(internal + 248, decoded + 48, symbols + 48, 4);
    g_function_freq_in<64>(internal + 248, internal + 240, internal + 244, symbols + 48, 4);
    middle_node_pruned_spc_after_g<64>(internal + 248, decoded + 52, symbols + 52, 4);
    for (int i = 0; i < 4; i += 1) {
        symbols[48 + i] ^= symbols[52 + i];
    }
    g_function_proba_in<64>(internal + 240, internal + 224, internal + 232, symbols + 48, 8);
    middle_node_pruned_spc_after_g<64>(internal + 240, decoded + 56, symbols + 56, 8);
    for (int i = 0; i < 8; i += 1) {
        symbols[48 + i] ^= symbols[56 + i];
    }
    for (int i = 0; i < 16; i += 1) {
        symbols[32 + i] ^= symbols[48 + i];
    }
    for (int i = 0; i < 32; i += 1) {
        symbols[0 + i] ^= symbols[32 + i];
    }
    g_function_freq_in<64>(internal + 128, internal + 0, internal + 64, symbols + 0, 64);
    // NODE LEVEL (64)
    f_function_proba_in<64>(internal + 192, internal + 128, internal + 160, 32);
    // NODE LEVEL (32)
    f_function_freq_in<64>(internal + 224, internal + 192, internal + 208, 16);
    // NODE LEVEL (16)
    f_function_freq_in<64>(internal + 240, internal + 224, internal + 232, 8);
    middle_node_pruned_rep_after_f<64>(internal + 240, decoded + 64, symbols + 64, 8);
    g_function_freq_in<64>(internal + 240, internal + 224, internal + 232, symbols + 64, 8);
    // NODE LEVEL (8)
    f_function_proba_in<64>(internal + 248, internal + 240, internal + 244, 4);
    middle_node_pruned_rep_after_f<64>(internal + 248, decoded + 72, symbols + 72, 4);
    g_function_proba_in<64>(internal + 248, internal + 240, internal + 244, symbols + 72, 4);
    middle_node_pruned_spc_after_g<64>(internal + 248, decoded + 76, symbols + 76, 4);
    for (int i = 0; i < 4; i += 1) {
        symbols[72 + i] ^= symbols[76 + i];
    }
    for (int i = 0; i < 8; i += 1) {
        symbols[64 + i] ^= symbols[72 + i];
    }
    g_function_freq_in<64>(internal + 224, internal + 192, internal + 208, symbols + 64, 16);
    // NODE LEVEL (16)
    f_function_proba_in<64>(internal + 240, internal + 224, internal + 232, 8);
    // NODE LEVEL (8)
    f_function_freq_in<64>(internal + 248, internal + 240, internal + 244, 4);
    middle_node_pruned_rep_after_f<64>(internal + 248, decoded + 80, symbols + 80, 4);
    g_function_freq_in<64>(internal + 248, internal + 240, internal + 244, symbols + 80, 4);
    middle_node_pruned_rate_1_after_g<64>(internal + 248, decoded + 84, symbols + 84, 4);
    for (int i = 0; i < 4; i += 1) {
        symbols[80 + i] ^= symbols[84 + i];
    }
    g_function_proba_in<64>(internal + 240, internal + 224, internal + 232, symbols + 80, 8);
    middle_node_pruned_rate_1_after_g<64>(internal + 240, decoded + 88, symbols + 88, 8);
    for (int i = 0; i < 8; i += 1) {
        symbols[80 + i] ^= symbols[88 + i];
    }
    for (int i = 0; i < 16; i += 1) {
        symbols[64 + i] ^= symbols[80 + i];
    }
    g_function_proba_in<64>(internal + 192, internal + 128, internal + 160, symbols + 64, 32);
    // NODE LEVEL (32)
    f_function_proba_in<64>(internal + 224, internal + 192, internal + 208, 16);
    // NODE LEVEL (16)
    f_function_freq_in<64>(internal + 240, internal + 224, internal + 232, 8);
    // NODE LEVEL (8)
    f_function_freq_in<64>(internal + 248, internal + 240, internal + 244, 4);
    // NODE LEVEL (4)
    // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
    middle_node_pruned_rate_0<64>(decoded + 96, symbols + 96, 2);
    g_function_freq_in_after_rate_0<64>(internal + 252, internal + 248, internal + 250, 2);
    middle_node_pruned_rate_1_after_g<64>(internal + 252, decoded + 98, symbols + 98, 2);
    for (int i = 0; i < 2; i += 1) {
        symbols[96 + i] ^= symbols[98 + i];
    }
    g_function_freq_in<64>(internal + 248, internal + 240, internal + 244, symbols + 96, 4);
    middle_node_pruned_rate_1_after_g<64>(internal + 248, decoded + 100, symbols + 100, 4);
    for (int i = 0; i < 4; i += 1) {
        symbols[96 + i] ^= symbols[100 + i];
    }
    g_function_freq_in<64>(internal + 240, internal + 224, internal + 232, symbols + 96, 8);
    middle_node_pruned_rate_1_after_g<64>(internal + 240, decoded + 104, symbols + 104, 8);
    for (int i = 0; i < 8; i += 1) {
        symbols[96 + i] ^= symbols[104 + i];
    }
    g_function_proba_in<64>(internal + 224, internal + 192, internal + 208, symbols + 96, 16);
    middle_node_pruned_rate_1_after_g<64>(internal + 224, decoded + 112, symbols + 112, 16);
    for (int i = 0; i < 16; i += 1) {
        symbols[96 + i] ^= symbols[112 + i];
    }
    for (int i = 0; i < 32; i += 1) {
        symbols[64 + i] ^= symbols[96 + i];
    }
    for (int i = 0; i < 64; i += 1) {
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
    f_function_freq_in<64>(internal + 240, internal + 224, internal + 232, 8);
    middle_node_pruned_rep_after_f<64>(internal + 240, decoded + 128, symbols + 128, 8);
    g_function_freq_in<64>(internal + 240, internal + 224, internal + 232, symbols + 128, 8);
    middle_node_pruned_spc_after_g<64>(internal + 240, decoded + 136, symbols + 136, 8);
    for (int i = 0; i < 8; i += 1) {
        symbols[128 + i] ^= symbols[136 + i];
    }
    g_function_freq_in<64>(internal + 224, internal + 192, internal + 208, symbols + 128, 16);
    middle_node_pruned_spc_after_g<64>(internal + 224, decoded + 144, symbols + 144, 16);
    for (int i = 0; i < 16; i += 1) {
        symbols[128 + i] ^= symbols[144 + i];
    }
    g_function_freq_in<64>(internal + 192, internal + 128, internal + 160, symbols + 128, 32);
    middle_node_pruned_spc_after_g<64>(internal + 192, decoded + 160, symbols + 160, 32);
    for (int i = 0; i < 32; i += 1) {
        symbols[128 + i] ^= symbols[160 + i];
    }
    g_function_proba_in<64>(internal + 128, internal + 0, internal + 64, symbols + 128, 64);
    middle_node_pruned_spc_after_g<64>(internal + 128, decoded + 192, symbols + 192, 64);
    for (int i = 0; i < 64; i += 1) {
        symbols[128 + i] ^= symbols[192 + i];
    }
}

template <int gf_size>
const int decoder_dedicated<gf_size>::N_gen = 256;

template <int gf_size>
const int decoder_dedicated<gf_size>::K_gen = 180;
