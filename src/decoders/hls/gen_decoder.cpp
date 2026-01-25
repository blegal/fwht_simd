#include "impl/f_datapath.hpp"
#include "impl/f_decision.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#define N            128
#define log2N        7
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
  
#pragma HLS bind_storage variable=channel type=RAM_2P  impl=BRAM
#pragma HLS bind_storage variable=decoded type=RAM_S2P impl=BRAM

  t_i_memo internal_l[N], internal_r[N];
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=internal_l
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=internal_r
#pragma HLS bind_storage variable=internal_l type=RAM_S2P impl=BRAM
#pragma HLS bind_storage variable=internal_r type=RAM_S2P impl=BRAM

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
  cnt_c = 0; cnt_a = 0; cnt_b = 64;
  loop_f_1 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = channel[cnt_a]; // channel[s    ];
    lwht_in_b   = channel[cnt_b]; // channel[s + 64];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_2 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_freq_in_after_rate_0
  cnt_c = 64; cnt_a = 0; cnt_b = 32;
  loop_g0_3 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 0];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 32];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 64]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 64]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_4 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_proba_in_after_rate_0
  cnt_c = 96; cnt_a = 64; cnt_b = 80;
  loop_g0_5 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 64];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 80];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 96]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 96]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_6 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_proba_in_after_rate_0
  cnt_c = 112; cnt_a = 96; cnt_b = 104;
  loop_g0_7 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 96];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 104];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 112]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 112]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_proba_in
  cnt_c = 120; cnt_a = 112; cnt_b = 116;
  loop_f_8 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 112];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 116];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[cnt_c] /*internal_l[s + 120]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 120]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 120, decoded + 56, symbols + 56, 4);
  // middle_node_pruned_rep_after_f
  loop_rep_9 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 120];
    lwht_in_b   = internal_r[s + 122];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[s + 124] = memo_in_a;
    internal_r[s + 124] = memo_in_a;
  }
  lwht_in_a   = internal_l[124];
  lwht_in_b   = internal_r[125];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal_l[126] = memo_in_a;
  internal_r[126] = memo_in_a;
  symbol_v = vec_decision( internal_l[126], false );
  loop_rep_10 : for (s = 0; s < 4; s += 1) {
    symbols[cnt_u] = symbol_v;
    decoded[cnt_u] = symbol_v;
    cnt_u += 1;
  }

  // g_function_proba_in
  cnt_c = 120; cnt_a = 112; cnt_b = 116; cnt_rd = 56;
	loop_g_11 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 112*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 116*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 120*/] = memo_in_a;
    	internal_r[cnt_c/*s + 120*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 120;
  loop_spc_12 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols[cnt_u] = symbol_v ;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!

  cnt_rw = 56; cnt_rd = 60;
  loop_xor_13 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[56 + i] ^= symbols[60 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 48; cnt_rd = 56;
  loop_xor_14 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[48 + i] ^= symbols[56 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 32; cnt_rd = 48;
  loop_xor_15 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    // symbols[32 + i] ^= symbols[48 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 0; cnt_rd = 32;
  loop_xor_16 : for(i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
    // symbols[0 + i] ^= symbols[32 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 0; cnt_a = 0; cnt_b = 64; cnt_rd = 64;
  loop_g_17 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = channel[cnt_a /*s    */];
    lwht_in_b   = channel[cnt_b /*s + 64*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    internal_l[cnt_c/*s*/] = memo_in_a;
    internal_r[cnt_c/*s*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 64; cnt_a = 0; cnt_b = 32;
  loop_f_18 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 0];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 32];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[cnt_c] /*internal_l[s + 64]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 64]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_19 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_freq_in_after_rate_0
  cnt_c = 96; cnt_a = 64; cnt_b = 80;
  loop_g0_20 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 64];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 80];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 96]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 96]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_proba_in
  cnt_c = 112; cnt_a = 96; cnt_b = 104;
  loop_f_21 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 96];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 104];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[cnt_c] /*internal_l[s + 112]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 112]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 112, decoded + 80, symbols + 80, 8);
  // middle_node_pruned_rep_after_f
  loop_rep_22 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 112];
    lwht_in_b   = internal_r[s + 116];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[s + 120] = memo_in_a;
    internal_r[s + 120] = memo_in_a;
  }
  lwht_in_a   = internal_l[120];
  lwht_in_b   = internal_r[121];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal_l[124] = memo_in_a;
  internal_r[124] = memo_in_a;
  symbol_v = vec_decision( internal_l[124], false );
  loop_rep_23 : for (s = 0; s < 8; s += 1) {
    symbols[cnt_u] = symbol_v;
    decoded[cnt_u] = symbol_v;
    cnt_u += 1;
  }

  // g_function_proba_in
  cnt_c = 112; cnt_a = 96; cnt_b = 104; cnt_rd = 80;
	loop_g_24 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 96*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 104*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 112*/] = memo_in_a;
    	internal_r[cnt_c/*s + 112*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 120; cnt_a = 112; cnt_b = 116;
  loop_f_25 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 112];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 116];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[cnt_c] /*internal_l[s + 120]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 120]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 120, decoded + 88, symbols + 88, 4);
  // middle_node_pruned_rep_after_f
  loop_rep_26 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 120];
    lwht_in_b   = internal_r[s + 122];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[s + 124] = memo_in_a;
    internal_r[s + 124] = memo_in_a;
  }
  lwht_in_a   = internal_l[124];
  lwht_in_b   = internal_r[125];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal_l[126] = memo_in_a;
  internal_r[126] = memo_in_a;
  symbol_v = vec_decision( internal_l[126], false );
  loop_rep_27 : for (s = 0; s < 4; s += 1) {
    symbols[cnt_u] = symbol_v;
    decoded[cnt_u] = symbol_v;
    cnt_u += 1;
  }

  // g_function_proba_in
  cnt_c = 120; cnt_a = 112; cnt_b = 116; cnt_rd = 88;
	loop_g_28 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 112*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 116*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 120*/] = memo_in_a;
    	internal_r[cnt_c/*s + 120*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 120;
  loop_spc_29 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols[cnt_u] = symbol_v ;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!

  cnt_rw = 88; cnt_rd = 92;
  loop_xor_30 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[88 + i] ^= symbols[92 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 80; cnt_rd = 88;
  loop_xor_31 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[80 + i] ^= symbols[88 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 64; cnt_rd = 80;
  loop_xor_32 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    // symbols[64 + i] ^= symbols[80 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 64; cnt_a = 0; cnt_b = 32; cnt_rd = 64;
	loop_g_33 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 0*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 32*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 64*/] = memo_in_a;
    	internal_r[cnt_c/*s + 64*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 96; cnt_a = 64; cnt_b = 80;
  loop_f_34 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 64];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 80];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[cnt_c] /*internal_l[s + 96]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 96]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 112; cnt_a = 96; cnt_b = 104;
  loop_f_35 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 96];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 104];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 112]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 112]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_36 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_freq_in_after_rate_0
  cnt_c = 120; cnt_a = 112; cnt_b = 116;
  loop_g0_37 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 112];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 116];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 120]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 120]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_38 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_proba_in_after_rate_0
  cnt_c = 124; cnt_a = 120; cnt_b = 122;
  loop_g0_39 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 120];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 122];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 124]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 124]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 124;
	loop_g_40 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 124*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }

  cnt_rw = 100; cnt_rd = 102;
  loop_xor_41 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
    // symbols[100 + i] ^= symbols[102 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 96; cnt_rd = 100;
  loop_xor_42 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[96 + i] ^= symbols[100 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 112; cnt_a = 96; cnt_b = 104; cnt_rd = 96;
  loop_g_43 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 96*/];
    lwht_in_b   = internal_r[cnt_b /*s + 104*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 112*/] = memo_in_a;
    internal_r[cnt_c /*s + 112*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 120; cnt_a = 112; cnt_b = 116;
  loop_f_44 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 112];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 116];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[cnt_c] /*internal_l[s + 120]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 120]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 124; cnt_a = 120; cnt_b = 122;
  loop_f_45 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 120];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 122];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 124]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 124]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 124, decoded + 104, symbols + 104, 2);
  // middle_node_pruned_rep_after_f
  loop_rep_46 : for (s = 0; s < 1; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 124];
    lwht_in_b   = internal_r[s + 125];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[s + 126] = memo_in_a;
    internal_r[s + 126] = memo_in_a;
  }
  lwht_in_a   = internal_l[126];
  lwht_in_b   = internal_r[127];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal_l[127] = memo_in_a;
  internal_r[127] = memo_in_a;
  symbol_v = vec_decision( internal_l[127], false );
  loop_rep_47 : for (s = 0; s < 2; s += 1) {
    symbols[cnt_u] = symbol_v;
    decoded[cnt_u] = symbol_v;
    cnt_u += 1;
  }

  // g_function_freq_in
  cnt_c = 124; cnt_a = 120; cnt_b = 122; cnt_rd = 104;
  loop_g_48 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 120*/];
    lwht_in_b   = internal_r[cnt_b /*s + 122*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 124*/] = memo_in_a;
    internal_r[cnt_c /*s + 124*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 124;
  loop_spc_49 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols[cnt_u] = symbol_v ;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!

  cnt_rw = 104; cnt_rd = 106;
  loop_xor_50 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
    // symbols[104 + i] ^= symbols[106 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 120; cnt_a = 112; cnt_b = 116; cnt_rd = 104;
	loop_g_51 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 112*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 116*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 120*/] = memo_in_a;
    	internal_r[cnt_c/*s + 120*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_rate_1_after_g
  cnt_a = 120;
	loop_g_52 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 120*/], false );
    	symbols[cnt_u] = symbol_v;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }

  cnt_rw = 104; cnt_rd = 108;
  loop_xor_53 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[104 + i] ^= symbols[108 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 96; cnt_rd = 104;
  loop_xor_54 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[96 + i] ^= symbols[104 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 96; cnt_a = 64; cnt_b = 80; cnt_rd = 96;
	loop_g_55 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 64*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 80*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 96*/] = memo_in_a;
    	internal_r[cnt_c/*s + 96*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 96;
  loop_spc_56 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols[cnt_u] = symbol_v ;
      decoded[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!

  cnt_rw = 96; cnt_rd = 112;
  loop_xor_57 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    // symbols[96 + i] ^= symbols[112 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 64; cnt_rd = 96;
  loop_xor_58 : for(i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
    // symbols[64 + i] ^= symbols[96 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

}

