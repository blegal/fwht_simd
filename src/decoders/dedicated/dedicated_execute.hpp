#pragma once

#include "decoder_dedicated.hpp"
#include "impl/f_function.hpp"
#include "impl/g_function.hpp"
#include "impl/node_rate_0.hpp"
#include "impl/node_rate_1.hpp"
#include "impl/node_rep.hpp"
#include "impl/node_spc.hpp"

template <int gf_size>
void decoder_dedicated<gf_size>::execute(void * s_channel, uint16_t * decoded) {
    symbols_s<gf_size> * channel = static_cast<symbols_s<gf_size> *>(s_channel);
    // NODE LEVEL (512)
    f_function_proba_in<64>(internal, channel, channel + 256, 256);
    // NODE LEVEL (256)
    // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
    middle_node_pruned_rate_0<64>(decoded + 0, symbols + 0, 128);
    g_function_freq_in_after_rate_0<64>(internal + 256, internal + 0, internal + 128, 128);
    // NODE LEVEL (128)
    f_function_proba_in<64>(internal + 384, internal + 256, internal + 320, 64);
    middle_node_pruned_rep_after_f<64>(internal + 384, decoded + 128, symbols + 128, 64);
    g_function_proba_in<64>(internal + 384, internal + 256, internal + 320, symbols + 128, 64);
    // NODE LEVEL (64)
    f_function_proba_in<64>(internal + 448, internal + 384, internal + 416, 32);
    // NODE LEVEL (32)
    // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
    middle_node_pruned_rate_0<64>(decoded + 192, symbols + 192, 16);
    g_function_freq_in_after_rate_0<64>(internal + 480, internal + 448, internal + 464, 16);
    // NODE LEVEL (16)
    // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
    middle_node_pruned_rate_0<64>(decoded + 208, symbols + 208, 8);
    g_function_proba_in_after_rate_0<64>(internal + 496, internal + 480, internal + 488, 8);
    // NODE LEVEL (8)
    // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
    middle_node_pruned_rate_0<64>(decoded + 216, symbols + 216, 4);
    g_function_proba_in_after_rate_0<64>(internal + 504, internal + 496, internal + 500, 4);
    middle_node_pruned_spc_after_g<64>(internal + 504, decoded + 220, symbols + 220, 4);
    for (int i = 0; i < 4; i += 1) {
        symbols[216 + i] ^= symbols[220 + i];
    }
    for (int i = 0; i < 8; i += 1) {
        symbols[208 + i] ^= symbols[216 + i];
    }
    for (int i = 0; i < 16; i += 1) {
        symbols[192 + i] ^= symbols[208 + i];
    }
    g_function_proba_in<64>(internal + 448, internal + 384, internal + 416, symbols + 192, 32);
    // NODE LEVEL (32)
    f_function_proba_in<64>(internal + 480, internal + 448, internal + 464, 16);
    // NODE LEVEL (16)
    f_function_freq_in<64>(internal + 496, internal + 480, internal + 488, 8);
    middle_node_pruned_rep_after_f<64>(internal + 496, decoded + 224, symbols + 224, 8);
    g_function_freq_in<64>(internal + 496, internal + 480, internal + 488, symbols + 224, 8);
    // NODE LEVEL (8)
    f_function_proba_in<64>(internal + 504, internal + 496, internal + 500, 4);
    middle_node_pruned_rep_after_f<64>(internal + 504, decoded + 232, symbols + 232, 4);
    g_function_proba_in<64>(internal + 504, internal + 496, internal + 500, symbols + 232, 4);
    middle_node_pruned_spc_after_g<64>(internal + 504, decoded + 236, symbols + 236, 4);
    for (int i = 0; i < 4; i += 1) {
        symbols[232 + i] ^= symbols[236 + i];
    }
    for (int i = 0; i < 8; i += 1) {
        symbols[224 + i] ^= symbols[232 + i];
    }
    g_function_proba_in<64>(internal + 480, internal + 448, internal + 464, symbols + 224, 16);
    // NODE LEVEL (16)
    f_function_proba_in<64>(internal + 496, internal + 480, internal + 488, 8);
    // NODE LEVEL (8)
    f_function_freq_in<64>(internal + 504, internal + 496, internal + 500, 4);
    middle_node_pruned_rep_after_f<64>(internal + 504, decoded + 240, symbols + 240, 4);
    g_function_freq_in<64>(internal + 504, internal + 496, internal + 500, symbols + 240, 4);
    middle_node_pruned_rate_1_after_g<64>(internal + 504, decoded + 244, symbols + 244, 4);
    for (int i = 0; i < 4; i += 1) {
        symbols[240 + i] ^= symbols[244 + i];
    }
    g_function_proba_in<64>(internal + 496, internal + 480, internal + 488, symbols + 240, 8);
    middle_node_pruned_rate_1_after_g<64>(internal + 496, decoded + 248, symbols + 248, 8);
    for (int i = 0; i < 8; i += 1) {
        symbols[240 + i] ^= symbols[248 + i];
    }
    for (int i = 0; i < 16; i += 1) {
        symbols[224 + i] ^= symbols[240 + i];
    }
    for (int i = 0; i < 32; i += 1) {
        symbols[192 + i] ^= symbols[224 + i];
    }
    for (int i = 0; i < 64; i += 1) {
        symbols[128 + i] ^= symbols[192 + i];
    }
    for (int i = 0; i < 128; i += 1) {
        symbols[0 + i] ^= symbols[128 + i];
    }
    g_function_proba_in<64>(internal, channel, channel + 256, symbols, 256);
    // NODE LEVEL (256)
    f_function_proba_in<64>(internal + 256, internal + 0, internal + 128, 128);
    // NODE LEVEL (128)
    f_function_freq_in<64>(internal + 384, internal + 256, internal + 320, 64);
    // NODE LEVEL (64)
    // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
    middle_node_pruned_rate_0<64>(decoded + 256, symbols + 256, 32);
    g_function_freq_in_after_rate_0<64>(internal + 448, internal + 384, internal + 416, 32);
    // NODE LEVEL (32)
    f_function_proba_in<64>(internal + 480, internal + 448, internal + 464, 16);
    middle_node_pruned_rep_after_f<64>(internal + 480, decoded + 288, symbols + 288, 16);
    g_function_proba_in<64>(internal + 480, internal + 448, internal + 464, symbols + 288, 16);
    // NODE LEVEL (16)
    f_function_proba_in<64>(internal + 496, internal + 480, internal + 488, 8);
    middle_node_pruned_rep_after_f<64>(internal + 496, decoded + 304, symbols + 304, 8);
    g_function_proba_in<64>(internal + 496, internal + 480, internal + 488, symbols + 304, 8);
    // NODE LEVEL (8)
    f_function_proba_in<64>(internal + 504, internal + 496, internal + 500, 4);
    middle_node_pruned_rep_after_f<64>(internal + 504, decoded + 312, symbols + 312, 4);
    g_function_proba_in<64>(internal + 504, internal + 496, internal + 500, symbols + 312, 4);
    middle_node_pruned_rate_1_after_g<64>(internal + 504, decoded + 316, symbols + 316, 4);
    for (int i = 0; i < 4; i += 1) {
        symbols[312 + i] ^= symbols[316 + i];
    }
    for (int i = 0; i < 8; i += 1) {
        symbols[304 + i] ^= symbols[312 + i];
    }
    for (int i = 0; i < 16; i += 1) {
        symbols[288 + i] ^= symbols[304 + i];
    }
    for (int i = 0; i < 32; i += 1) {
        symbols[256 + i] ^= symbols[288 + i];
    }
    g_function_freq_in<64>(internal + 384, internal + 256, internal + 320, symbols + 256, 64);
    // NODE LEVEL (64)
    f_function_proba_in<64>(internal + 448, internal + 384, internal + 416, 32);
    // NODE LEVEL (32)
    f_function_freq_in<64>(internal + 480, internal + 448, internal + 464, 16);
    // NODE LEVEL (16)
    // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
    middle_node_pruned_rate_0<64>(decoded + 320, symbols + 320, 8);
    g_function_freq_in_after_rate_0<64>(internal + 496, internal + 480, internal + 488, 8);
    // NODE LEVEL (8)
    // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
    middle_node_pruned_rate_0<64>(decoded + 328, symbols + 328, 4);
    g_function_proba_in_after_rate_0<64>(internal + 504, internal + 496, internal + 500, 4);
    // NODE LEVEL (4)
    // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
    middle_node_pruned_rate_0<64>(decoded + 332, symbols + 332, 2);
    g_function_proba_in_after_rate_0<64>(internal + 508, internal + 504, internal + 506, 2);
    middle_node_pruned_rate_1_after_g<64>(internal + 508, decoded + 334, symbols + 334, 2);
    for (int i = 0; i < 2; i += 1) {
        symbols[332 + i] ^= symbols[334 + i];
    }
    for (int i = 0; i < 4; i += 1) {
        symbols[328 + i] ^= symbols[332 + i];
    }
    for (int i = 0; i < 8; i += 1) {
        symbols[320 + i] ^= symbols[328 + i];
    }
    g_function_freq_in<64>(internal + 480, internal + 448, internal + 464, symbols + 320, 16);
    // NODE LEVEL (16)
    f_function_proba_in<64>(internal + 496, internal + 480, internal + 488, 8);
    // NODE LEVEL (8)
    f_function_freq_in<64>(internal + 504, internal + 496, internal + 500, 4);
    middle_node_pruned_rep_after_f<64>(internal + 504, decoded + 336, symbols + 336, 4);
    g_function_freq_in<64>(internal + 504, internal + 496, internal + 500, symbols + 336, 4);
    middle_node_pruned_spc_after_g<64>(internal + 504, decoded + 340, symbols + 340, 4);
    for (int i = 0; i < 4; i += 1) {
        symbols[336 + i] ^= symbols[340 + i];
    }
    g_function_proba_in<64>(internal + 496, internal + 480, internal + 488, symbols + 336, 8);
    middle_node_pruned_spc_after_g<64>(internal + 496, decoded + 344, symbols + 344, 8);
    for (int i = 0; i < 8; i += 1) {
        symbols[336 + i] ^= symbols[344 + i];
    }
    for (int i = 0; i < 16; i += 1) {
        symbols[320 + i] ^= symbols[336 + i];
    }
    g_function_proba_in<64>(internal + 448, internal + 384, internal + 416, symbols + 320, 32);
    // NODE LEVEL (32)
    f_function_proba_in<64>(internal + 480, internal + 448, internal + 464, 16);
    // NODE LEVEL (16)
    f_function_freq_in<64>(internal + 496, internal + 480, internal + 488, 8);
    // NODE LEVEL (8)
    f_function_freq_in<64>(internal + 504, internal + 496, internal + 500, 4);
    middle_node_pruned_rep_after_f<64>(internal + 504, decoded + 352, symbols + 352, 4);
    g_function_freq_in<64>(internal + 504, internal + 496, internal + 500, symbols + 352, 4);
    middle_node_pruned_spc_after_g<64>(internal + 504, decoded + 356, symbols + 356, 4);
    for (int i = 0; i < 4; i += 1) {
        symbols[352 + i] ^= symbols[356 + i];
    }
    g_function_freq_in<64>(internal + 496, internal + 480, internal + 488, symbols + 352, 8);
    middle_node_pruned_spc_after_g<64>(internal + 496, decoded + 360, symbols + 360, 8);
    for (int i = 0; i < 8; i += 1) {
        symbols[352 + i] ^= symbols[360 + i];
    }
    g_function_proba_in<64>(internal + 480, internal + 448, internal + 464, symbols + 352, 16);
    middle_node_pruned_spc_after_g<64>(internal + 480, decoded + 368, symbols + 368, 16);
    for (int i = 0; i < 16; i += 1) {
        symbols[352 + i] ^= symbols[368 + i];
    }
    for (int i = 0; i < 32; i += 1) {
        symbols[320 + i] ^= symbols[352 + i];
    }
    for (int i = 0; i < 64; i += 1) {
        symbols[256 + i] ^= symbols[320 + i];
    }
    g_function_proba_in<64>(internal + 256, internal + 0, internal + 128, symbols + 256, 128);
    // NODE LEVEL (128)
    f_function_proba_in<64>(internal + 384, internal + 256, internal + 320, 64);
    // NODE LEVEL (64)
    f_function_freq_in<64>(internal + 448, internal + 384, internal + 416, 32);
    // NODE LEVEL (32)
    f_function_freq_in<64>(internal + 480, internal + 448, internal + 464, 16);
    // NODE LEVEL (16)
    f_function_freq_in<64>(internal + 496, internal + 480, internal + 488, 8);
    middle_node_pruned_rep_after_f<64>(internal + 496, decoded + 384, symbols + 384, 8);
    g_function_freq_in<64>(internal + 496, internal + 480, internal + 488, symbols + 384, 8);
    // NODE LEVEL (8)
    f_function_proba_in<64>(internal + 504, internal + 496, internal + 500, 4);
    middle_node_pruned_rep_after_f<64>(internal + 504, decoded + 392, symbols + 392, 4);
    g_function_proba_in<64>(internal + 504, internal + 496, internal + 500, symbols + 392, 4);
    middle_node_pruned_spc_after_g<64>(internal + 504, decoded + 396, symbols + 396, 4);
    for (int i = 0; i < 4; i += 1) {
        symbols[392 + i] ^= symbols[396 + i];
    }
    for (int i = 0; i < 8; i += 1) {
        symbols[384 + i] ^= symbols[392 + i];
    }
    g_function_freq_in<64>(internal + 480, internal + 448, internal + 464, symbols + 384, 16);
    // NODE LEVEL (16)
    f_function_proba_in<64>(internal + 496, internal + 480, internal + 488, 8);
    // NODE LEVEL (8)
    f_function_freq_in<64>(internal + 504, internal + 496, internal + 500, 4);
    middle_node_pruned_rep_after_f<64>(internal + 504, decoded + 400, symbols + 400, 4);
    g_function_freq_in<64>(internal + 504, internal + 496, internal + 500, symbols + 400, 4);
    middle_node_pruned_rate_1_after_g<64>(internal + 504, decoded + 404, symbols + 404, 4);
    for (int i = 0; i < 4; i += 1) {
        symbols[400 + i] ^= symbols[404 + i];
    }
    g_function_proba_in<64>(internal + 496, internal + 480, internal + 488, symbols + 400, 8);
    middle_node_pruned_rate_1_after_g<64>(internal + 496, decoded + 408, symbols + 408, 8);
    for (int i = 0; i < 8; i += 1) {
        symbols[400 + i] ^= symbols[408 + i];
    }
    for (int i = 0; i < 16; i += 1) {
        symbols[384 + i] ^= symbols[400 + i];
    }
    g_function_freq_in<64>(internal + 448, internal + 384, internal + 416, symbols + 384, 32);
    middle_node_pruned_spc_after_g<64>(internal + 448, decoded + 416, symbols + 416, 32);
    for (int i = 0; i < 32; i += 1) {
        symbols[384 + i] ^= symbols[416 + i];
    }
    g_function_proba_in<64>(internal + 384, internal + 256, internal + 320, symbols + 384, 64);
    middle_node_pruned_spc_after_g<64>(internal + 384, decoded + 448, symbols + 448, 64);
    for (int i = 0; i < 64; i += 1) {
        symbols[384 + i] ^= symbols[448 + i];
    }
    for (int i = 0; i < 128; i += 1) {
        symbols[256 + i] ^= symbols[384 + i];
    }
}

template <int gf_size>
const int decoder_dedicated<gf_size>::N_gen = 512;

template <int gf_size>
const int decoder_dedicated<gf_size>::K_gen = 180;
