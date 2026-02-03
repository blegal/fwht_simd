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

#pragma HLS ARRAY_PARTITION dim=1 type=cyclic variable=decoded  factor=4
#pragma HLS bind_storage variable=decoded type=RAM_S2P impl=BRAM

  static t_i_memo internal_l[N];
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=internal_l
#pragma HLS bind_storage variable=internal_l type=RAM_S2P impl=BRAM

  static t_i_memo internal_r[N];
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=internal_r
#pragma HLS bind_storage variable=internal_r type=RAM_S2P impl=BRAM

  static uint8_t  symbols_l[N];
//#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=symbols_l
#pragma HLS ARRAY_PARTITION dim=1 type=cyclic variable=symbols_l  factor=4
#pragma HLS bind_storage variable=symbols_l type=RAM_1WNR impl=BRAM

  static uint8_t  symbols_r[N];
//#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=symbols_r
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

  static int cnt_a; // LLRs from left  memory
  static int cnt_b; // LLRs from right memory
  static int cnt_c; // LLRs to left/right memories

  static int cnt_u  = 0; // 
  static int cnt_rw = 0; // right and first left
  static int cnt_rd = 0; // 2nd right (read only)

  static ap_uint<log2N+1> s; // optimize this with <log2_n + 1>
  static ap_uint<log2N+1> i; // optimize this with <log2_n + 1>

  // f_function_proba_in
  cnt_c = 0; cnt_a = 0; cnt_b = 128;
  loop_f_1 : for (s = 0; s < 128; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = channel[cnt_a]; // channel[s    ];
    lwht_in_b   = channel[cnt_b]; // channel[s + 128];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // g_function_freq_in_after_rate_0
  cnt_c = 128; cnt_a = 0; cnt_b = 64;
  loop_g0_3 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    symbols_l  [cnt_rw] = 0;
    symbols_r  [cnt_rw] = 0;
    cnt_rw += 1;
    decoded    [cnt_u ] = 0;
    cnt_u  += 1; 
    lwht_in_a   = internal_l[cnt_a];
    lwht_in_b   = internal_r[cnt_b];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1;
    cnt_a += 1;
    cnt_b += 1;
  }

  // g_function_proba_in_after_rate_0
  cnt_c = 192; cnt_a = 128; cnt_b = 160;
  loop_g0_5 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    symbols_l[cnt_rw] = 0;
    symbols_r[cnt_rw] = 0;
    decoded  [cnt_u ] = 0;
    cnt_u  += 1; 
    cnt_rw += 1;
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 128];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 160];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] = memo_in_a;
    internal_r[cnt_c] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // g_function_proba_in_after_rate_0
  cnt_c = 224; cnt_a = 192; cnt_b = 208;
  loop_g0_7 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    symbols_l[cnt_rw] = 0;
    symbols_r[cnt_rw] = 0;
    decoded  [cnt_u ] = 0;
    cnt_u  += 1; 
    cnt_rw += 1;
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 192];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 208];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 224]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 224]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // g_function_proba_in_after_rate_0
  cnt_c = 240; cnt_a = 224; cnt_b = 232;
  loop_g0_9 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    symbols_l[cnt_rw] = 0;
    symbols_r[cnt_rw] = 0;
    decoded  [cnt_u ] = 0;
    cnt_u  += 1; 
    cnt_rw += 1;
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
  	  symbols_l[cnt_rw] = symbol_v;
  	  symbols_r[cnt_rw] = symbol_v;
      cnt_a  += 1;
      cnt_rw += 1;
      xor_proc_i[s] = symbol_v;
  }
  v64_xor_processor(xor_proc_o, xor_proc_i, 0x07);

  cnt_rw = 112;
  cnt_rd = 120;
  loop_xor_11 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
#pragma HLS dependence variable=symbols_l type=inter false
#pragma HLS dependence variable=symbols_r type=inter false
    symbol_v = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
    decoded  [cnt_u] = xor_proc_o[i];
    cnt_u  += 1;
  }

  cnt_rw = 96;
  cnt_rd = 112;
  loop_xor_12 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
#pragma HLS dependence variable=symbols_l type=inter false
#pragma HLS dependence variable=symbols_r type=inter false
    symbol_v = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 64;
  cnt_rd = 96;
  loop_xor_13 : for(i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
#pragma HLS dependence variable=symbols_l type=inter false
#pragma HLS dependence variable=symbols_r type=inter false
    symbol_v = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = 0;
  cnt_rd = 64;
  loop_xor_14 : for(i = 0; i < 64; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
#pragma HLS dependence variable=symbols_l type=inter false
#pragma HLS dependence variable=symbols_r type=inter false
    symbol_v = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
  }

  cnt_rw = cnt_u; /* synchro */

  // g_function_proba_in
  cnt_c = 0; cnt_a = 0; cnt_b = 128; cnt_rd = 0;
  loop_g_15 : for (s = 0; s < 128; s += 1) {
#pragma HLS PIPELINE
#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter false
#pragma HLS dependence class=array direction=RAW variable=internal_r type=inter false
    lwht_in_a   = channel[cnt_a /*s    */];
    lwht_in_b   = channel[cnt_b /*s + 128*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], false);
    internal_l[cnt_c/*s*/] = memo_in_a;
    internal_r[cnt_c/*s*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // f_function_proba_in
  cnt_c = 128; cnt_a = 0; cnt_b = 64;
  loop_f_16 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
#pragma HLS dependence variable=internal_l type=inter false
#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 0];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 64];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[cnt_c] /*internal_l[s + 128]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 128]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_freq_in
  cnt_c = 192; cnt_a = 128; cnt_b = 160;
  loop_f_17 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
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
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
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
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
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
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a]; // internal_l[s + 240];
    lwht_in_b   = internal_r[cnt_b]; // internal_r[s + 244];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s + 248]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 248]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 248, decoded + 128, symbols + 128, 4);

  ////////////////////////////////////////////////////////////////
  // middle_node_pruned_rep_after_f
  loop_rep_21 : for (s = 0; s < 2; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
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
    symbols_l  [cnt_rw] = symbol_v;
    symbols_r  [cnt_rw] = symbol_v;
    decoded  [cnt_u] = (s == 3) ?  symbol_v : (ap_uint<log2_gf_size>)0;
    cnt_u  += 1;
    cnt_rw += 1;
  }
  ////////////////////////////////////////////////////////////////

  // g_function_freq_in
  cnt_c = 248; cnt_a = 240; cnt_b = 244; cnt_rd = 128;
  loop_g_23 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a /*s + 240*/];
    lwht_in_b   = internal_r[cnt_b /*s + 244*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], true);
    internal_l[cnt_c /*s + 248*/] = memo_in_a;
    internal_r[cnt_c /*s + 248*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 248;
  loop_spc_24 : for (s = 0; s < 4; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols_l[cnt_rw] = symbol_v ;
      symbols_r[cnt_rw] = symbol_v ;
      xor_proc_i[s] = symbol_v;
      cnt_a  += 1;
      cnt_rw += 1;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!
  //local_remove_xors<4>(decoded, decoded_r, cnt_u - 4);
  v64_xor_processor(xor_proc_o, xor_proc_i, 0x03);

  cnt_rw = 128; cnt_rd = 132;
  loop_xor_25 : for(i = 0; i < 4; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
//#pragma HLS dependence variable=symbols_l type=inter false
//#pragma HLS dependence variable=symbols_r type=inter false
    symbol_v = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
    decoded  [cnt_u] = xor_proc_o[i];
    cnt_u  += 1;
  }

  cnt_rw = cnt_u; /* synchro */

  // g_function_freq_in
  cnt_c = 240; cnt_a = 224; cnt_b = 232; cnt_rd = 128;
  loop_g_26 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a /*s + 224*/];
    lwht_in_b   = internal_r[cnt_b /*s + 232*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], true);
    internal_l[cnt_c /*s + 240*/] = memo_in_a;
    internal_r[cnt_c /*s + 240*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 240;
  loop_spc_27 : for (s = 0; s < 8; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols_l[cnt_rw] = symbol_v ;
      symbols_r[cnt_rw] = symbol_v ;
      cnt_a  += 1;
      cnt_rw += 1;
      xor_proc_i[s]      = symbol_v;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!
  //local_remove_xors<8>(decoded, decoded_r, cnt_u - 8);
  v64_xor_processor(xor_proc_o, xor_proc_i, 0x07);

  cnt_rw = 128; cnt_rd = 136;
  loop_xor_28 : for(i = 0; i < 8; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
#pragma HLS dependence variable=symbols_l type=inter false
//#pragma HLS dependence variable=symbols_r type=inter false
    symbol_v = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;

    decoded  [cnt_u]  = xor_proc_o[i];
    cnt_u  += 1;
  }

  cnt_rw = cnt_u; /* synchro */

  // g_function_freq_in
  cnt_c = 224; cnt_a = 192; cnt_b = 208; cnt_rd = 128;
  loop_g_29 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a /*s + 192*/];
    lwht_in_b   = internal_r[cnt_b /*s + 208*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], true);
    internal_l[cnt_c /*s + 224*/] = memo_in_a;
    internal_r[cnt_c /*s + 224*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 224;
  loop_spc_30 : for (s = 0; s < 16; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols_l[cnt_rw] = symbol_v ;
      symbols_r[cnt_rw] = symbol_v ;
      cnt_a  += 1;
      cnt_rw += 1;
      xor_proc_i[s]      = symbol_v;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!
  //local_remove_xors<16>(decoded, decoded_r, cnt_u - 16);
  v64_xor_processor(xor_proc_o, xor_proc_i, 0x0F);

  cnt_rw = 128; cnt_rd = 144;
  loop_xor_31 : for(i = 0; i < 16; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
//#pragma HLS dependence variable=symbols_l type=inter false
//#pragma HLS dependence variable=symbols_r type=inter false
    symbol_v = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
    decoded  [cnt_u]  = xor_proc_o[i];
    cnt_u  += 1;
  }

  cnt_rw = cnt_u; /* synchro */

  // g_function_freq_in
  cnt_c = 192; cnt_a = 128; cnt_b = 160; cnt_rd = 128;
  loop_g_32 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    lwht_in_a   = internal_l[cnt_a /*s + 128*/];
    lwht_in_b   = internal_r[cnt_b /*s + 160*/];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], true);
    internal_l[cnt_c /*s + 192*/] = memo_in_a;
    internal_r[cnt_c /*s + 192*/] = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 192;
  loop_spc_33 : for (s = 0; s < 32; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols_l[cnt_rw] = symbol_v ;
      symbols_r[cnt_rw] = symbol_v ;
      cnt_a  += 1;
      cnt_rw += 1;
      xor_proc_i[s]      = symbol_v;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!
  //local_remove_xors<32>(decoded, decoded_r, cnt_u - 32);
  v64_xor_processor(xor_proc_o, xor_proc_i, 0x1F);

  cnt_rw = 128; cnt_rd = 160;
  loop_xor_34 : for(i = 0; i < 32; i += 1){
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=4
#pragma HLS dependence variable=symbols_l type=inter false
//#pragma HLS dependence variable=symbols_r type=inter false
    symbol_v = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];
    symbols_l[cnt_rw] = symbol_v;
    symbols_r[cnt_rw] = symbol_v;
    cnt_rd += 1;
    cnt_rw += 1;
    decoded  [cnt_u]  = xor_proc_o[i];
    cnt_u  += 1;
  }

  cnt_rw = cnt_u; /* synchro */

  // g_function_proba_in
  cnt_c = 128; cnt_a = 0; cnt_b = 64; cnt_rd = 128;
	loop_g_35 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
//#pragma HLS dependence variable=internal_l type=inter false
//#pragma HLS dependence variable=internal_r type=inter false
    	lwht_in_a   = internal_l[cnt_a/*s + 0*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 64*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], false);
    	internal_l[cnt_c/*s + 128*/] = memo_in_a;
    	internal_r[cnt_c/*s + 128*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 128;
  loop_spc_36 : for (s = 0; s < 64; s += 1) {
#pragma HLS PIPELINE
      symbol_v = vec_decision( internal_l[cnt_a], false );
      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);
      symbols_l[cnt_rw] = symbol_v ;
      symbols_r[cnt_rw] = symbol_v ;
      cnt_a  += 1;
      cnt_rw += 1;
      xor_proc_i[s] = symbol_v;
  }
  // SPC processing !!!
  //if ( symbol_x != 0 ) {
  //    printf("We have a SPC decoding error ;-)\n");
  //}
  // SPC processing !!!
  v64_xor_processor(xor_proc_o, xor_proc_i, 0x3F);

  loop_spc_37 : for (i = 0; i < 64; i += 1) {
    decoded  [cnt_u]  = xor_proc_o[i];
    cnt_u  += 1;
  }

}

