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
#pragma HLS bind_storage variable=internal_l type=RAM_1WNR impl=BRAM

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

  static ap_uint<log2_gf_size> symbol_v;
  static ap_uint<log2_gf_size> symbol_x;

  static ap_uint<log2N+1> cnt_a; // LLRs from left  memory
  static ap_uint<log2N+1> cnt_b; // LLRs from right memory
  static ap_uint<log2N+1> cnt_c; // LLRs to left/right memories

  static ap_uint<log2N+1> cnt_u  = 0; // 
  static ap_uint<log2N+1> cnt_rw = 0; // right and first left
  static ap_uint<log2N+1> cnt_rd = 0; // 2nd right (read only)

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_proba_in [hls_generator:293]
  //
  cnt_c = 0; cnt_a = 0; cnt_b = 32;
  loop_f_1 : for (int s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
    const auto lwht_in_a   = channel[cnt_a];
    const auto lwht_in_b   = channel[cnt_b];
    internal_l[cnt_c] = datapath(lwht_in_a, lwht_in_b, 0, true);
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rate_0 [hls_generator:407]
  //
  //loop_f0_2 : for (int s = 0; s < 16; s += 1) {
  //#pragma HLS PIPELINE
  //  symbols  [cnt_u] = 0;
  //  decoded  [cnt_u] = 0;
  //  decoded_r[cnt_u] = 0;
  //  cnt_u += 1; 
  //}

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_freq_in_after_rate_0 [hls_generator:707]
  //
  cnt_c = 32; cnt_a = 0; cnt_b = 16;
  loop_g0_3 : for (int s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
    symbols_l[cnt_rw] = 0;
    symbols_r[cnt_rw] = 0;
    cnt_rw += 1;
    decoded  [cnt_u ] = 0;
    cnt_u  += 1;
    internal_l[cnt_c] = datapath(internal_l[cnt_a], internal_l[cnt_b], 0, true);
    cnt_c += 1; // dst
    cnt_a += 1; // src 1
    cnt_b += 1; // src 2
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_proba_in [hls_generator:346]
  //
  cnt_c = 48; cnt_a = 32; cnt_b = 40;
  loop_f_4 : for (int s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_l type=inter false
    internal_l[cnt_c] = datapath(internal_l[cnt_a], internal_l[cnt_b], 0, true);
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rep_after_f [hls_generator:542]
  //
  cnt_c = 56; cnt_a = 48; cnt_b = 52;
  loop_rep_5 : for (int s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_l type=inter false
    internal_l[cnt_c] = datapath(internal_l[cnt_a], internal_l[cnt_b], 0, true);
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }
  cnt_c = 60; cnt_a = 56; cnt_b = 58;
  loop_rep_6 : for (int s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_l type=inter false
    internal_l[cnt_c] = datapath(internal_l[cnt_a], internal_l[cnt_b], 0, false);
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }
  cnt_c = 62; cnt_a = 60; cnt_b = 61;
  {
    internal_l[cnt_c] = datapath(internal_l[cnt_a], internal_l[cnt_b], false, 0);
    cnt_a = cnt_c;
    symbol_v = vec_decision( internal_l[cnt_a], false );
    loop_rep_7 : for (int s = 0; s < 8; s += 1) {
      symbols_l[cnt_rw] = symbol_v;
      symbols_r[cnt_rw] = symbol_v;
      cnt_rw += 1;
      decoded  [cnt_u] = (s == 7) ?  symbol_v : (ap_uint<log2_gf_size>)0;
      cnt_u  += 1;
    }
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_proba_in [hls_generator:867]
  //
  cnt_c = 48; cnt_a = 32; cnt_b = 40; cnt_rd = 16;
	loop_g_8 : for (int s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
    	internal_l[cnt_c] = datapath(internal_l[cnt_a], internal_l[cnt_b], symbols_l[cnt_rd], false);
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_proba_in [hls_generator:346]
  //
  cnt_c = 56; cnt_a = 48; cnt_b = 52;
  loop_f_9 : for (int s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_l type=inter false
    internal_l[cnt_c] = datapath(internal_l[cnt_a], internal_l[cnt_b], 0, true);
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rate_0 [hls_generator:407]
  //
  //loop_f0_10 : for (int s = 0; s < 2; s += 1) {
  //#pragma HLS PIPELINE
  //  symbols  [cnt_u] = 0;
  //  decoded  [cnt_u] = 0;
  //  decoded_r[cnt_u] = 0;
  //  cnt_u += 1; 
  //}

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_freq_in_after_rate_0 [hls_generator:707]
  //
  cnt_c = 60; cnt_a = 56; cnt_b = 58;
  loop_g0_11 : for (int s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
    symbols_l[cnt_rw] = 0;
    symbols_r[cnt_rw] = 0;
    cnt_rw += 1;
    decoded  [cnt_u ] = 0;
    cnt_u  += 1;
    internal_l[cnt_c] = datapath(internal_l[cnt_a], internal_l[cnt_b], 0, true);
    cnt_c += 1; // dst
    cnt_a += 1; // src 1
    cnt_b += 1; // src 2
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rate_1_after_g [hls_generator:947]
  //
  cnt_a = 60;
  loop_g_12 : for (int s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
    symbol_v = vec_decision( internal_l[cnt_a], false );
    cnt_a += 1;
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rw += 1;
    xor_proc_i[s]     = symbol_v;
  }
  v64_xor_processor(xor_proc_o, xor_proc_i, 1);

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 5, pred_is_f =1) [hls_generator:1121]
  //
  cnt_rw = 24;
  cnt_rd = 26;
  loop_xor_13 : for(int i = 0; i < 2; i += 1){
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
  // g_function_proba_in [hls_generator:867]
  //
  cnt_c = 56; cnt_a = 48; cnt_b = 52; cnt_rd = 24;
	loop_g_14 : for (int s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
    	internal_l[cnt_c] = datapath(internal_l[cnt_a], internal_l[cnt_b], symbols_l[cnt_rd], false);
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rate_1_after_g [hls_generator:947]
  //
  cnt_a = 56;
  loop_g_15 : for (int s = 0; s < 4; s += 1) {
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
  // xor procesing (level 4, pred_is_f =0) [hls_generator:1121]
  //
  cnt_rw = 24;
  cnt_rd = 28;
  loop_xor_16 : for(int i = 0; i < 4; i += 1){
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
  // xor procesing (level 3, pred_is_f =0) [hls_generator:1121]
  //
  cnt_rw = 16;
  cnt_rd = 24;
  loop_xor_17 : for(int i = 0; i < 8; i += 1){
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
  // xor procesing (level 2, pred_is_f =1) [hls_generator:1121]
  //
  cnt_rw = 0;
  cnt_rd = 16;
  loop_xor_18 : for(int i = 0; i < 16; i += 1){
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
  // g_function_proba_in [hls_generator:808]
  //
  cnt_c = 0; cnt_a = 0; cnt_b = 32; cnt_rd = 0;
  loop_g_19 : for (int s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_l type=inter false
    const auto lwht_in_a   = channel[cnt_a];
    const auto lwht_in_b   = channel[cnt_b];
    internal_l[cnt_c] = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], false);
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_proba_in [hls_generator:346]
  //
  cnt_c = 32; cnt_a = 0; cnt_b = 16;
  loop_f_20 : for (int s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_l type=inter false
    const auto lwht_in_a   = internal_l[cnt_a];
    const auto lwht_in_b   = internal_l[cnt_b];
    internal_l[cnt_c] = datapath(lwht_in_a, lwht_in_b, 0, true);
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // f_function_freq_in [hls_generator:319]
  //
  cnt_c = 48; cnt_a = 32; cnt_b = 40;
  loop_f_21 : for (int s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_l type=inter false
    internal_l[cnt_c] = datapath(internal_l[cnt_a], internal_l[cnt_b], 0, false);
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_rate_0 [hls_generator:407]
  //
  //loop_f0_22 : for (int s = 0; s < 4; s += 1) {
  //#pragma HLS PIPELINE
  //  symbols  [cnt_u] = 0;
  //  decoded  [cnt_u] = 0;
  //  decoded_r[cnt_u] = 0;
  //  cnt_u += 1; 
  //}

  ////////////////////////////////////////////////////////////////////////////
  //
  // g_function_freq_in_after_rate_0 [hls_generator:707]
  //
  cnt_c = 56; cnt_a = 48; cnt_b = 52;
  loop_g0_23 : for (int s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
    symbols_l[cnt_rw] = 0;
    symbols_r[cnt_rw] = 0;
    cnt_rw += 1;
    decoded  [cnt_u ] = 0;
    cnt_u  += 1;
    internal_l[cnt_c] = datapath(internal_l[cnt_a], internal_l[cnt_b], 0, true);
    cnt_c += 1; // dst
    cnt_a += 1; // src 1
    cnt_b += 1; // src 2
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_spc_after_g [hls_generator:1033]
  //
  cnt_a = 56;
  loop_spc_24 : for (int s = 0; s < 4; s += 1) {
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
  // xor procesing (level 4, pred_is_f =1) [hls_generator:1121]
  //
  cnt_rw = 32;
  cnt_rd = 36;
  loop_xor_25 : for(int i = 0; i < 4; i += 1){
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
  // g_function_freq_in [hls_generator:840]
  //
  cnt_c = 48; cnt_a = 32; cnt_b = 40; cnt_rd = 32;
  loop_g_26 : for (int s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_l type=inter false
    internal_l[cnt_c] = datapath(internal_l[cnt_a], internal_l[cnt_b], symbols_l[cnt_rd], true);
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_spc_after_g [hls_generator:1033]
  //
  cnt_a = 48;
  loop_spc_27 : for (int s = 0; s < 8; s += 1) {
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
  v64_xor_processor(xor_proc_o, xor_proc_i, 7);

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 3, pred_is_f =1) [hls_generator:1121]
  //
  cnt_rw = 32;
  cnt_rd = 40;
  loop_xor_28 : for(int i = 0; i < 8; i += 1){
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
  // g_function_proba_in [hls_generator:867]
  //
  cnt_c = 32; cnt_a = 0; cnt_b = 16; cnt_rd = 32;
	loop_g_29 : for (int s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false
    	internal_l[cnt_c] = datapath(internal_l[cnt_a], internal_l[cnt_b], symbols_l[cnt_rd], false);
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  ////////////////////////////////////////////////////////////////////////////
  //
  // middle_node_pruned_spc_after_g [hls_generator:1033]
  //
  cnt_a = 32;
  loop_spc_30 : for (int s = 0; s < 16; s += 1) {
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
  v64_xor_processor(xor_proc_o, xor_proc_i, 15);

  ////////////////////////////////////////////////////////////////////////////
  //
  // xor procesing (level 2, pred_is_f =0) [hls_generator:1121]
  //
  cnt_rw = 32;
  cnt_rd = 48;
  loop_xor_31 : for(int i = 0; i < 16; i += 1){
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

}

