#include "impl/f_datapath.hpp"
#include "impl/f_decision.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#define N            256
#define log2N        8
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
void local_remove_xors(uint16_t * values, int size) {
  if (size == 1)
    return;
  for (int i = 0; i < size / 2; i += 1)
    values[i] ^= values[i + size / 2];
  local_remove_xors(values, size / 2);
  local_remove_xors(values + size / 2, size / 2);
}
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
//#pragma HLS bind_storage variable=channel type=RAM_2P  impl=BRAM
//#pragma HLS bind_storage variable=decoded type=RAM_S2P impl=BRAM

  t_i_memo internal_l[N], internal_r[N];
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=internal_l
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=internal_r
//#pragma HLS bind_storage variable=internal_l type=RAM_S2P impl=BRAM
//#pragma HLS bind_storage variable=internal_r type=RAM_S2P impl=BRAM



  uint8_t  symbols [N];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=symbols

  t_i_memo lwht_in_a, lwht_in_b;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_a.value
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_b.value

  t_i_memo memo_in_a;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=memo_in_a.value

  ap_uint<log2_gf_size> symbol_v;
  ap_uint<log2_gf_size> symbol_x;

  ap_uint<log2N+1> cnt_a; // LLRs from left  memory
  ap_uint<log2N+1> cnt_b; // LLRs from right memory
  ap_uint<log2N+1> cnt_c; // LLRs to left/right memories

  ap_uint<log2N+1> cnt_u  = 0; // 
  ap_uint<log2N+1> cnt_rw = 0; // right and first left
  ap_uint<log2N+1> cnt_rd = 0; // 2nd right (read only)

  ap_uint<log2N+1> s; // optimize this with <log2_n + 1>
  ap_uint<log2N+1> i; // optimize this with <log2_n + 1>

  // f_function_proba_in
  cnt_c = 0; cnt_a = 0; cnt_b = 128;
  loop_f_1 : for (s = 0; s < 128; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = channel[cnt_a]; // channel[s    ];
    lwht_in_b   = channel[cnt_b]; // channel[s + 128];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_2 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_freq_in_after_rate_0
  cnt_c = 128; cnt_a = 0; cnt_b = 64;
  loop_g0_3 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 0];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 64];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 128]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 128]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_4 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_proba_in_after_rate_0
  cnt_c = 192; cnt_a = 128; cnt_b = 160;
  loop_g0_5 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 128];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 160];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 192]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 192]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_6 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_proba_in_after_rate_0
  cnt_c = 224; cnt_a = 192; cnt_b = 208;
  loop_g0_7 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 192];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 224]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 224]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_8 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_proba_in_after_rate_0
  cnt_c = 240; cnt_a = 224; cnt_b = 232;
  loop_g0_9 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 224];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 240]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 240]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 240;
	loop_g_10 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 240*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 8, 8);

  cnt_rw = 112; cnt_rd = 120;
  loop_xor_11 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[112 + i] ^= symbols[120 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 96; cnt_rd = 112;
  loop_xor_12 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    // symbols[96 + i] ^= symbols[112 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 64; cnt_rd = 96;
  loop_xor_13 : for(i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
    // symbols[64 + i] ^= symbols[96 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 0; cnt_rd = 64;
  loop_xor_14 : for(i = 0; i < 64; i += 1){
#pragma HLS PIPELINE
    // symbols[0 + i] ^= symbols[64 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 0; cnt_a = 0; cnt_b = 128; cnt_rd = 128;
  loop_g_15 : for (s = 0; s < 128; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = channel[cnt_a /*s    */];
    lwht_in_b   = channel[cnt_b /*s + 128*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    internal_l[cnt_c/*s*/] = memo_in_a;
    internal_r[cnt_c/*s*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 128; cnt_a = 0; cnt_b = 64;
  loop_f_16 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 0];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 64];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[cnt_c] /*internal_l[s + 128]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 128]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 192; cnt_a = 128; cnt_b = 160;
  loop_f_17 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 128];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 160];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 192]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 192]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 224; cnt_a = 192; cnt_b = 208;
  loop_f_18 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 192];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 224]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 224]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 240; cnt_a = 224; cnt_b = 232;
  loop_f_19 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 224];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 240]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 240]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 248; cnt_a = 240; cnt_b = 244;
  loop_f_20 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 240];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 248]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 248]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_21 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_freq_in_after_rate_0
  cnt_c = 252; cnt_a = 248; cnt_b = 250;
  loop_g0_22 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 248];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 250];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 252]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 252]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // leaf_node_rate_0
  symbols[cnt_u] = 0;
  decoded[cnt_u] = 0;
  cnt_u += 1;

  // g_function_proba_in_after_rate_0
  cnt_c = 254; cnt_a = 252; cnt_b = 253;
  loop_g0_23 : for (s = 0; s < 1; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 252];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 253];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 254]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 254]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // leaf_node_after_g
  cnt_a = 254;
  symbol_v = vec_decision( internal_l[cnt_a], false );
  symbols[cnt_u] = symbol_v;
  decoded[cnt_u] = symbol_v;
  cnt_a += 1;
  cnt_u += 1;

  cnt_rw = 130; cnt_rd = 131;
  loop_xor_24 : for(i = 0; i < 1; i += 1){
#pragma HLS PIPELINE
    // symbols[130 + i] ^= symbols[131 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 128; cnt_rd = 130;
  loop_xor_25 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
    // symbols[128 + i] ^= symbols[130 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 248; cnt_a = 240; cnt_b = 244; cnt_rd = 128;
  loop_g_26 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 240*/];
    lwht_in_b   = internal_r[cnt_b /*s + 244*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 248*/] = memo_in_a;
    internal_r[cnt_c /*s + 248*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 252; cnt_a = 248; cnt_b = 250;
  loop_f_27 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 248];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 250];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[cnt_c] /*internal_l[s + 252]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 252]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // leaf_node_rate_0
  symbols[cnt_u] = 0;
  decoded[cnt_u] = 0;
  cnt_u += 1;

  // g_function_freq_in_after_rate_0
  cnt_c = 254; cnt_a = 252; cnt_b = 253;
  loop_g0_28 : for (s = 0; s < 1; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 252];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 253];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 254]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 254]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // leaf_node_after_g
  cnt_a = 254;
  symbol_v = vec_decision( internal_l[cnt_a], false );
  symbols[cnt_u] = symbol_v;
  decoded[cnt_u] = symbol_v;
  cnt_a += 1;
  cnt_u += 1;

  cnt_rw = 132; cnt_rd = 133;
  loop_xor_29 : for(i = 0; i < 1; i += 1){
#pragma HLS PIPELINE
    // symbols[132 + i] ^= symbols[133 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 252; cnt_a = 248; cnt_b = 250; cnt_rd = 132;
	loop_g_30 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 248*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 250*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 252*/] = memo_in_a;
    	internal_r[cnt_c/*s + 252*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 252;
	loop_g_31 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 252*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 2, 2);

  cnt_rw = 132; cnt_rd = 134;
  loop_xor_32 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
    // symbols[132 + i] ^= symbols[134 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 128; cnt_rd = 132;
  loop_xor_33 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[128 + i] ^= symbols[132 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 240; cnt_a = 224; cnt_b = 232; cnt_rd = 128;
  loop_g_34 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 224*/];
    lwht_in_b   = internal_r[cnt_b /*s + 232*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 240*/] = memo_in_a;
    internal_r[cnt_c /*s + 240*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 248; cnt_a = 240; cnt_b = 244;
  loop_f_35 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 240];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[cnt_c] /*internal_l[s + 248]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 248]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 252; cnt_a = 248; cnt_b = 250;
  loop_f_36 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 248];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 250];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 252]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 252]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // leaf_node_rate_0
  symbols[cnt_u] = 0;
  decoded[cnt_u] = 0;
  cnt_u += 1;

  // g_function_freq_in_after_rate_0
  cnt_c = 254; cnt_a = 252; cnt_b = 253;
  loop_g0_37 : for (s = 0; s < 1; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 252];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 253];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 254]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 254]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // leaf_node_after_g
  cnt_a = 254;
  symbol_v = vec_decision( internal_l[cnt_a], false );
  symbols[cnt_u] = symbol_v;
  decoded[cnt_u] = symbol_v;
  cnt_a += 1;
  cnt_u += 1;

  cnt_rw = 136; cnt_rd = 137;
  loop_xor_38 : for(i = 0; i < 1; i += 1){
#pragma HLS PIPELINE
    // symbols[136 + i] ^= symbols[137 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 252; cnt_a = 248; cnt_b = 250; cnt_rd = 136;
  loop_g_39 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 248*/];
    lwht_in_b   = internal_r[cnt_b /*s + 250*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 252*/] = memo_in_a;
    internal_r[cnt_c /*s + 252*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 252;
	loop_g_40 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 252*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 2, 2);

  cnt_rw = 136; cnt_rd = 138;
  loop_xor_41 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
    // symbols[136 + i] ^= symbols[138 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 248; cnt_a = 240; cnt_b = 244; cnt_rd = 136;
	loop_g_42 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 240*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 244*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 248*/] = memo_in_a;
    	internal_r[cnt_c/*s + 248*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 248;
	loop_g_43 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 248*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 4, 4);

  cnt_rw = 136; cnt_rd = 140;
  loop_xor_44 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[136 + i] ^= symbols[140 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 128; cnt_rd = 136;
  loop_xor_45 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[128 + i] ^= symbols[136 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 224; cnt_a = 192; cnt_b = 208; cnt_rd = 128;
  loop_g_46 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 192*/];
    lwht_in_b   = internal_r[cnt_b /*s + 208*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 224*/] = memo_in_a;
    internal_r[cnt_c /*s + 224*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 240; cnt_a = 224; cnt_b = 232;
  loop_f_47 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 224];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[cnt_c] /*internal_l[s + 240]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 240]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 248; cnt_a = 240; cnt_b = 244;
  loop_f_48 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 240];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 248]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 248]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 252; cnt_a = 248; cnt_b = 250;
  loop_f_49 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 248];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 250];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 252]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 252]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // leaf_node_rate_0
  symbols[cnt_u] = 0;
  decoded[cnt_u] = 0;
  cnt_u += 1;

  // g_function_freq_in_after_rate_0
  cnt_c = 254; cnt_a = 252; cnt_b = 253;
  loop_g0_50 : for (s = 0; s < 1; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 252];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 253];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 254]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 254]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // leaf_node_after_g
  cnt_a = 254;
  symbol_v = vec_decision( internal_l[cnt_a], false );
  symbols[cnt_u] = symbol_v;
  decoded[cnt_u] = symbol_v;
  cnt_a += 1;
  cnt_u += 1;

  cnt_rw = 144; cnt_rd = 145;
  loop_xor_51 : for(i = 0; i < 1; i += 1){
#pragma HLS PIPELINE
    // symbols[144 + i] ^= symbols[145 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 252; cnt_a = 248; cnt_b = 250; cnt_rd = 144;
  loop_g_52 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 248*/];
    lwht_in_b   = internal_r[cnt_b /*s + 250*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 252*/] = memo_in_a;
    internal_r[cnt_c /*s + 252*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 252;
	loop_g_53 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 252*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 2, 2);

  cnt_rw = 144; cnt_rd = 146;
  loop_xor_54 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
    // symbols[144 + i] ^= symbols[146 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 248; cnt_a = 240; cnt_b = 244; cnt_rd = 144;
  loop_g_55 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 240*/];
    lwht_in_b   = internal_r[cnt_b /*s + 244*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 248*/] = memo_in_a;
    internal_r[cnt_c /*s + 248*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 248;
	loop_g_56 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 248*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 4, 4);

  cnt_rw = 144; cnt_rd = 148;
  loop_xor_57 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[144 + i] ^= symbols[148 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 240; cnt_a = 224; cnt_b = 232; cnt_rd = 144;
	loop_g_58 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 224*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 232*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 240*/] = memo_in_a;
    	internal_r[cnt_c/*s + 240*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 240;
	loop_g_59 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 240*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 8, 8);

  cnt_rw = 144; cnt_rd = 152;
  loop_xor_60 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[144 + i] ^= symbols[152 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 128; cnt_rd = 144;
  loop_xor_61 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    // symbols[128 + i] ^= symbols[144 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 192; cnt_a = 128; cnt_b = 160; cnt_rd = 128;
  loop_g_62 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 128*/];
    lwht_in_b   = internal_r[cnt_b /*s + 160*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 192*/] = memo_in_a;
    internal_r[cnt_c /*s + 192*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 224; cnt_a = 192; cnt_b = 208;
  loop_f_63 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 192];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[cnt_c] /*internal_l[s + 224]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 224]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 240; cnt_a = 224; cnt_b = 232;
  loop_f_64 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 224];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 240]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 240]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 248; cnt_a = 240; cnt_b = 244;
  loop_f_65 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 240];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 248]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 248]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 252; cnt_a = 248; cnt_b = 250;
  loop_f_66 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 248];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 250];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 252]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 252]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // leaf_node_rate_0
  symbols[cnt_u] = 0;
  decoded[cnt_u] = 0;
  cnt_u += 1;

  // g_function_freq_in_after_rate_0
  cnt_c = 254; cnt_a = 252; cnt_b = 253;
  loop_g0_67 : for (s = 0; s < 1; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 252];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 253];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 254]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 254]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // leaf_node_after_g
  cnt_a = 254;
  symbol_v = vec_decision( internal_l[cnt_a], false );
  symbols[cnt_u] = symbol_v;
  decoded[cnt_u] = symbol_v;
  cnt_a += 1;
  cnt_u += 1;

  cnt_rw = 160; cnt_rd = 161;
  loop_xor_68 : for(i = 0; i < 1; i += 1){
#pragma HLS PIPELINE
    // symbols[160 + i] ^= symbols[161 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 252; cnt_a = 248; cnt_b = 250; cnt_rd = 160;
  loop_g_69 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 248*/];
    lwht_in_b   = internal_r[cnt_b /*s + 250*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 252*/] = memo_in_a;
    internal_r[cnt_c /*s + 252*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 252;
	loop_g_70 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 252*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 2, 2);

  cnt_rw = 160; cnt_rd = 162;
  loop_xor_71 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
    // symbols[160 + i] ^= symbols[162 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 248; cnt_a = 240; cnt_b = 244; cnt_rd = 160;
  loop_g_72 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 240*/];
    lwht_in_b   = internal_r[cnt_b /*s + 244*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 248*/] = memo_in_a;
    internal_r[cnt_c /*s + 248*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 248;
	loop_g_73 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 248*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 4, 4);

  cnt_rw = 160; cnt_rd = 164;
  loop_xor_74 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[160 + i] ^= symbols[164 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 240; cnt_a = 224; cnt_b = 232; cnt_rd = 160;
  loop_g_75 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 224*/];
    lwht_in_b   = internal_r[cnt_b /*s + 232*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 240*/] = memo_in_a;
    internal_r[cnt_c /*s + 240*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 240;
	loop_g_76 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 240*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 8, 8);

  cnt_rw = 160; cnt_rd = 168;
  loop_xor_77 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[160 + i] ^= symbols[168 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 224; cnt_a = 192; cnt_b = 208; cnt_rd = 160;
	loop_g_78 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 192*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 208*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 224*/] = memo_in_a;
    	internal_r[cnt_c/*s + 224*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 224;
	loop_g_79 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 224*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 16, 16);

  cnt_rw = 160; cnt_rd = 176;
  loop_xor_80 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    // symbols[160 + i] ^= symbols[176 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 128; cnt_rd = 160;
  loop_xor_81 : for(i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
    // symbols[128 + i] ^= symbols[160 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 128; cnt_a = 0; cnt_b = 64; cnt_rd = 128;
	loop_g_82 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 0*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 64*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 128*/] = memo_in_a;
    	internal_r[cnt_c/*s + 128*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 192; cnt_a = 128; cnt_b = 160;
  loop_f_83 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 128];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 160];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[cnt_c] /*internal_l[s + 192]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 192]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 224; cnt_a = 192; cnt_b = 208;
  loop_f_84 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 192];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 224]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 224]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 240; cnt_a = 224; cnt_b = 232;
  loop_f_85 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 224];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 240]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 240]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 248; cnt_a = 240; cnt_b = 244;
  loop_f_86 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 240];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 248]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 248]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 252; cnt_a = 248; cnt_b = 250;
  loop_f_87 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 248];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 250];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 252]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 252]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // leaf_node_rate_0
  symbols[cnt_u] = 0;
  decoded[cnt_u] = 0;
  cnt_u += 1;

  // g_function_freq_in_after_rate_0
  cnt_c = 254; cnt_a = 252; cnt_b = 253;
  loop_g0_88 : for (s = 0; s < 1; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 252];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 253];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 254]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 254]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // leaf_node_after_g
  cnt_a = 254;
  symbol_v = vec_decision( internal_l[cnt_a], false );
  symbols[cnt_u] = symbol_v;
  decoded[cnt_u] = symbol_v;
  cnt_a += 1;
  cnt_u += 1;

  cnt_rw = 192; cnt_rd = 193;
  loop_xor_89 : for(i = 0; i < 1; i += 1){
#pragma HLS PIPELINE
    // symbols[192 + i] ^= symbols[193 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 252; cnt_a = 248; cnt_b = 250; cnt_rd = 192;
  loop_g_90 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 248*/];
    lwht_in_b   = internal_r[cnt_b /*s + 250*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 252*/] = memo_in_a;
    internal_r[cnt_c /*s + 252*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 252;
	loop_g_91 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 252*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 2, 2);

  cnt_rw = 192; cnt_rd = 194;
  loop_xor_92 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
    // symbols[192 + i] ^= symbols[194 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 248; cnt_a = 240; cnt_b = 244; cnt_rd = 192;
  loop_g_93 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 240*/];
    lwht_in_b   = internal_r[cnt_b /*s + 244*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 248*/] = memo_in_a;
    internal_r[cnt_c /*s + 248*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 248;
	loop_g_94 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 248*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 4, 4);

  cnt_rw = 192; cnt_rd = 196;
  loop_xor_95 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[192 + i] ^= symbols[196 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 240; cnt_a = 224; cnt_b = 232; cnt_rd = 192;
  loop_g_96 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 224*/];
    lwht_in_b   = internal_r[cnt_b /*s + 232*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 240*/] = memo_in_a;
    internal_r[cnt_c /*s + 240*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 240;
	loop_g_97 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 240*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 8, 8);

  cnt_rw = 192; cnt_rd = 200;
  loop_xor_98 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[192 + i] ^= symbols[200 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 224; cnt_a = 192; cnt_b = 208; cnt_rd = 192;
  loop_g_99 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 192*/];
    lwht_in_b   = internal_r[cnt_b /*s + 208*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 224*/] = memo_in_a;
    internal_r[cnt_c /*s + 224*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 224;
	loop_g_100 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 224*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 16, 16);

  cnt_rw = 192; cnt_rd = 208;
  loop_xor_101 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    // symbols[192 + i] ^= symbols[208 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 192; cnt_a = 128; cnt_b = 160; cnt_rd = 192;
	loop_g_102 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 128*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 160*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 192*/] = memo_in_a;
    	internal_r[cnt_c/*s + 192*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 192;
	loop_g_103 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 192*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 32, 32);

  cnt_rw = 192; cnt_rd = 224;
  loop_xor_104 : for(i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
    // symbols[192 + i] ^= symbols[224 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 128; cnt_rd = 192;
  loop_xor_105 : for(i = 0; i < 64; i += 1){
#pragma HLS PIPELINE
    // symbols[128 + i] ^= symbols[192 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

}

