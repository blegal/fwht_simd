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

  // f_function_freq_in
  cnt_c = 128; cnt_a = 0; cnt_b = 64;
  loop_f_2 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 0];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 64];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 128]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 128]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 192; cnt_a = 128; cnt_b = 160;
  loop_f_3 : for (s = 0; s < 32; s += 1) {
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
  loop_f0_4 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_freq_in_after_rate_0
  cnt_c = 224; cnt_a = 192; cnt_b = 208;
  loop_g0_5 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 192];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 224]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 224]*/ = memo_in_a;
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
  cnt_c = 240; cnt_a = 224; cnt_b = 232;
  loop_g0_7 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 224];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 240]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 240]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_8 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_proba_in_after_rate_0
  cnt_c = 248; cnt_a = 240; cnt_b = 244;
  loop_g0_9 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 240];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 248]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 248]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 248;
  loop_spc_10 : for (s = 0; s < 4; s += 1) {
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

  cnt_rw = 24; cnt_rd = 28;
  loop_xor_11 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[24 + i] ^= symbols[28 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 16; cnt_rd = 24;
  loop_xor_12 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[16 + i] ^= symbols[24 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 0; cnt_rd = 16;
  loop_xor_13 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    // symbols[0 + i] ^= symbols[16 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 192; cnt_a = 128; cnt_b = 160; cnt_rd = 0;
  loop_g_14 : for (s = 0; s < 32; s += 1) {
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
  loop_f_15 : for (s = 0; s < 16; s += 1) {
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
  loop_f_16 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 224];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 232];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 240]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 240]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_17 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_freq_in_after_rate_0
  cnt_c = 248; cnt_a = 240; cnt_b = 244;
  loop_g0_18 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 240];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 248]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 248]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 248;
  loop_spc_19 : for (s = 0; s < 4; s += 1) {
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

  cnt_rw = 32; cnt_rd = 36;
  loop_xor_20 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[32 + i] ^= symbols[36 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 240; cnt_a = 224; cnt_b = 232; cnt_rd = 32;
  loop_g_21 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 224*/];
    lwht_in_b   = internal_r[cnt_b /*s + 232*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 240*/] = memo_in_a;
    internal_r[cnt_c /*s + 240*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 240;
  loop_spc_22 : for (s = 0; s < 8; s += 1) {
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

  cnt_rw = 32; cnt_rd = 40;
  loop_xor_23 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[32 + i] ^= symbols[40 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 224; cnt_a = 192; cnt_b = 208; cnt_rd = 32;
	loop_g_24 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 192*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 208*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 224*/] = memo_in_a;
    	internal_r[cnt_c/*s + 224*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 224;
  loop_spc_25 : for (s = 0; s < 16; s += 1) {
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

  cnt_rw = 32; cnt_rd = 48;
  loop_xor_26 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    // symbols[32 + i] ^= symbols[48 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 0; cnt_rd = 32;
  loop_xor_27 : for(i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
    // symbols[0 + i] ^= symbols[32 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 128; cnt_a = 0; cnt_b = 64; cnt_rd = 0;
  loop_g_28 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 0*/];
    lwht_in_b   = internal_r[cnt_b /*s + 64*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 128*/] = memo_in_a;
    internal_r[cnt_c /*s + 128*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 192; cnt_a = 128; cnt_b = 160;
  loop_f_29 : for (s = 0; s < 32; s += 1) {
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
  loop_f_30 : for (s = 0; s < 16; s += 1) {
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
  loop_f_31 : for (s = 0; s < 8; s += 1) {
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
  loop_f_32 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 240];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 248]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 248]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 248, decoded + 64, symbols + 64, 4);
  // middle_node_pruned_rep_after_f
  loop_rep_33 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 248];
    lwht_in_b   = internal_r[s + 250];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[s + 252] = memo_in_a;
    internal_r[s + 252] = memo_in_a;
  }
  lwht_in_a   = internal_l[252];
  lwht_in_b   = internal_r[253];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal_l[254] = memo_in_a;
  internal_r[254] = memo_in_a;
  symbol_v = vec_decision( internal_l[254], false );
  loop_rep_34 : for (s = 0; s < 4; s += 1) {
    symbols[cnt_u] = symbol_v;
    decoded[cnt_u] = symbol_v;
    cnt_u += 1;
  }

  // g_function_freq_in
  cnt_c = 248; cnt_a = 240; cnt_b = 244; cnt_rd = 64;
  loop_g_35 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 240*/];
    lwht_in_b   = internal_r[cnt_b /*s + 244*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 248*/] = memo_in_a;
    internal_r[cnt_c /*s + 248*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 248;
  loop_spc_36 : for (s = 0; s < 4; s += 1) {
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

  cnt_rw = 64; cnt_rd = 68;
  loop_xor_37 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[64 + i] ^= symbols[68 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 240; cnt_a = 224; cnt_b = 232; cnt_rd = 64;
  loop_g_38 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 224*/];
    lwht_in_b   = internal_r[cnt_b /*s + 232*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 240*/] = memo_in_a;
    internal_r[cnt_c /*s + 240*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 240;
  loop_spc_39 : for (s = 0; s < 8; s += 1) {
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

  cnt_rw = 64; cnt_rd = 72;
  loop_xor_40 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[64 + i] ^= symbols[72 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 224; cnt_a = 192; cnt_b = 208; cnt_rd = 64;
  loop_g_41 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 192*/];
    lwht_in_b   = internal_r[cnt_b /*s + 208*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 224*/] = memo_in_a;
    internal_r[cnt_c /*s + 224*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 224;
  loop_spc_42 : for (s = 0; s < 16; s += 1) {
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

  cnt_rw = 64; cnt_rd = 80;
  loop_xor_43 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    // symbols[64 + i] ^= symbols[80 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 192; cnt_a = 128; cnt_b = 160; cnt_rd = 64;
	loop_g_44 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 128*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 160*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 192*/] = memo_in_a;
    	internal_r[cnt_c/*s + 192*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 192;
  loop_spc_45 : for (s = 0; s < 32; s += 1) {
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

  cnt_rw = 64; cnt_rd = 96;
  loop_xor_46 : for(i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
    // symbols[64 + i] ^= symbols[96 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 0; cnt_rd = 64;
  loop_xor_47 : for(i = 0; i < 64; i += 1){
#pragma HLS PIPELINE
    // symbols[0 + i] ^= symbols[64 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  // cnt_s = 0; cnt_a = 0; cnt_b = 128; cnt_rd = 128;
  loop_spc_48 : for (s = 0; s < 128; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = channel[s    ];
    lwht_in_b   = channel[s + 128];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    internal_l[s] = memo_in_a;
    internal_r[s] = memo_in_a;
    // cnt_s += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 128; cnt_a = 0; cnt_b = 64;
  loop_f_49 : for (s = 0; s < 64; s += 1) {
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
  loop_f_50 : for (s = 0; s < 32; s += 1) {
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
  loop_f_51 : for (s = 0; s < 16; s += 1) {
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
  loop_f_52 : for (s = 0; s < 8; s += 1) {
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
  loop_f_53 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 240];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 248]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 248]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 248, decoded + 128, symbols + 128, 4);
  // middle_node_pruned_rep_after_f
  loop_rep_54 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 248];
    lwht_in_b   = internal_r[s + 250];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[s + 252] = memo_in_a;
    internal_r[s + 252] = memo_in_a;
  }
  lwht_in_a   = internal_l[252];
  lwht_in_b   = internal_r[253];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal_l[254] = memo_in_a;
  internal_r[254] = memo_in_a;
  symbol_v = vec_decision( internal_l[254], false );
  loop_rep_55 : for (s = 0; s < 4; s += 1) {
    symbols[cnt_u] = symbol_v;
    decoded[cnt_u] = symbol_v;
    cnt_u += 1;
  }

  // g_function_freq_in
  cnt_c = 248; cnt_a = 240; cnt_b = 244; cnt_rd = 128;
  loop_g_56 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 240*/];
    lwht_in_b   = internal_r[cnt_b /*s + 244*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 248*/] = memo_in_a;
    internal_r[cnt_c /*s + 248*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 248;
  loop_spc_57 : for (s = 0; s < 4; s += 1) {
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

  cnt_rw = 128; cnt_rd = 132;
  loop_xor_58 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    // symbols[128 + i] ^= symbols[132 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 240; cnt_a = 224; cnt_b = 232; cnt_rd = 128;
  loop_g_59 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 224*/];
    lwht_in_b   = internal_r[cnt_b /*s + 232*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 240*/] = memo_in_a;
    internal_r[cnt_c /*s + 240*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 240;
  loop_spc_60 : for (s = 0; s < 8; s += 1) {
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

  cnt_rw = 128; cnt_rd = 136;
  loop_xor_61 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    // symbols[128 + i] ^= symbols[136 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 224; cnt_a = 192; cnt_b = 208; cnt_rd = 128;
  loop_g_62 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 192*/];
    lwht_in_b   = internal_r[cnt_b /*s + 208*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 224*/] = memo_in_a;
    internal_r[cnt_c /*s + 224*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 224;
  loop_spc_63 : for (s = 0; s < 16; s += 1) {
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

  cnt_rw = 128; cnt_rd = 144;
  loop_xor_64 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    // symbols[128 + i] ^= symbols[144 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_freq_in
  cnt_c = 192; cnt_a = 128; cnt_b = 160; cnt_rd = 128;
  loop_g_65 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a /*s + 128*/];
    lwht_in_b   = internal_r[cnt_b /*s + 160*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c /*s + 192*/] = memo_in_a;
    internal_r[cnt_c /*s + 192*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 192;
  loop_spc_66 : for (s = 0; s < 32; s += 1) {
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

  cnt_rw = 128; cnt_rd = 160;
  loop_xor_67 : for(i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
    // symbols[128 + i] ^= symbols[160 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  // g_function_proba_in
  cnt_c = 128; cnt_a = 0; cnt_b = 64; cnt_rd = 128;
	loop_g_68 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 0*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 64*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 128*/] = memo_in_a;
    	internal_r[cnt_c/*s + 128*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 128;
  loop_spc_69 : for (s = 0; s < 64; s += 1) {
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

  cnt_rw = 128; cnt_rd = 192;
  loop_xor_70 : for(i = 0; i < 64; i += 1){
#pragma HLS PIPELINE
    // symbols[128 + i] ^= symbols[192 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

}

