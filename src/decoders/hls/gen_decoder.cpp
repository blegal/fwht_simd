#include "impl/f_datapath.hpp"
#include "impl/f_decision.hpp"
#include "impl/f_xor_processor.hpp"
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
			uint8_t  decoded[N])
{
#pragma HLS bind_storage variable=channel type=RAM_2P  impl=BRAM

#pragma HLS ARRAY_PARTITION dim=1 type=cyclic variable=decoded  factor=4
#pragma HLS bind_storage variable=decoded type=RAM_S2P impl=BRAM

  static t_i_memo internal_l[N];
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=internal_l
#pragma HLS bind_storage variable=internal_l type=RAM_S2P impl=BRAM

  static t_i_memo internal_r[N];
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=internal_r
#pragma HLS bind_storage variable=internal_r type=RAM_S2P impl=BRAM

  static uint8_t symbols_l[N];
#pragma HLS ARRAY_PARTITION dim=1 type=cyclic variable=symbols_l  factor=4
#pragma HLS bind_storage variable=symbols_l type=RAM_1WNR impl=BRAM

  static uint8_t  symbols_r[N];
#pragma HLS ARRAY_PARTITION dim=1 type=cyclic variable=symbols_r  factor=4
#pragma HLS bind_storage variable=symbols_r type=RAM_1WNR impl=BRAM

  static uint8_t xor_proc_i[64];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=xor_proc_i
  static uint8_t xor_proc_o[64];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=xor_proc_o

  static t_i_memo lwht_in_a;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_a.value
  static t_i_memo lwht_in_b;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_b.value

  static t_i_memo memo_in_a;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=memo_in_a.value

  static ap_uint<log2_gf_size> symbol_v;
  static ap_uint<log2_gf_size> symbol_x;

  static ap_uint<log2N+1> cnt_a; // LLRs from left  memory
  static ap_uint<log2N+1> cnt_b; // LLRs from right memory
  static ap_uint<log2N+1> cnt_c; // LLRs to left/right memories

  static ap_uint<log2N+1> cnt_u  = 0; // 
  static ap_uint<log2N+1> cnt_rw = 0; // right and first left
  static ap_uint<log2N+1> cnt_rd = 0; // 2nd right (read only)

  ap_uint<log2N+1> s; // optimize this with <log2_n + 1>
  ap_uint<log2N+1> i; // optimize this with <log2_n + 1>

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_proba_in [hls_generator:293]
  //
  cnt_c = 0; cnt_a = 0; cnt_b = 32;
  loop_f_1 : for (s = 0; s < 32; s += 1) {
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
  // f_function_freq_in [hls_generator:319]
  //
  cnt_c = 32; cnt_a = 0; cnt_b = 16;
  loop_f_2 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_freq_in [hls_generator:319]
  //
  cnt_c = 48; cnt_a = 32; cnt_b = 40;
  loop_f_3 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_freq_in [hls_generator:319]
  //
  cnt_c = 56; cnt_a = 48; cnt_b = 52;
  loop_f_4 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rep_after_f [hls_generator:539]
  //
  cnt_c = 60; cnt_a = 56; cnt_b = 58;
  loop_rep_5 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }
  cnt_c = 62; cnt_a = 60; cnt_b = 61;
  lwht_in_a   = internal_l[cnt_a];
  lwht_in_b   = internal_r[cnt_b];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal_l[cnt_c] = memo_in_a;
  internal_r[cnt_c] = memo_in_a;
  cnt_a = cnt_c;
  symbol_v = vec_decision( internal_l[cnt_a], false );
  loop_rep_6 : for (s = 0; s < 4; s += 1) {
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rw += 1;
    decoded  [cnt_u] = (s == 3) ?  symbol_v : (ap_uint<log2_gf_size>)0;
    cnt_u  += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_freq_in [hls_generator:837]
  //
  cnt_c = 56; cnt_a = 48; cnt_b = 52; cnt_rd = 0;
  loop_g_7 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rep_after_g [hls_generator:1076]
  //
  loop_rep_8 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 48];
    lwht_in_b   = internal_r[s + 52];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[s + 56] = memo_in_a;
    internal_r[s + 56] = memo_in_a;
  }
  lwht_in_a   = internal_l[60];
  lwht_in_b   = internal_r[61];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
  internal_l[60] = memo_in_a;
  internal_r[60] = memo_in_a;
  symbol_v = vec_decision( internal_l[60], false );
  loop_rep_9 : for (s = 0; s < 4; s += 1) {
    symbols  [cnt_u] = symbol_v;
    decoded  [cnt_u] = (s == 3) ?  symbol_v : (ap_uint<log2_gf_size>)0;
    decoded_r[cnt_u] = (s == 3) ?  symbol_v : (ap_uint<log2_gf_size>)0;
    cnt_u += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 4, pred_is_f =1) [hls_generator:1127]
  //
  cnt_rw = 0;
  cnt_rd = 4;
  loop_xor_10 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
//#pragma HLS dependence class=array direction=RAW variable=symbols_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=symbols_r type=inter distance=1 false
    symbol_v          = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = cnt_u; /* synchro */

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_freq_in [hls_generator:837]
  //
  cnt_c = 48; cnt_a = 32; cnt_b = 40; cnt_rd = 0;
  loop_g_11 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_proba_in [hls_generator:346]
  //
  cnt_c = 56; cnt_a = 48; cnt_b = 52;
  loop_f_12 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rep_after_f [hls_generator:539]
  //
  cnt_c = 60; cnt_a = 56; cnt_b = 58;
  loop_rep_13 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }
  cnt_c = 62; cnt_a = 60; cnt_b = 61;
  lwht_in_a   = internal_l[cnt_a];
  lwht_in_b   = internal_r[cnt_b];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal_l[cnt_c] = memo_in_a;
  internal_r[cnt_c] = memo_in_a;
  cnt_a = cnt_c;
  symbol_v = vec_decision( internal_l[cnt_a], false );
  loop_rep_14 : for (s = 0; s < 4; s += 1) {
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rw += 1;
    decoded  [cnt_u] = (s == 3) ?  symbol_v : (ap_uint<log2_gf_size>)0;
    cnt_u  += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_proba_in [hls_generator:864]
  //
  cnt_c = 56; cnt_a = 48; cnt_b = 52; cnt_rd = 8;
	loop_g_15 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=internal_r type=inter distance=1 false
    	lwht_in_a   = internal_l[cnt_a];
    	lwht_in_b   = internal_r[cnt_b];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], false);
    	internal_l[cnt_c] = memo_in_a;
    	internal_r[cnt_c] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_spc_after_g [hls_generator:1039]
  //
  cnt_a = 56;
  loop_spc_16 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols_l[cnt_rw] = symbol_v;
      symbols_r[cnt_rw] = symbol_v;
      xor_proc_i[s]     = symbol_v;
      cnt_a  += 1;
      cnt_rw += 1;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!
  v64_xor_processor(xor_proc_o, xor_proc_i, 3);

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 4, pred_is_f =0) [hls_generator:1127]
  //
  cnt_rw = 8;
  cnt_rd = 12;
  loop_xor_17 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
//#pragma HLS dependence class=array direction=RAW variable=symbols_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=symbols_r type=inter distance=1 false
    symbol_v          = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
    decoded  [cnt_u]  = xor_proc_o[i];
    cnt_u  += 1;
  }

  cnt_rw = cnt_u; /* synchro */

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 3, pred_is_f =1) [hls_generator:1127]
  //
  cnt_rw = 0;
  cnt_rd = 8;
  loop_xor_18 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
//#pragma HLS dependence class=array direction=RAW variable=symbols_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=symbols_r type=inter distance=1 false
    symbol_v          = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = cnt_u; /* synchro */

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_freq_in [hls_generator:837]
  //
  cnt_c = 32; cnt_a = 0; cnt_b = 16; cnt_rd = 0;
  loop_g_19 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_proba_in [hls_generator:346]
  //
  cnt_c = 48; cnt_a = 32; cnt_b = 40;
  loop_f_20 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_freq_in [hls_generator:319]
  //
  cnt_c = 56; cnt_a = 48; cnt_b = 52;
  loop_f_21 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_freq_in [hls_generator:319]
  //
  cnt_c = 60; cnt_a = 56; cnt_b = 58;
  loop_f_22 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rep_after_f [hls_generator:539]
  //
  cnt_c = 64; cnt_a = 62; cnt_b = 63;
  lwht_in_a   = internal_l[cnt_a];
  lwht_in_b   = internal_r[cnt_b];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal_l[cnt_c] = memo_in_a;
  internal_r[cnt_c] = memo_in_a;
  cnt_a = cnt_c;
  symbol_v = vec_decision( internal_l[cnt_a], false );
  loop_rep_23 : for (s = 0; s < 2; s += 1) {
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rw += 1;
    decoded  [cnt_u] = (s == 1) ?  symbol_v : (ap_uint<log2_gf_size>)0;
    cnt_u  += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_freq_in [hls_generator:837]
  //
  cnt_c = 60; cnt_a = 56; cnt_b = 58; cnt_rd = 16;
  loop_g_24 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_spc_after_g [hls_generator:1039]
  //
  cnt_a = 60;
  loop_spc_25 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols_l[cnt_rw] = symbol_v;
      symbols_r[cnt_rw] = symbol_v;
      xor_proc_i[s]     = symbol_v;
      cnt_a  += 1;
      cnt_rw += 1;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!
  v64_xor_processor(xor_proc_o, xor_proc_i, 1);

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 5, pred_is_f =1) [hls_generator:1127]
  //
  cnt_rw = 16;
  cnt_rd = 18;
  loop_xor_26 : for(i = 0; i < 2; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
//#pragma HLS dependence class=array direction=RAW variable=symbols_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=symbols_r type=inter distance=1 false
    symbol_v          = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
    decoded  [cnt_u]  = xor_proc_o[i];
    cnt_u  += 1;
  }

  cnt_rw = cnt_u; /* synchro */

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_freq_in [hls_generator:837]
  //
  cnt_c = 56; cnt_a = 48; cnt_b = 52; cnt_rd = 16;
  loop_g_27 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rate_1_after_g [hls_generator:944]
  //
  cnt_a = 56;
  loop_g_28 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
    symbol_v = vec_decision( internal_l[cnt_a], false );
    cnt_a += 1;
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rw += 1;
    xor_proc_i[s]     = symbol_v;
  }
  v64_xor_processor(xor_proc_o, xor_proc_i, 3);

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 4, pred_is_f =1) [hls_generator:1127]
  //
  cnt_rw = 16;
  cnt_rd = 20;
  loop_xor_29 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
//#pragma HLS dependence class=array direction=RAW variable=symbols_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=symbols_r type=inter distance=1 false
    symbol_v          = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
    decoded  [cnt_u]  = xor_proc_o[i];
    cnt_u  += 1;
  }

  cnt_rw = cnt_u; /* synchro */

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_proba_in [hls_generator:864]
  //
  cnt_c = 48; cnt_a = 32; cnt_b = 40; cnt_rd = 16;
	loop_g_30 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=internal_r type=inter distance=1 false
    	lwht_in_a   = internal_l[cnt_a];
    	lwht_in_b   = internal_r[cnt_b];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], false);
    	internal_l[cnt_c] = memo_in_a;
    	internal_r[cnt_c] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rate_1_after_g [hls_generator:944]
  //
  cnt_a = 48;
  loop_g_31 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
    symbol_v = vec_decision( internal_l[cnt_a], false );
    cnt_a += 1;
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rw += 1;
    xor_proc_i[s]     = symbol_v;
  }
  v64_xor_processor(xor_proc_o, xor_proc_i, 7);

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 3, pred_is_f =0) [hls_generator:1127]
  //
  cnt_rw = 16;
  cnt_rd = 24;
  loop_xor_32 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
//#pragma HLS dependence class=array direction=RAW variable=symbols_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=symbols_r type=inter distance=1 false
    symbol_v          = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
    decoded  [cnt_u]  = xor_proc_o[i];
    cnt_u  += 1;
  }

  cnt_rw = cnt_u; /* synchro */

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 2, pred_is_f =1) [hls_generator:1127]
  //
  cnt_rw = 0;
  cnt_rd = 16;
  loop_xor_33 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
//#pragma HLS dependence class=array direction=RAW variable=symbols_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=symbols_r type=inter distance=1 false
    symbol_v          = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = cnt_u; /* synchro */

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_proba_in [hls_generator:805]
  //
  cnt_c = 0; cnt_a = 0; cnt_b = 32; cnt_rd = 0;
  loop_g_34 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = channel[cnt_a];
    lwht_in_b   = channel[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_spc_after_g [hls_generator:998]
  //
  cnt_a = 0;
  loop_spc_35 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols_l[cnt_rw] = symbol_v;
      symbols_r[cnt_rw] = symbol_v;
      xor_proc_i[s]     = symbol_v;
      cnt_a  += 1;
      cnt_rw += 1;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!
  v64_xor_processor(xor_proc_o, xor_proc_i, 31);

  loop_spc_36 : for (i = 0; i < 32; i += 1) {
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
    decoded[cnt_u]  = xor_proc_o[i];
    cnt_u  += 1;
  }

}

