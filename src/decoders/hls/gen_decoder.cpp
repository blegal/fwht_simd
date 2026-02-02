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
			uint8_t  decoded[N])
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

  uint8_t decoded_r[N];

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

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_proba_in [hls_generator:244]
  //
  cnt_c = 0; cnt_a = 0; cnt_b = 128;
  loop_f_1 : for (s = 0; s < 128; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = channel[cnt_a];
    lwht_in_b   = channel[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rate_0 [hls_generator:354]
  //
  //loop_f0_2 : for (s = 0; s < 64; s += 1) {
  //#pragma HLS PIPELINE
    //symbols  [cnt_u] = 0;
    //decoded  [cnt_u] = 0;
    //decoded_r[cnt_u] = 0;
    //cnt_u += 1; 
  //}

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_freq_in_after_rate_0 [hls_generator:645]
  //
  cnt_c = 128; cnt_a = 0; cnt_b = 64;
  loop_g0_3 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
    symbols  [cnt_u] = 0;
    decoded  [cnt_u] = 0;
    decoded_r[cnt_u] = 0;
    cnt_u += 1; 
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rate_0 [hls_generator:354]
  //
  //loop_f0_4 : for (s = 0; s < 32; s += 1) {
  //#pragma HLS PIPELINE
    //symbols  [cnt_u] = 0;
    //decoded  [cnt_u] = 0;
    //decoded_r[cnt_u] = 0;
    //cnt_u += 1; 
  //}

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_proba_in_after_rate_0 [hls_generator:702]
  //
  cnt_c = 192; cnt_a = 128; cnt_b = 160;
  loop_g0_5 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    symbols  [cnt_u] = 0;
    decoded  [cnt_u] = 0;
    decoded_r[cnt_u] = 0;
    cnt_u += 1; 
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rate_0 [hls_generator:354]
  //
  //loop_f0_6 : for (s = 0; s < 16; s += 1) {
  //#pragma HLS PIPELINE
    //symbols  [cnt_u] = 0;
    //decoded  [cnt_u] = 0;
    //decoded_r[cnt_u] = 0;
    //cnt_u += 1; 
  //}

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_proba_in_after_rate_0 [hls_generator:702]
  //
  cnt_c = 224; cnt_a = 192; cnt_b = 208;
  loop_g0_7 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    symbols  [cnt_u] = 0;
    decoded  [cnt_u] = 0;
    decoded_r[cnt_u] = 0;
    cnt_u += 1; 
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rate_0 [hls_generator:354]
  //
  //loop_f0_8 : for (s = 0; s < 8; s += 1) {
  //#pragma HLS PIPELINE
    //symbols  [cnt_u] = 0;
    //decoded  [cnt_u] = 0;
    //decoded_r[cnt_u] = 0;
    //cnt_u += 1; 
  //}

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_proba_in_after_rate_0 [hls_generator:702]
  //
  cnt_c = 240; cnt_a = 224; cnt_b = 232;
  loop_g0_9 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    symbols  [cnt_u] = 0;
    decoded  [cnt_u] = 0;
    decoded_r[cnt_u] = 0;
    cnt_u += 1; 
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rate_1_after_g [hls_generator:869]
  //
  cnt_a = 240;
	loop_g_10 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    	symbol_v = vec_decision( internal_l[cnt_a/*s + 240*/], false );
    	symbols  [cnt_u] = symbol_v;
      decoded  [cnt_u] = symbol_v;
      decoded_r[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  local_remove_xors<8>(decoded, decoded_r, cnt_u - 8);

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 5, pred_is_f =0) [hls_generator:1039]
  //
  cnt_rw = 112; cnt_rd = 120;
  loop_xor_11 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 4, pred_is_f =0) [hls_generator:1039]
  //
  cnt_rw = 96; cnt_rd = 112;
  loop_xor_12 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 3, pred_is_f =0) [hls_generator:1039]
  //
  cnt_rw = 64; cnt_rd = 96;
  loop_xor_13 : for(i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 2, pred_is_f =1) [hls_generator:1039]
  //
  cnt_rw = 0; cnt_rd = 64;
  loop_xor_14 : for(i = 0; i < 64; i += 1){
#pragma HLS PIPELINE
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_proba_in [hls_generator:736]
  //
  cnt_c = 0; cnt_a = 0; cnt_b = 128; cnt_rd = 0;
  loop_g_15 : for (s = 0; s < 128; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = channel[cnt_a];
    lwht_in_b   = channel[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_proba_in [hls_generator:295]
  //
  cnt_c = 128; cnt_a = 0; cnt_b = 64;
  loop_f_16 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_freq_in [hls_generator:270]
  //
  cnt_c = 192; cnt_a = 128; cnt_b = 160;
  loop_f_17 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_freq_in [hls_generator:270]
  //
  cnt_c = 224; cnt_a = 192; cnt_b = 208;
  loop_f_18 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_freq_in [hls_generator:270]
  //
  cnt_c = 240; cnt_a = 224; cnt_b = 232;
  loop_f_19 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_freq_in [hls_generator:270]
  //
  cnt_c = 248; cnt_a = 240; cnt_b = 244;
  loop_f_20 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rep_after_f [hls_generator:489]
  //
  loop_rep_21 : for (s = 0; s < 2; s += 1) {
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
  loop_rep_22 : for (s = 0; s < 4; s += 1) {
    symbols  [cnt_u] = symbol_v;
    decoded  [cnt_u] = (s == 3) ?  symbol_v : (ap_uint<log2_gf_size>)0;
    decoded_r[cnt_u] = (s == 3) ?  symbol_v : (ap_uint<log2_gf_size>)0;
    cnt_u += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_freq_in [hls_generator:766]
  //
  cnt_c = 248; cnt_a = 240; cnt_b = 244; cnt_rd = 128;
  loop_g_23 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_spc_after_g [hls_generator:952]
  //
  cnt_a = 248;
  loop_spc_24 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols  [cnt_u] = symbol_v ;
      decoded  [cnt_u] = symbol_v;
      decoded_r[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!
  local_remove_xors<4>(decoded, decoded_r, cnt_u - 4);

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 6, pred_is_f =1) [hls_generator:1039]
  //
  cnt_rw = 128; cnt_rd = 132;
  loop_xor_25 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_freq_in [hls_generator:766]
  //
  cnt_c = 240; cnt_a = 224; cnt_b = 232; cnt_rd = 128;
  loop_g_26 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_spc_after_g [hls_generator:952]
  //
  cnt_a = 240;
  loop_spc_27 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols  [cnt_u] = symbol_v ;
      decoded  [cnt_u] = symbol_v;
      decoded_r[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!
  local_remove_xors<8>(decoded, decoded_r, cnt_u - 8);

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 5, pred_is_f =1) [hls_generator:1039]
  //
  cnt_rw = 128; cnt_rd = 136;
  loop_xor_28 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_freq_in [hls_generator:766]
  //
  cnt_c = 224; cnt_a = 192; cnt_b = 208; cnt_rd = 128;
  loop_g_29 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_spc_after_g [hls_generator:952]
  //
  cnt_a = 224;
  loop_spc_30 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols  [cnt_u] = symbol_v ;
      decoded  [cnt_u] = symbol_v;
      decoded_r[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!
  local_remove_xors<16>(decoded, decoded_r, cnt_u - 16);

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 4, pred_is_f =1) [hls_generator:1039]
  //
  cnt_rw = 128; cnt_rd = 144;
  loop_xor_31 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_freq_in [hls_generator:766]
  //
  cnt_c = 192; cnt_a = 128; cnt_b = 160; cnt_rd = 128;
  loop_g_32 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_spc_after_g [hls_generator:952]
  //
  cnt_a = 192;
  loop_spc_33 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols  [cnt_u] = symbol_v ;
      decoded  [cnt_u] = symbol_v;
      decoded_r[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!
  local_remove_xors<32>(decoded, decoded_r, cnt_u - 32);

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 3, pred_is_f =1) [hls_generator:1039]
  //
  cnt_rw = 128; cnt_rd = 160;
  loop_xor_34 : for(i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_proba_in [hls_generator:791]
  //
  cnt_c = 128; cnt_a = 0; cnt_b = 64; cnt_rd = 128;
	loop_g_35 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a];
    	lwht_in_b   = internal_r[cnt_b];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c] = memo_in_a;
    	internal_r[cnt_c] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_spc_after_g [hls_generator:952]
  //
  cnt_a = 128;
  loop_spc_36 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols  [cnt_u] = symbol_v ;
      decoded  [cnt_u] = symbol_v;
      decoded_r[cnt_u] = symbol_v;
      cnt_a += 1;
      cnt_u += 1;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!
  local_remove_xors<64>(decoded, decoded_r, cnt_u - 64);

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 2, pred_is_f =0) [hls_generator:1039]
  //
  cnt_rw = 128; cnt_rd = 192;
  loop_xor_37 : for(i = 0; i < 64; i += 1){
#pragma HLS PIPELINE
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

}

