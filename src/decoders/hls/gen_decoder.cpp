#include "impl/f_datapath.hpp"
#include "impl/f_decision.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#define N            1024
#define log2N        10
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

  // f_function_freq_in<64>(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !
  // middle_node_pruned_rate_0
  loop_f0_1 : for (s = 0; s < 512; s += 1) {
#pragma HLS PIPELINE
    symbols[cnt_u] = 0;
    decoded[cnt_u] = 0;
    cnt_u += 1; 
  }

  // g_function_proba_in_after_rate_0
  cnt_c = 0; cnt_a = 0; cnt_b = 512;
  loop_g0_2 : for (s = 0; s < 512; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = channel[cnt_a]; // channel[s    ];
    lwht_in_b   = channel[cnt_b]; // channel[s + 512];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);
    internal_l[cnt_c] /*internal_l[s]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  // f_function_proba_in
  cnt_c = 512; cnt_a = 0; cnt_b = 256;
  loop_f_3 : for (s = 0; s < 256; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[cnt_a]; //internal_l[s + 0];
    lwht_in_b   = internal_r[cnt_b]; //internal_r[s + 256];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, true, 0);
    internal_l[cnt_c] /*internal_l[s + 512]*/ = memo_in_a;
    internal_r[cnt_c] /*internal_r[s + 512]*/ = memo_in_a;
    cnt_c += 1; cnt_a += 1; cnt_b += 1;
  }

  //middle_node_pruned_rep_after_f<gf_size>(internal + 512, decoded + 512, symbols + 512, 256);
  // middle_node_pruned_rep_after_f
  loop_rep_4 : for (s = 0; s < 128; s += 1) {
#pragma HLS PIPELINE
    lwht_in_a   = internal_l[s + 512];
    lwht_in_b   = internal_r[s + 640];
    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);
    internal_l[s + 768] = memo_in_a;
    internal_r[s + 768] = memo_in_a;
  }
  lwht_in_a   = internal_l[768];
  lwht_in_b   = internal_r[769];
  memo_in_a   = datapath(lwht_in_a, lwht_in_b, false, 0);
  internal_l[896] = memo_in_a;
  internal_r[896] = memo_in_a;
  symbol_v = vec_decision( internal_l[896], false );
  loop_rep_5 : for (s = 0; s < 256; s += 1) {
    symbols[cnt_u] = symbol_v;
    decoded[cnt_u] = symbol_v;
    cnt_u += 1;
  }

  // g_function_proba_in
  cnt_c = 512; cnt_a = 0; cnt_b = 256; cnt_rd = 512;
	loop_g_6 : for (s = 0; s < 256; s += 1) {
#pragma HLS PIPELINE
    	lwht_in_a   = internal_l[cnt_a/*s + 0*/];
    	lwht_in_b   = internal_r[cnt_b/*s + 256*/];
    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols[cnt_rd], false);
    	internal_l[cnt_c/*s + 512*/] = memo_in_a;
    	internal_r[cnt_c/*s + 512*/] = memo_in_a;
      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;
  }

  // middle_node_pruned_spc_after_g
  cnt_a = 512;
  loop_spc_7 : for (s = 0; s < 256; s += 1) {
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

  cnt_rw = 512; cnt_rd = 768;
  loop_xor_8 : for(i = 0; i < 256; i += 1){
#pragma HLS PIPELINE
    // symbols[512 + i] ^= symbols[768 + i];
    symbols[cnt_rw] = symbols[cnt_rw] ^ symbols[cnt_rd];
    cnt_rd += 1;
    cnt_rw += 1;
  }

}

