#include "impl/f_datapath.hpp"
#include "impl/f_decision.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#define N            64
#define log2N        6
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
  cnt_c = 0; cnt_a = 0; cnt_b = 32;
  loop_f_1 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = channel[cnt_a]; // channel[s    ];
    lwht_in_b   = channel[cnt_b]; // channel[s + 32];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_2 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_freq_in_after_rate_0
  cnt_c = 32; cnt_a = 0; cnt_b = 16;
  loop_g0_3 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 0];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 16];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 32]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 32]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_proba_in
  cnt_c = 48; cnt_a = 32; cnt_b = 40;
  loop_f_4 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 32];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 40];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 48]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 48]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 48, decoded + 16, symbols + 16, 8);
  // middle_node_pruned_rep_after_f
  loop_rep_5 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 48];
    lwht_in_b   = internal_r[s + 52];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[s + 56] = memo_in_a;
    internal_r[s + 56] = memo_in_a;
  }
  lwht_in_a   = internal_l[56];
  lwht_in_b   = internal_r[57];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal_l[60] = memo_in_a;
  internal_r[60] = memo_in_a;
  symbol_v = vec_decision( internal_l[60], false );
  loop_rep_6 : for (s = 0; s < 8; s += 1) {
    symbols[cnt_u] = symbol_v;
    decoded[cnt_u] = (s == 7) ?  symbol_v : (ap_uint<log2_gf_size>)0;
    cnt_u += 1;
  }

  // g_function_proba_in
  cnt_c = 48; cnt_a = 32; cnt_b = 40; cnt_rd = 16;
	loop_g_7 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 32*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 40*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 48*/] = memo_in_a;
    	internal_r[cnt_c/*s + 48*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 56; cnt_a = 48; cnt_b = 52;
  loop_f_8 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 48];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 52];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 56]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 56]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_9 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_freq_in_after_rate_0
  cnt_c = 60; cnt_a = 56; cnt_b = 58;
  loop_g0_10 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 56];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 58];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 60]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 60]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 60;
	loop_g_11 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 60*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 2, 2);

  cnt_rw = 24; cnt_rd = 26;
  loop_xor_12 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
    // symbols[24 + i] ^= symbols[26 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 56; cnt_a = 48; cnt_b = 52; cnt_rd = 24;
	loop_g_13 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 48*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 52*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 56*/] = memo_in_a;
    	internal_r[cnt_c/*s + 56*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 56;
	loop_g_14 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 56*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 4, 4);

  cnt_rw = 24; cnt_rd = 28;
  loop_xor_15 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[24 + i] ^= symbols[28 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 16; cnt_rd = 24;
  loop_xor_16 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[16 + i] ^= symbols[24 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 0; cnt_rd = 16;
  loop_xor_17 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    // symbols[0 + i] ^= symbols[16 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 0; cnt_a = 0; cnt_b = 32; cnt_rd = 0;
  loop_g_18 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = channel[cnt_a /*s    */];
    lwht_in_b   = channel[cnt_b /*s + 32*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    internal_l[cnt_c/*s*/] = memo_in_a;
    internal_r[cnt_c/*s*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 32; cnt_a = 0; cnt_b = 16;
  loop_f_19 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 0];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 16];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 32]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 32]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 48; cnt_a = 32; cnt_b = 40;
  loop_f_20 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 32];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 40];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 48]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 48]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_21 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_freq_in_after_rate_0
  cnt_c = 56; cnt_a = 48; cnt_b = 52;
  loop_g0_22 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 48];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 52];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 56]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 56]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_proba_in
  cnt_c = 60; cnt_a = 56; cnt_b = 58;
  loop_f_23 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 56];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 58];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 60]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 60]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 60, decoded + 36, symbols + 36, 2);
  // middle_node_pruned_rep_after_f
  loop_rep_24 : for (s = 0; s < 1; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 60];
    lwht_in_b   = internal_r[s + 61];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[s + 62] = memo_in_a;
    internal_r[s + 62] = memo_in_a;
  }
  symbol_v = vec_decision( internal_l[62], false );
  loop_rep_25 : for (s = 0; s < 2; s += 1) {
    symbols[cnt_u] = symbol_v;
    decoded[cnt_u] = (s == 1) ?  symbol_v : (ap_uint<log2_gf_size>)0;
    cnt_u += 1;
  }

  // g_function_proba_in
  cnt_c = 60; cnt_a = 56; cnt_b = 58; cnt_rd = 36;
	loop_g_26 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 56*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 58*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 60*/] = memo_in_a;
    	internal_r[cnt_c/*s + 60*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 60;
	loop_g_27 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 60*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 2, 2);

  cnt_rw = 36; cnt_rd = 38;
  loop_xor_28 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
    // symbols[36 + i] ^= symbols[38 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 32; cnt_rd = 36;
  loop_xor_29 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[32 + i] ^= symbols[36 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 48; cnt_a = 32; cnt_b = 40; cnt_rd = 32;
  loop_g_30 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 32*/];
    lwht_in_b   = internal_r[cnt_b /*s + 40*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 48*/] = memo_in_a;
    internal_r[cnt_c /*s + 48*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 56; cnt_a = 48; cnt_b = 52;
  loop_f_31 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 48];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 52];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 56]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 56]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 60; cnt_a = 56; cnt_b = 58;
  loop_f_32 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 56];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 58];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 60]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 60]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 60, decoded + 40, symbols + 40, 2);
  // middle_node_pruned_rep_after_f
  loop_rep_33 : for (s = 0; s < 1; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 60];
    lwht_in_b   = internal_r[s + 61];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[s + 62] = memo_in_a;
    internal_r[s + 62] = memo_in_a;
  }
  symbol_v = vec_decision( internal_l[62], false );
  loop_rep_34 : for (s = 0; s < 2; s += 1) {
    symbols[cnt_u] = symbol_v;
    decoded[cnt_u] = (s == 1) ?  symbol_v : (ap_uint<log2_gf_size>)0;
    cnt_u += 1;
  }

  // g_function_freq_in
  cnt_c = 60; cnt_a = 56; cnt_b = 58; cnt_rd = 40;
  loop_g_35 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 56*/];
    lwht_in_b   = internal_r[cnt_b /*s + 58*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 60*/] = memo_in_a;
    internal_r[cnt_c /*s + 60*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 60;
	loop_g_36 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 60*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 2, 2);

  cnt_rw = 40; cnt_rd = 42;
  loop_xor_37 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
    // symbols[40 + i] ^= symbols[42 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 56; cnt_a = 48; cnt_b = 52; cnt_rd = 40;
	loop_g_38 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 48*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 52*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 56*/] = memo_in_a;
    	internal_r[cnt_c/*s + 56*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 56;
	loop_g_39 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 56*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 4, 4);

  cnt_rw = 40; cnt_rd = 44;
  loop_xor_40 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[40 + i] ^= symbols[44 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 32; cnt_rd = 40;
  loop_xor_41 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[32 + i] ^= symbols[40 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 32; cnt_a = 0; cnt_b = 16; cnt_rd = 32;
	loop_g_42 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 0*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 16*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 32*/] = memo_in_a;
    	internal_r[cnt_c/*s + 32*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 48; cnt_a = 32; cnt_b = 40;
  loop_f_43 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 32];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 40];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 48]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 48]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 56; cnt_a = 48; cnt_b = 52;
  loop_f_44 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 48];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 52];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 56]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 56]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 60; cnt_a = 56; cnt_b = 58;
  loop_f_45 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 56];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 58];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 60]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 60]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 60, decoded + 48, symbols + 48, 2);
  // middle_node_pruned_rep_after_f
  loop_rep_46 : for (s = 0; s < 1; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 60];
    lwht_in_b   = internal_r[s + 61];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[s + 62] = memo_in_a;
    internal_r[s + 62] = memo_in_a;
  }
  symbol_v = vec_decision( internal_l[62], false );
  loop_rep_47 : for (s = 0; s < 2; s += 1) {
    symbols[cnt_u] = symbol_v;
    decoded[cnt_u] = (s == 1) ?  symbol_v : (ap_uint<log2_gf_size>)0;
    cnt_u += 1;
  }

  // g_function_freq_in
  cnt_c = 60; cnt_a = 56; cnt_b = 58; cnt_rd = 48;
  loop_g_48 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 56*/];
    lwht_in_b   = internal_r[cnt_b /*s + 58*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 60*/] = memo_in_a;
    internal_r[cnt_c /*s + 60*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 60;
	loop_g_49 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 60*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 2, 2);

  cnt_rw = 48; cnt_rd = 50;
  loop_xor_50 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
    // symbols[48 + i] ^= symbols[50 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 56; cnt_a = 48; cnt_b = 52; cnt_rd = 48;
  loop_g_51 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 48*/];
    lwht_in_b   = internal_r[cnt_b /*s + 52*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 56*/] = memo_in_a;
    internal_r[cnt_c /*s + 56*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 56;
	loop_g_52 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 56*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 4, 4);

  cnt_rw = 48; cnt_rd = 52;
  loop_xor_53 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[48 + i] ^= symbols[52 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 48; cnt_a = 32; cnt_b = 40; cnt_rd = 48;
	loop_g_54 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 32*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 40*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 48*/] = memo_in_a;
    	internal_r[cnt_c/*s + 48*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 48;
	loop_g_55 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 48*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors(decoded + cnt_u - 8, 8);

  cnt_rw = 48; cnt_rd = 56;
  loop_xor_56 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[48 + i] ^= symbols[56 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 32; cnt_rd = 48;
  loop_xor_57 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    // symbols[32 + i] ^= symbols[48 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

}

