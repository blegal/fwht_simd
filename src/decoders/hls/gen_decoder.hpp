#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstdint>

#include "include/ap_fixed.hpp"

#define N            64

#define gf_size      64
#define log2_gf_size  6
#define data_width   18


#define i_lwht_width   data_width
#define o_lwht_width   (i_lwht_width+log2_gf_size)
#define i_mult_width   (o_lwht_width)
#define o_mult_width   (i_lwht_width*i_lwht_width)
#define i_norm_width   (o_mult_width)
#define o_norm_width   (i_lwht_width)

typedef struct { ap_int<data_width  > value[gf_size]; } t_memory;
typedef struct { ap_int<i_lwht_width> value[gf_size]; } t_i_lwht;
typedef struct { ap_int<o_lwht_width> value[gf_size]; } t_o_lwht;
typedef struct { ap_int<i_mult_width> value[gf_size]; } t_i_mult;
typedef struct { ap_int<o_mult_width> value[gf_size]; } t_o_mult;
typedef struct { ap_int<i_norm_width> value[gf_size]; } t_i_norm;
typedef struct { ap_int<o_norm_width> value[gf_size]; } t_o_norm;

template <int gf_size>
void decoder_dedicated<gf_size>::execute(
			t_memory channel[N],
			uint16_t decoded[N])
{
  // f_function_proba_in
  loop_1 : for (int s = 0; s < 128; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = channel[s    ];
    lwht_in_b   = channel[s + 128];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal[s] = memo_in_a;
  }

  // f_function_freq_in
  loop_2 : for (int s = 0; s < 64; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 0];
    lwht_in_b   = internal[s + 64];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal[s + 128] = memo_in_a;
  }

  // f_function_freq_in
  loop_3 : for (int s = 0; s < 32; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 128];
    lwht_in_b   = internal[s + 160];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal[s + 192] = memo_in_a;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_4 : for (int s = 0; s < 16; s++) {
#pragma HLS PIPELINE
    symbols[s + 0] = 0;
    decoded[s + 0] = 0;
  }

  // g_function_freq_in_after_rate_0
  loop_5 : for (int s = 0; s < 16; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 192];
    lwht_in_b   = internal[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal[s + 224] = memo_in_a;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_6 : for (int s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    symbols[s + 16] = 0;
    decoded[s + 16] = 0;
  }

  // g_function_proba_in_after_rate_0
  loop_7 : for (int s = 0; s < 8; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 224];
    lwht_in_b   = internal[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal[s + 240] = memo_in_a;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_8 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    symbols[s + 24] = 0;
    decoded[s + 24] = 0;
  }

  // g_function_proba_in_after_rate_0
  loop_9 : for (int s = 0; s < 4; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 240];
    lwht_in_b   = internal[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal[s + 248] = memo_in_a;
  }

  middle_node_pruned_spc_after_g<gf_size>(internal + 248, decoded + 28, symbols + 28, 4);

  for(int i = 0; i < 4; i += 1){
    symbols[24 + i] ^= symbols[28 + i];
  }

  for(int i = 0; i < 8; i += 1){
    symbols[16 + i] ^= symbols[24 + i];
  }

  for(int i = 0; i < 16; i += 1){
    symbols[0 + i] ^= symbols[16 + i];
  }

  // g_function_freq_in
  loop_10 : for (int s = 0; s < 32; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 128];
    	lwht_in_b   = internal[s + 160];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal[s + 192] = memo_in_a;
  }

  // f_function_proba_in
  loop_11 : for (int s = 0; s < 16; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 192];
    lwht_in_b   = internal[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal[s + 224] = memo_in_a;
  }

  // f_function_freq_in
  loop_12 : for (int s = 0; s < 8; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 224];
    lwht_in_b   = internal[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal[s + 240] = memo_in_a;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_13 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    symbols[s + 32] = 0;
    decoded[s + 32] = 0;
  }

  // g_function_freq_in_after_rate_0
  loop_14 : for (int s = 0; s < 4; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 240];
    lwht_in_b   = internal[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal[s + 248] = memo_in_a;
  }

  middle_node_pruned_spc_after_g<gf_size>(internal + 248, decoded + 36, symbols + 36, 4);

  for(int i = 0; i < 4; i += 1){
    symbols[32 + i] ^= symbols[36 + i];
  }

  // g_function_freq_in
  loop_15 : for (int s = 0; s < 8; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 224];
    	lwht_in_b   = internal[s + 232];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal[s + 240] = memo_in_a;
  }

  middle_node_pruned_spc_after_g<gf_size>(internal + 240, decoded + 40, symbols + 40, 8);

  for(int i = 0; i < 8; i += 1){
    symbols[32 + i] ^= symbols[40 + i];
  }

  // g_function_proba_in
	loop_16 : for (int s = 0; s < 16; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 192];
    	lwht_in_b   = internal[s + 208];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    	internal[s + 224] = memo_in_a;
  }

  middle_node_pruned_spc_after_g<gf_size>(internal + 224, decoded + 48, symbols + 48, 16);

  for(int i = 0; i < 16; i += 1){
    symbols[32 + i] ^= symbols[48 + i];
  }

  for(int i = 0; i < 32; i += 1){
    symbols[0 + i] ^= symbols[32 + i];
  }

  // g_function_freq_in
  loop_17 : for (int s = 0; s < 64; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 0];
    	lwht_in_b   = internal[s + 64];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal[s + 128] = memo_in_a;
  }

  // f_function_proba_in
  loop_18 : for (int s = 0; s < 32; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 128];
    lwht_in_b   = internal[s + 160];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal[s + 192] = memo_in_a;
  }

  // f_function_freq_in
  loop_19 : for (int s = 0; s < 16; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 192];
    lwht_in_b   = internal[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal[s + 224] = memo_in_a;
  }

  // f_function_freq_in
  loop_20 : for (int s = 0; s < 8; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 224];
    lwht_in_b   = internal[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal[s + 240] = memo_in_a;
  }

  // f_function_freq_in
  loop_21 : for (int s = 0; s < 4; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 240];
    lwht_in_b   = internal[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal[s + 248] = memo_in_a;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 248, decoded + 64, symbols + 64, 4);
  // middle_node_pruned_rep_after_f
  loop_22 : for (int s = 0; s < 2; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 248];
    lwht_in_b   = internal[s + 250];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal[s + 252] = memo_in_a;
  }
  lwht_in_a   = internal[252];
  lwht_in_b   = internal[253];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal[254] = memo_in_a;
  uint8_t e = vec_i_unroll_argmax( internal[254] );
  loop_17 : for (int s = 0; s < 4; s++) {
    symbols[s + 64] = e;
    decoded[s + 64] = e;
  }

  // g_function_freq_in
  loop_23 : for (int s = 0; s < 4; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 240];
    	lwht_in_b   = internal[s + 244];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal[s + 248] = memo_in_a;
  }

  middle_node_pruned_spc_after_g<gf_size>(internal + 248, decoded + 68, symbols + 68, 4);

  for(int i = 0; i < 4; i += 1){
    symbols[64 + i] ^= symbols[68 + i];
  }

  // g_function_freq_in
  loop_24 : for (int s = 0; s < 8; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 224];
    	lwht_in_b   = internal[s + 232];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal[s + 240] = memo_in_a;
  }

  middle_node_pruned_spc_after_g<gf_size>(internal + 240, decoded + 72, symbols + 72, 8);

  for(int i = 0; i < 8; i += 1){
    symbols[64 + i] ^= symbols[72 + i];
  }

  // g_function_freq_in
  loop_25 : for (int s = 0; s < 16; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 192];
    	lwht_in_b   = internal[s + 208];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal[s + 224] = memo_in_a;
  }

  middle_node_pruned_spc_after_g<gf_size>(internal + 224, decoded + 80, symbols + 80, 16);

  for(int i = 0; i < 16; i += 1){
    symbols[64 + i] ^= symbols[80 + i];
  }

  // g_function_proba_in
	loop_26 : for (int s = 0; s < 32; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 128];
    	lwht_in_b   = internal[s + 160];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    	internal[s + 192] = memo_in_a;
  }

  middle_node_pruned_spc_after_g<gf_size>(internal + 192, decoded + 96, symbols + 96, 32);

  for(int i = 0; i < 32; i += 1){
    symbols[64 + i] ^= symbols[96 + i];
  }

  for(int i = 0; i < 64; i += 1){
    symbols[0 + i] ^= symbols[64 + i];
  }

  // g_function_proba_in
  loop_27 : for (int s = 0; s < 128; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = channel[s    ];
    lwht_in_b   = channel[s + 128];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal[s] = memo_in_a;
  }

  // f_function_proba_in
  loop_28 : for (int s = 0; s < 64; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 0];
    lwht_in_b   = internal[s + 64];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal[s + 128] = memo_in_a;
  }

  // f_function_freq_in
  loop_29 : for (int s = 0; s < 32; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 128];
    lwht_in_b   = internal[s + 160];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal[s + 192] = memo_in_a;
  }

  // f_function_freq_in
  loop_30 : for (int s = 0; s < 16; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 192];
    lwht_in_b   = internal[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal[s + 224] = memo_in_a;
  }

  // f_function_freq_in
  loop_31 : for (int s = 0; s < 8; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 224];
    lwht_in_b   = internal[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal[s + 240] = memo_in_a;
  }

  // f_function_freq_in
  loop_32 : for (int s = 0; s < 4; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 240];
    lwht_in_b   = internal[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal[s + 248] = memo_in_a;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 248, decoded + 128, symbols + 128, 4);
  // middle_node_pruned_rep_after_f
  loop_33 : for (int s = 0; s < 2; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    lwht_in_a   = internal[s + 248];
    lwht_in_b   = internal[s + 250];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal[s + 252] = memo_in_a;
  }
  lwht_in_a   = internal[252];
  lwht_in_b   = internal[253];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal[254] = memo_in_a;
  uint8_t e = vec_i_unroll_argmax( internal[254] );
  loop_17 : for (int s = 0; s < 4; s++) {
    symbols[s + 128] = e;
    decoded[s + 128] = e;
  }

  // g_function_freq_in
  loop_34 : for (int s = 0; s < 4; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 240];
    	lwht_in_b   = internal[s + 244];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal[s + 248] = memo_in_a;
  }

  middle_node_pruned_spc_after_g<gf_size>(internal + 248, decoded + 132, symbols + 132, 4);

  for(int i = 0; i < 4; i += 1){
    symbols[128 + i] ^= symbols[132 + i];
  }

  // g_function_freq_in
  loop_35 : for (int s = 0; s < 8; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 224];
    	lwht_in_b   = internal[s + 232];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal[s + 240] = memo_in_a;
  }

  middle_node_pruned_spc_after_g<gf_size>(internal + 240, decoded + 136, symbols + 136, 8);

  for(int i = 0; i < 8; i += 1){
    symbols[128 + i] ^= symbols[136 + i];
  }

  // g_function_freq_in
  loop_36 : for (int s = 0; s < 16; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 192];
    	lwht_in_b   = internal[s + 208];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal[s + 224] = memo_in_a;
  }

  middle_node_pruned_spc_after_g<gf_size>(internal + 224, decoded + 144, symbols + 144, 16);

  for(int i = 0; i < 16; i += 1){
    symbols[128 + i] ^= symbols[144 + i];
  }

  // g_function_freq_in
  loop_37 : for (int s = 0; s < 32; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 128];
    	lwht_in_b   = internal[s + 160];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal[s + 192] = memo_in_a;
  }

  middle_node_pruned_spc_after_g<gf_size>(internal + 192, decoded + 160, symbols + 160, 32);

  for(int i = 0; i < 32; i += 1){
    symbols[128 + i] ^= symbols[160 + i];
  }

  // g_function_proba_in
	loop_38 : for (int s = 0; s < 64; s++) {
#pragma HLS UNROLL off
#pragma HLS PIPELINE
    	lwht_in_a   = internal[s + 0];
    	lwht_in_b   = internal[s + 64];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    	internal[s + 128] = memo_in_a;
  }

  middle_node_pruned_spc_after_g<gf_size>(internal + 128, decoded + 192, symbols + 192, 64);

  for(int i = 0; i < 64; i += 1){
    symbols[128 + i] ^= symbols[192 + i];
  }

}

template <int gf_size>
const int decoder_dedicated<gf_size>::N_gen = 256;

template <int gf_size>
const int decoder_dedicated<gf_size>::K_gen = 205;

