#include "types.hpp"
#include "f_argmax.hpp"
#include "f_extend.hpp"
#include "f_fwht.hpp"
#include "f_mult.hpp"
#include "f_norm.hpp"
#include "f_datapath.hpp"
#include "f_decision.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#define N            256
#define gf_size      64
#define log2_gf_size  6
#define data_width   18
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
void the_decoder_v2(
			t_i_memo channel[N],
			uint16_t decoded[N])
{

  t_i_memo internal_l[N], internal_r[N];
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=internal_l
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=internal_r

  uint8_t  symbols [N];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=symbols

  t_i_memo lwht_in_a, lwht_in_b;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_b.value

  t_i_memo memo_in_a;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=memo_in_a.value

  uint8_t symbol_v;
  uint8_t symbol_x;

  // f_function_proba_in
  // cnt_s = 0; cnt_a = 0; cnt_b = 128;
  loop_f_1 : for (int s = 0; s < 128; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = channel[s    ];
    lwht_in_b   = channel[s + 128];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[s] = memo_in_a;
    internal_r[s] = memo_in_a;
  }

  // f_function_freq_in
  // cnt_s = 128; cnt_a = 0; cnt_b = 64;
  loop_f_2 : for (int s = 0; s < 64; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 0];
    lwht_in_b   = internal_r[s + 64];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal_l[s + 128] = memo_in_a;
    internal_r[s + 128] = memo_in_a;
  }

  // f_function_freq_in
  // cnt_s = 192; cnt_a = 128; cnt_b = 160;
  loop_f_3 : for (int s = 0; s < 32; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 128];
    lwht_in_b   = internal_r[s + 160];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal_l[s + 192] = memo_in_a;
    internal_r[s + 192] = memo_in_a;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_4 : for (int s = 0; s < 16; s++) {
#pragma HLS PIPELINE
    symbols[s + 0] = 0;
    decoded[s + 0] = 0;
  }

  // g_function_freq_in_after_rate_0
  // cnt_s = 224; cnt_a = 192; cnt_b = 208;
  loop_spc_5 : for (int s = 0; s < 16; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 192];
    lwht_in_b   = internal_r[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[s + 224] = memo_in_a;
    internal_r[s + 224] = memo_in_a;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_6 : for (int s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    symbols[s + 16] = 0;
    decoded[s + 16] = 0;
  }

  // g_function_proba_in_after_rate_0
  // cnt_s = 240; cnt_a = 224; cnt_b = 232;
  loop_spc_7 : for (int s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 224];
    lwht_in_b   = internal_r[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal_l[s + 240] = memo_in_a;
    internal_r[s + 240] = memo_in_a;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_8 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    symbols[s + 24] = 0;
    decoded[s + 24] = 0;
  }

  // g_function_proba_in_after_rate_0
  // cnt_s = 248; cnt_a = 240; cnt_b = 244;
  loop_spc_9 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 240];
    lwht_in_b   = internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal_l[s + 248] = memo_in_a;
    internal_r[s + 248] = memo_in_a;
  }

  // middle_node_pruned_spc_after_g
  loop_spc_10 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    	symbol_v  = vec_decision( internal[s + 248], false );
    	symbols[s + 28] = symbol_v ;
    	decoded[s + 28] = symbol_v ;
    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
  }
  // SPC processing !!!

  loop_xor_11 : for(int i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    symbols[24 + i] ^= symbols[28 + i];
  }

  loop_xor_12 : for(int i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    symbols[16 + i] ^= symbols[24 + i];
  }

  loop_xor_13 : for(int i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    symbols[0 + i] ^= symbols[16 + i];
  }

  // g_function_freq_in
  // cnt_s = 192; cnt_a = 128; cnt_b = 160;
  loop_g_14 : for (int s = 0; s < 32; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[s + 128];
    	lwht_in_b   = internal_r[s + 160];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal_l[s + 192] = memo_in_a;
    	internal_r[s + 192] = memo_in_a;
  }

  // f_function_proba_in
  // cnt_s = 224; cnt_a = 192; cnt_b = 208;
  loop_f_15 : for (int s = 0; s < 16; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 192];
    lwht_in_b   = internal_r[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[s + 224] = memo_in_a;
    internal_r[s + 224] = memo_in_a;
  }

  // f_function_freq_in
  // cnt_s = 240; cnt_a = 224; cnt_b = 232;
  loop_f_16 : for (int s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 224];
    lwht_in_b   = internal_r[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal_l[s + 240] = memo_in_a;
    internal_r[s + 240] = memo_in_a;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_17 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    symbols[s + 32] = 0;
    decoded[s + 32] = 0;
  }

  // g_function_freq_in_after_rate_0
  // cnt_s = 248; cnt_a = 240; cnt_b = 244;
  loop_spc_18 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 240];
    lwht_in_b   = internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[s + 248] = memo_in_a;
    internal_r[s + 248] = memo_in_a;
  }

  // middle_node_pruned_spc_after_g
  loop_spc_19 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    	symbol_v  = vec_decision( internal[s + 248], false );
    	symbols[s + 36] = symbol_v ;
    	decoded[s + 36] = symbol_v ;
    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
  }
  // SPC processing !!!

  loop_xor_20 : for(int i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    symbols[32 + i] ^= symbols[36 + i];
  }

  // g_function_freq_in
  // cnt_s = 240; cnt_a = 224; cnt_b = 232;
  loop_g_21 : for (int s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[s + 224];
    	lwht_in_b   = internal_r[s + 232];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal_l[s + 240] = memo_in_a;
    	internal_r[s + 240] = memo_in_a;
  }

  // middle_node_pruned_spc_after_g
  loop_spc_22 : for (int s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    	symbol_v  = vec_decision( internal[s + 240], false );
    	symbols[s + 40] = symbol_v ;
    	decoded[s + 40] = symbol_v ;
    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
  }
  // SPC processing !!!

  loop_xor_23 : for(int i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    symbols[32 + i] ^= symbols[40 + i];
  }

  // g_function_proba_in
  // cnt_s = 224; cnt_a = 192; cnt_b = 208;
	loop_g_24 : for (int s = 0; s < 16; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[s + 192];
    	lwht_in_b   = internal_r[s + 208];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    	internal_l[s + 224] = memo_in_a;
    	internal_r[s + 224] = memo_in_a;
  }

  // middle_node_pruned_spc_after_g
  loop_spc_25 : for (int s = 0; s < 16; s++) {
#pragma HLS PIPELINE
    	symbol_v  = vec_decision( internal[s + 224], false );
    	symbols[s + 48] = symbol_v ;
    	decoded[s + 48] = symbol_v ;
    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
  }
  // SPC processing !!!

  loop_xor_26 : for(int i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    symbols[32 + i] ^= symbols[48 + i];
  }

  loop_xor_27 : for(int i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
    symbols[0 + i] ^= symbols[32 + i];
  }

  // g_function_freq_in
  // cnt_s = 128; cnt_a = 0; cnt_b = 64;
  loop_g_28 : for (int s = 0; s < 64; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[s + 0];
    	lwht_in_b   = internal_r[s + 64];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal_l[s + 128] = memo_in_a;
    	internal_r[s + 128] = memo_in_a;
  }

  // f_function_proba_in
  // cnt_s = 192; cnt_a = 128; cnt_b = 160;
  loop_f_29 : for (int s = 0; s < 32; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 128];
    lwht_in_b   = internal_r[s + 160];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[s + 192] = memo_in_a;
    internal_r[s + 192] = memo_in_a;
  }

  // f_function_freq_in
  // cnt_s = 224; cnt_a = 192; cnt_b = 208;
  loop_f_30 : for (int s = 0; s < 16; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 192];
    lwht_in_b   = internal_r[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal_l[s + 224] = memo_in_a;
    internal_r[s + 224] = memo_in_a;
  }

  // f_function_freq_in
  // cnt_s = 240; cnt_a = 224; cnt_b = 232;
  loop_f_31 : for (int s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 224];
    lwht_in_b   = internal_r[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal_l[s + 240] = memo_in_a;
    internal_r[s + 240] = memo_in_a;
  }

  // f_function_freq_in
  // cnt_s = 248; cnt_a = 240; cnt_b = 244;
  loop_f_32 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 240];
    lwht_in_b   = internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal_l[s + 248] = memo_in_a;
    internal_r[s + 248] = memo_in_a;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 248, decoded + 64, symbols + 64, 4);
  // middle_node_pruned_rep_after_f
  loop_rep_33 : for (int s = 0; s < 2; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 248];
    lwht_in_b   = internal_r[s + 250];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[s + 252] = memo_in_a;
    internal_r[s + 252] = memo_in_a;
  }
  lwht_in_a   = internal_l[252];
  lwht_in_b   = internal_r[253];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal_l[254] = memo_in_a;
  internal_r[254] = memo_in_a;
  symbol_v = vec_decision( internal_l[254], false );
  loop_rep_34 : for (int s = 0; s < 4; s++) {
    symbols[s + 64] = symbol_v;
    decoded[s + 64] = symbol_v;
  }

  // g_function_freq_in
  // cnt_s = 248; cnt_a = 240; cnt_b = 244;
  loop_g_35 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[s + 240];
    	lwht_in_b   = internal_r[s + 244];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal_l[s + 248] = memo_in_a;
    	internal_r[s + 248] = memo_in_a;
  }

  // middle_node_pruned_spc_after_g
  loop_spc_36 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    	symbol_v  = vec_decision( internal[s + 248], false );
    	symbols[s + 68] = symbol_v ;
    	decoded[s + 68] = symbol_v ;
    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
  }
  // SPC processing !!!

  loop_xor_37 : for(int i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    symbols[64 + i] ^= symbols[68 + i];
  }

  // g_function_freq_in
  // cnt_s = 240; cnt_a = 224; cnt_b = 232;
  loop_g_38 : for (int s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[s + 224];
    	lwht_in_b   = internal_r[s + 232];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal_l[s + 240] = memo_in_a;
    	internal_r[s + 240] = memo_in_a;
  }

  // middle_node_pruned_spc_after_g
  loop_spc_39 : for (int s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    	symbol_v  = vec_decision( internal[s + 240], false );
    	symbols[s + 72] = symbol_v ;
    	decoded[s + 72] = symbol_v ;
    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
  }
  // SPC processing !!!

  loop_xor_40 : for(int i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    symbols[64 + i] ^= symbols[72 + i];
  }

  // g_function_freq_in
  // cnt_s = 224; cnt_a = 192; cnt_b = 208;
  loop_g_41 : for (int s = 0; s < 16; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[s + 192];
    	lwht_in_b   = internal_r[s + 208];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal_l[s + 224] = memo_in_a;
    	internal_r[s + 224] = memo_in_a;
  }

  // middle_node_pruned_spc_after_g
  loop_spc_42 : for (int s = 0; s < 16; s++) {
#pragma HLS PIPELINE
    	symbol_v  = vec_decision( internal[s + 224], false );
    	symbols[s + 80] = symbol_v ;
    	decoded[s + 80] = symbol_v ;
    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
  }
  // SPC processing !!!

  loop_xor_43 : for(int i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    symbols[64 + i] ^= symbols[80 + i];
  }

  // g_function_proba_in
  // cnt_s = 192; cnt_a = 128; cnt_b = 160;
	loop_g_44 : for (int s = 0; s < 32; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[s + 128];
    	lwht_in_b   = internal_r[s + 160];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    	internal_l[s + 192] = memo_in_a;
    	internal_r[s + 192] = memo_in_a;
  }

  // middle_node_pruned_spc_after_g
  loop_spc_45 : for (int s = 0; s < 32; s++) {
#pragma HLS PIPELINE
    	symbol_v  = vec_decision( internal[s + 192], false );
    	symbols[s + 96] = symbol_v ;
    	decoded[s + 96] = symbol_v ;
    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
  }
  // SPC processing !!!

  loop_xor_46 : for(int i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
    symbols[64 + i] ^= symbols[96 + i];
  }

  loop_xor_47 : for(int i = 0; i < 64; i += 1){
#pragma HLS PIPELINE
    symbols[0 + i] ^= symbols[64 + i];
  }

  // g_function_proba_in
  // cnt_s = 0; cnt_a = 0; cnt_b = 128;
  loop_spc_48 : for (int s = 0; s < 128; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = channel[s    ];
    lwht_in_b   = channel[s + 128];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal_l[s] = memo_in_a;
    internal_r[s] = memo_in_a;
  }

  // f_function_proba_in
  // cnt_s = 128; cnt_a = 0; cnt_b = 64;
  loop_f_49 : for (int s = 0; s < 64; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 0];
    lwht_in_b   = internal_r[s + 64];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[s + 128] = memo_in_a;
    internal_r[s + 128] = memo_in_a;
  }

  // f_function_freq_in
  // cnt_s = 192; cnt_a = 128; cnt_b = 160;
  loop_f_50 : for (int s = 0; s < 32; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 128];
    lwht_in_b   = internal_r[s + 160];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal_l[s + 192] = memo_in_a;
    internal_r[s + 192] = memo_in_a;
  }

  // f_function_freq_in
  // cnt_s = 224; cnt_a = 192; cnt_b = 208;
  loop_f_51 : for (int s = 0; s < 16; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 192];
    lwht_in_b   = internal_r[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal_l[s + 224] = memo_in_a;
    internal_r[s + 224] = memo_in_a;
  }

  // f_function_freq_in
  // cnt_s = 240; cnt_a = 224; cnt_b = 232;
  loop_f_52 : for (int s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 224];
    lwht_in_b   = internal_r[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal_l[s + 240] = memo_in_a;
    internal_r[s + 240] = memo_in_a;
  }

  // f_function_freq_in
  // cnt_s = 248; cnt_a = 240; cnt_b = 244;
  loop_f_53 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 240];
    lwht_in_b   = internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    internal_l[s + 248] = memo_in_a;
    internal_r[s + 248] = memo_in_a;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 248, decoded + 128, symbols + 128, 4);
  // middle_node_pruned_rep_after_f
  loop_rep_54 : for (int s = 0; s < 2; s++) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 248];
    lwht_in_b   = internal_r[s + 250];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[s + 252] = memo_in_a;
    internal_r[s + 252] = memo_in_a;
  }
  lwht_in_a   = internal_l[252];
  lwht_in_b   = internal_r[253];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal_l[254] = memo_in_a;
  internal_r[254] = memo_in_a;
  symbol_v = vec_decision( internal_l[254], false );
  loop_rep_55 : for (int s = 0; s < 4; s++) {
    symbols[s + 128] = symbol_v;
    decoded[s + 128] = symbol_v;
  }

  // g_function_freq_in
  // cnt_s = 248; cnt_a = 240; cnt_b = 244;
  loop_g_56 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[s + 240];
    	lwht_in_b   = internal_r[s + 244];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal_l[s + 248] = memo_in_a;
    	internal_r[s + 248] = memo_in_a;
  }

  // middle_node_pruned_spc_after_g
  loop_spc_57 : for (int s = 0; s < 4; s++) {
#pragma HLS PIPELINE
    	symbol_v  = vec_decision( internal[s + 248], false );
    	symbols[s + 132] = symbol_v ;
    	decoded[s + 132] = symbol_v ;
    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
  }
  // SPC processing !!!

  loop_xor_58 : for(int i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    symbols[128 + i] ^= symbols[132 + i];
  }

  // g_function_freq_in
  // cnt_s = 240; cnt_a = 224; cnt_b = 232;
  loop_g_59 : for (int s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[s + 224];
    	lwht_in_b   = internal_r[s + 232];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal_l[s + 240] = memo_in_a;
    	internal_r[s + 240] = memo_in_a;
  }

  // middle_node_pruned_spc_after_g
  loop_spc_60 : for (int s = 0; s < 8; s++) {
#pragma HLS PIPELINE
    	symbol_v  = vec_decision( internal[s + 240], false );
    	symbols[s + 136] = symbol_v ;
    	decoded[s + 136] = symbol_v ;
    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
  }
  // SPC processing !!!

  loop_xor_61 : for(int i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    symbols[128 + i] ^= symbols[136 + i];
  }

  // g_function_freq_in
  // cnt_s = 224; cnt_a = 192; cnt_b = 208;
  loop_g_62 : for (int s = 0; s < 16; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[s + 192];
    	lwht_in_b   = internal_r[s + 208];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal_l[s + 224] = memo_in_a;
    	internal_r[s + 224] = memo_in_a;
  }

  // middle_node_pruned_spc_after_g
  loop_spc_63 : for (int s = 0; s < 16; s++) {
#pragma HLS PIPELINE
    	symbol_v  = vec_decision( internal[s + 224], false );
    	symbols[s + 144] = symbol_v ;
    	decoded[s + 144] = symbol_v ;
    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
  }
  // SPC processing !!!

  loop_xor_64 : for(int i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    symbols[128 + i] ^= symbols[144 + i];
  }

  // g_function_freq_in
  // cnt_s = 192; cnt_a = 128; cnt_b = 160;
  loop_g_65 : for (int s = 0; s < 32; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[s + 128];
    	lwht_in_b   = internal_r[s + 160];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    	internal_l[s + 192] = memo_in_a;
    	internal_r[s + 192] = memo_in_a;
  }

  // middle_node_pruned_spc_after_g
  loop_spc_66 : for (int s = 0; s < 32; s++) {
#pragma HLS PIPELINE
    	symbol_v  = vec_decision( internal[s + 192], false );
    	symbols[s + 160] = symbol_v ;
    	decoded[s + 160] = symbol_v ;
    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
  }
  // SPC processing !!!

  loop_xor_67 : for(int i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
    symbols[128 + i] ^= symbols[160 + i];
  }

  // g_function_proba_in
  // cnt_s = 128; cnt_a = 0; cnt_b = 64;
	loop_g_68 : for (int s = 0; s < 64; s++) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[s + 0];
    	lwht_in_b   = internal_r[s + 64];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
    	internal_l[s + 128] = memo_in_a;
    	internal_r[s + 128] = memo_in_a;
  }

  // middle_node_pruned_spc_after_g
  loop_spc_69 : for (int s = 0; s < 64; s++) {
#pragma HLS PIPELINE
    	symbol_v  = vec_decision( internal[s + 128], false );
    	symbols[s + 192] = symbol_v ;
    	decoded[s + 192] = symbol_v ;
    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
  }
  // SPC processing !!!

  loop_xor_70 : for(int i = 0; i < 64; i += 1){
#pragma HLS PIPELINE
    symbols[128 + i] ^= symbols[192 + i];
  }

}

