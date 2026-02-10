//
// Created by legal on 29/06/2025.
//

#ifndef _frozen_tree_generator_
#define _frozen_tree_generator_

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>

enum next_node {
    RATE_0,
    RATE_1_FROM_F,
    RATE_1_FROM_G,
    REP_FROM_F,
    REP_FROM_G,
    SPC_FROM_F,
    SPC_FROM_G,
    LEAF_RATE_0,
    LEAF_RATE_1_FROM_F,
    LEAF_RATE_1_FROM_G,
    MID_NODE_FROM_F,
    MID_NODE_FROM_G,
    USELESS
};

class hls_generator {
private:
    std::ofstream ofile;
    const int     GF;

public:
    bool verbose = false;

    bool en_rate_0   = true;
    bool en_rate_1   = true;
    bool en_rate_spc = true;
    bool en_rate_rep = true;

    int loop_id = 1;

public:
    hls_generator(const int n, const int gf) : GF(gf) {
        next_node_status.resize(2 * n);
    }

    ~hls_generator() {
        // nothing
    }

    std::vector<next_node> next_node_status;

    void analyze(const std::string filen, const int * frozen, const int size) {

        //
        //
        //

        if (verbose == true) {
            printf("\nFrozen matrix:\n");
            for (int i = 0; i < size; i += 1) {
                if ((i %  8) == 0) printf(" | ");
                if ((i % 32) == 0) printf("\n | ");
                printf("%2d ", frozen[i]);
            }
            printf(" |\n");
        }

        //
        //
        //

        int K_value = 0;
        for (int i = 0; i < size; i += 1) {
            K_value += (frozen[i] == false);
        }

        //
        //
        //
        std::cout << "(II) Creating configuration file" << std::endl;
        std::string nfilen = filen.substr(0,filen.find_last_of('.'))+".hpp";
        std::cout << "(II) - Opening file (" << nfilen << ")" << std::endl;
        ofile.open(nfilen.c_str());
        if ( ofile.is_open() == false ) {
            printf("\nError opening output file !\n");
            exit( EXIT_FAILURE );
        }
        ofile << "//" << std::endl;
        ofile << "//" << std::endl;
        ofile << "//" << std::endl;
        ofile << "//////////////////////////////////////////////////////////////////////" << std::endl;
        ofile << "//" << std::endl;
        ofile << "//" << std::endl;
        ofile << "#define t_N            " << size << std::endl;
        ofile << "#define t_log2N        " << (int)std::log2(size) << std::endl;
        ofile << "//" << std::endl;
        ofile << "#define t_K            " << K_value << std::endl;
        ofile << "//" << std::endl;
        ofile << "#define t_GF           " << GF << std::endl;
        ofile << "#define t_log2GF       " << (int)std::log2(GF) << std::endl;
        ofile << "//" << std::endl;
        ofile << "#define en_rate_0      " << en_rate_0   << std::endl;
        ofile << "#define en_rate_1      " << en_rate_1   << std::endl;
        ofile << "#define en_rate_spc    " << en_rate_spc << std::endl;
        ofile << "#define en_rate_rep    " << en_rate_rep << std::endl;
        ofile << "//" << std::endl;
        ofile << "//////////////////////////////////////////////////////////////////////" << std::endl;
        ofile << "//" << std::endl;
        ofile << "//" << std::endl;
        ofile << "//" << std::endl;
        ofile.close();
        std::cout << "(II) - Closing file (" << nfilen << ")" << std::endl;
        //
        //

        std::cout << "(II) Creating decoder file" << std::endl;
        std::cout << "(II) - Opening file (" << filen << ")" << std::endl;
        ofile.open(filen.c_str());
        if ( ofile.is_open() == false ) {
            printf("\nError opening output file !\n");
            exit( EXIT_FAILURE );
        }

        ofile << "#include \"impl/f_datapath.hpp\""  << std::endl;
        ofile << "#include \"impl/f_decision.hpp\""  << std::endl;
        ofile << "#include \"impl/f_xor_processor.hpp\""  << std::endl;

        ofile << "//" << std::endl;
        ofile << "//" << std::endl;
        ofile << "//" << std::endl;
        ofile << "//////////////////////////////////////////////////////////////////////" << std::endl;
        ofile << "//" << std::endl;
        ofile << "//" << std::endl;
        ofile << "//" << std::endl;
        ofile << "#define N            " << size << std::endl;
        ofile << "#define log2N        " << (int)std::log2(size) << std::endl;
        ofile << "//" << std::endl;
        ofile << "//" << std::endl;
        ofile << "//" << std::endl;
        ofile << "//////////////////////////////////////////////////////////////////////" << std::endl;
        ofile << "//" << std::endl;
        ofile << "//" << std::endl;
        ofile << "//" << std::endl;
        //      ofile << "template <int gf_size>" << std::endl;
        ofile << "void the_decoder_v2(" <<  std::endl <<
                 "			t_i_memo channel[N],"            << std::endl <<
                 "			uint8_t  decoded[N])"            << std::endl;
        ofile << "{" << std::endl;

        ofile << "#pragma HLS bind_storage variable=channel type=RAM_2P  impl=BRAM"  << std::endl;
        ofile << std::endl;
        ofile << "#pragma HLS ARRAY_PARTITION dim=1 type=cyclic variable=decoded  factor=4" << std::endl;
        ofile << "#pragma HLS bind_storage variable=decoded type=RAM_S2P impl=BRAM" << std::endl;
        ofile << std::endl;
        ofile << "  static t_i_memo internal_l[N];" << std::endl;
        ofile << "#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=internal_l" << std::endl;
        ofile << "#pragma HLS bind_storage variable=internal_l type=RAM_S2P impl=BRAM" << std::endl;
        ofile << std::endl;
        ofile << "  static t_i_memo internal_r[N];" << std::endl;
        ofile << "#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=internal_r" << std::endl;
        ofile << "#pragma HLS bind_storage variable=internal_r type=RAM_S2P impl=BRAM" << std::endl;
        ofile << std::endl;
        ofile << "  static uint8_t symbols_l[N];" << std::endl;
        ofile << "#pragma HLS ARRAY_PARTITION dim=1 type=cyclic variable=symbols_l  factor=4" << std::endl;
        ofile << "#pragma HLS bind_storage variable=symbols_l type=RAM_1WNR impl=BRAM" << std::endl;
        ofile << std::endl;
        ofile << "  static uint8_t  symbols_r[N];" << std::endl;
        ofile << "#pragma HLS ARRAY_PARTITION dim=1 type=cyclic variable=symbols_r  factor=4" << std::endl;
        ofile << "#pragma HLS bind_storage variable=symbols_r type=RAM_1WNR impl=BRAM" << std::endl;
        ofile << std::endl;
        ofile << "  static uint8_t xor_proc_i[64];" << std::endl;
        ofile << "#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=xor_proc_i" << std::endl;
        ofile << "  static uint8_t xor_proc_o[64];" << std::endl;
        ofile << "#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=xor_proc_o" << std::endl;
        ofile << std::endl;
        ofile << "  static t_i_memo lwht_in_a;" << std::endl;
        ofile << "#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_a.value" << std::endl;
        ofile << "  static t_i_memo lwht_in_b;" << std::endl;
        ofile << "#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=lwht_in_b.value" << std::endl;
        ofile << std::endl;
        ofile << "  static t_i_memo memo_in_a;" << std::endl;
        ofile << "#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=memo_in_a.value" << std::endl;
        ofile << std::endl;
        ofile << "  static ap_uint<log2_gf_size> symbol_v;" << std::endl;
        ofile << "  static ap_uint<log2_gf_size> symbol_x;" << std::endl;
        ofile << std::endl;
        ofile << "  static ap_uint<log2N+1> cnt_a; // LLRs from left  memory" << std::endl;
        ofile << "  static ap_uint<log2N+1> cnt_b; // LLRs from right memory" << std::endl;
        ofile << "  static ap_uint<log2N+1> cnt_c; // LLRs to left/right memories" << std::endl;
        ofile << std::endl;
        ofile << "  static ap_uint<log2N+1> cnt_u  = 0; // " << std::endl;
        ofile << "  static ap_uint<log2N+1> cnt_rw = 0; // right and first left" << std::endl;
        ofile << "  static ap_uint<log2N+1> cnt_rd = 0; // 2nd right (read only)" << std::endl;
        ofile << std::endl;
        ofile << "  ap_uint<log2N+1> s; // optimize this with <log2_n + 1>" << std::endl;
        ofile << "  ap_uint<log2N+1> i; // optimize this with <log2_n + 1>" << std::endl;
        ofile << std::endl;

        const int n_elmnt = execute(frozen, 0, next_node_status.data(), 0, size);
        next_node_status.resize(n_elmnt);

        ofile << "}" << std::endl;
        ofile << std::endl;

        ofile.close();
        if (verbose == true) {
            printf("-> #elements : %d\n", n_elmnt);
            printf("-> #elements : %zu\n", next_node_status.size());
        }
        std::cout << "(II) - Closing file (" << filen << ")" << std::endl;
    }

private:
    void add_space(const int level) {
        for (int i = 0; i < level; i += 1) {
            printf(" ");
        }
    }

    void indentation(const int level) {
        for (int z = 0; z < level; z += 1)
            printf("+  ");
    }

#define _enable_pruning_

    int execute(
        const int * frozen,
        const int   curr_frozen,
        next_node * array,
        int         curr_elmnt, // identifiant du premier frozen de la branche
        const int   size,       // nombre de LLRs a ce niveau
        const int   p_llrs    = 0,
        const int   level     = 1,
        const bool  pred_is_f = false) {
        const int n = size / 2; // Assuming size is the number of symbols

        //
        // Analyse de la branche gauche
        //

        int sum_l = 0;
        for (int i = 0; i < n; i++)
            sum_l += frozen[curr_frozen + i];

        const bool is_rate0_after_f = en_rate_0   && (sum_l == n);
        const bool is_rate1_after_f = en_rate_1   && (sum_l == 0);
        const bool is_rep_after_f   = en_rate_rep && (sum_l == (n - 1)) && (frozen[curr_frozen + n - 1] == false);
        const bool is_spc_after_f   = en_rate_spc && (sum_l == 1) && (frozen[0] == true);

        int sum_r = 0;
        for (int i = 0; i < n; i++)
            sum_r += frozen[curr_frozen + n + i];

        const bool is_rate0_after_g = en_rate_0 && (sum_r == n);
        const bool is_rate1_after_g = en_rate_1 && (sum_r == 0);
        const bool is_rep_after_g   = en_rate_rep && (sum_r == (n - 1)) && (frozen[curr_frozen + n - 1] == false);
        const bool is_spc_after_g   = en_rate_spc && (sum_r == 1) && (frozen[0] == true);

        //
        //
        //
        if (verbose) {
            indentation(level);
            printf("NODE LEVEL (%d)\n", size);
            indentation(level);
            printf("F edge : ");
            for (int i = 0; i < n; i++) {
                printf("%d", frozen[curr_frozen + i]);
            }
            printf("\n");
            printf("after_f [%d %d %d %d]\n", is_rate0_after_f, is_rate1_after_f, is_rep_after_f, is_spc_after_f);
        }

        if (is_rate0_after_f) {
            //
            // Rien à faire du tout !
            //
            //ofile << "  // f_function_freq_in<" << GF << ">(....); NO F COMPUTATIONS AS WE HAVE A RATE 0 NODE AFTER !" << std::endl;
        } else if (level == 1) {
            if (verbose) {
                indentation(level);
                printf("\e[1;31m Internal[%d...%d] <= Channel(%d...%d) (f) Channel(%d...%d) \e[0m\n", 0, n, 0, n, n, n + n);
            }
            //
          //ofile << "\t" << "f_function_proba_in<" << GF << ">(internal, channel, channel + " << n << ", " << n << ");" << std::endl;
            //
            ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
            ofile << "  //"   << std::endl;
            ofile << "  // f_function_proba_in [hls_generator:" << __LINE__ << "]" << std::endl;
            ofile << "  //"   << std::endl;
            ofile << "  cnt_c = 0; cnt_a = 0; cnt_b = " << n << ";" << std::endl;
            ofile << "  loop_f_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {" << std::endl;
            ofile << "#pragma HLS PIPELINE"   << std::endl;
            ofile << "    lwht_in_a   = channel[cnt_a];"   << std::endl;
            ofile << "    lwht_in_b   = channel[cnt_b];"   << std::endl;
            ofile << "    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);"   << std::endl;
            ofile << "    internal_l[cnt_c] = memo_in_a;"      << std::endl;
            ofile << "    internal_r[cnt_c] = memo_in_a;"      << std::endl;
            ofile << "    cnt_c += 1; cnt_a += 1; cnt_b += 1;" << std::endl;
            ofile << "  }"   << std::endl;
            ofile << std::endl;
            //
            //
        } else {
            if (pred_is_f) {
                if (verbose) {
                    indentation(level);
                    printf("\e[1;31m Internal[%d...%d] <= Internal(%d...%d) (f) Internal(%d...%d) \e[0m\n", p_llrs + size, p_llrs + size + n, p_llrs, p_llrs + n, p_llrs + n, p_llrs + n + n);
                }
                //
              //ofile << "\t" << "f_function_freq_in<" << GF << ">(internal + " << p_llrs + size << ", internal + " << p_llrs << ", internal + " << p_llrs + n << ", " << n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // f_function_freq_in [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  cnt_c = " << (p_llrs + size) << "; cnt_a = " <<  p_llrs    << "; cnt_b = " << (p_llrs+n) << ";" << std::endl;
                ofile << "  loop_f_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {" << std::endl;
                ofile << "#pragma HLS PIPELINE"   << std::endl;
                ofile << "//#pragma HLS dependence variable=internal_l type=inter false"   << std::endl;
                ofile << "//#pragma HLS dependence variable=internal_r type=inter false"   << std::endl;
                ofile << "    lwht_in_a   = internal_l[cnt_a];"   << std::endl;
                ofile << "    lwht_in_b   = internal_r[cnt_b];"   << std::endl;
                ofile << "    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);"   << std::endl;
                ofile << "    internal_l[cnt_c] = memo_in_a;"   << std::endl;
                ofile << "    internal_r[cnt_c] = memo_in_a;"   << std::endl;
                ofile << "    cnt_c += 1; cnt_a += 1; cnt_b += 1;" << std::endl;
                ofile << "  }"   << std::endl;
                ofile << std::endl;
                //
                //
            } else {
                if (verbose) {
                    indentation(level);
                    printf("\e[1;31m Internal[%d...%d] <= Internal(%d...%d) (f) Internal(%d...%d) \e[0m\n", p_llrs + size, p_llrs + size + n, p_llrs, p_llrs + n, p_llrs + n, p_llrs + n + n);
                }
                //
              //ofile << "\t" << "f_function_proba_in<gf_size>(internal + " << p_llrs + size << ", internal + " << p_llrs << ", internal + " << p_llrs + n << ", " << n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // f_function_proba_in [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  cnt_c = " << (p_llrs + size) << "; cnt_a = " <<  p_llrs    << "; cnt_b = " << (p_llrs+n) << ";" << std::endl;
                ofile << "  loop_f_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {" << std::endl;
                ofile << "#pragma HLS PIPELINE"   << std::endl;
                ofile << "//#pragma HLS dependence variable=internal_l type=inter false"   << std::endl;
                ofile << "//#pragma HLS dependence variable=internal_r type=inter false"   << std::endl;
                ofile << "    lwht_in_a   = internal_l[cnt_a];"   << std::endl;
                ofile << "    lwht_in_b   = internal_r[cnt_b];"   << std::endl;
                ofile << "    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);"   << std::endl;
                ofile << "    internal_l[cnt_c] = memo_in_a;"   << std::endl;
                ofile << "    internal_r[cnt_c] = memo_in_a;"   << std::endl;
                ofile << "    cnt_c += 1; cnt_a += 1; cnt_b += 1;" << std::endl;
                ofile << "  }"   << std::endl;
                ofile << std::endl;
                //
                //
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //
        // ON TRAITE L'ARC DESCENDANT A GAUCHE DU NOEUD
        //
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        int next_elmnt;
        if (is_rate0_after_f) {
            if (n == 1) {
                if (verbose) {
                    indentation(level);
                    printf("> Leaf rate-0 node found (%d)\n", n);
                    indentation(level);
                    printf("\e[1;31m LEAF_RATE_0 (Internal(%d) Symbols(%d) Size(%d )) \e[0m\n", p_llrs + n, curr_frozen,
                           n);
                }
                //ofile << "\t" << "leaf_node_rate_0<gf_size>(decoded + " << curr_frozen << ", symbols + " << curr_frozen << ");" << std::endl;
                //
                ofile << "  // leaf_node_rate_0" << std::endl;
                ofile << "  symbols  [cnt_u] = 0;"   << std::endl;
                ofile << "  decoded  [cnt_u] = 0;"   << std::endl;
                ofile << "  decoded_r[cnt_u] = 0;"   << std::endl;
                ofile << "  cnt_u += 1;"   << std::endl;
                ofile << std::endl;
                //
                //
                array[curr_elmnt] = LEAF_RATE_0;
            } else {
                if (verbose) {
                    indentation(level);
                    printf("> Rate-0 node found (%d)\n", n);
                    indentation(level);
                    printf("\e[1;31m RATE_0 (Internal(%d) Symbols(%d) Size(%d )) \e[0m\n", p_llrs + n, curr_frozen, n);
                }
                array[curr_elmnt] = RATE_0;
                //ofile << "  middle_node_pruned_rate_0<gf_size>(decoded + " << curr_frozen << ", symbols + " << curr_frozen << ", " << n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // middle_node_pruned_rate_0 [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  //loop_f0_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {" << std::endl;
                ofile << "  //#pragma HLS PIPELINE"   << std::endl;
                ofile << "  //  symbols  [cnt_u] = 0;"   << std::endl;
                ofile << "  //  decoded  [cnt_u] = 0;"   << std::endl;
                ofile << "  //  decoded_r[cnt_u] = 0;"   << std::endl;
                ofile << "  //  cnt_u += 1; "   << std::endl;
                ofile << "  //}"   << std::endl;
                ofile << std::endl;
                //
                //
            }
            next_elmnt = curr_elmnt + 1;
            //
            // RATE 1
            //
        } else if (is_rate1_after_f) {
            if (n == 1) {
                array[curr_elmnt] = LEAF_RATE_1_FROM_F;
                if (verbose) {
                    indentation(level);
                    printf("> Leaf rate-1 node found (%d)\n", n);
                    indentation(level);
                    printf("\e[1;31m LEAF_RATE_1_FROM_F (Internal(%d) Symbols(%d) Size(%d )) \e[0m\n", p_llrs + n, curr_frozen, n);
                }
                //ofile << "\t" << "leaf_node_after_f<gf_size>(internal + " << p_llrs << ", p_llrs + size + " << curr_frozen << ", symbols + " << curr_frozen << ");" << std::endl;
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // leaf_node_after_f [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  cnt_a = " << (p_llrs + size) << ";"   << std::endl;
                ofile << "  symbol_v = /*FWHT + */ vec_decision( internal[cnt_a/*s + " << (p_llrs + size) << "*/], true );" << std::endl;
                ofile << "  symbols  [cnt_u] = symbol_v;"                                       << std::endl;
                ofile << "  decoded  [cnt_u] = symbol_v;"   << std::endl;
                ofile << "  decoded_r[cnt_u] = symbol_v;"   << std::endl;
                ofile << "  cnt_a += 1;"   << std::endl;
                ofile << "  cnt_u += 1;"   << std::endl;
                ofile  << std::endl;

            } else {
                array[curr_elmnt] = RATE_1_FROM_F;
                if (verbose) {
                    indentation(level);
                    printf("> Rate-1 node found (%d)\n", n);
                    indentation(level);
                    printf("\e[1;31m RATE_1_FROM_F (Internal(%d) Symbols(%d) Size(%d )) \e[0m\n", p_llrs + n, curr_frozen, n);
                }
                //ofile << "\t" << "middle_node_pruned_rate_1_after_f<gf_size>(internal + " << p_llrs + size << ", decoded + " << curr_frozen << ", symbols + " << curr_frozen << ", " << n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // middle_node_pruned_rate_1_after_g [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  cnt_a = " << (p_llrs + size) << ";" << std::endl;
                ofile << "	loop_r1_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"                         << std::endl;
                ofile << "#pragma HLS PIPELINE"                                                              << std::endl;
                ofile << "    	symbol_v = vec_decision( internal_l[cnt_a/*s + " << (p_llrs + size) << "*/], true );" << std::endl;
                ofile << "    	symbols  [cnt_u] = e;"                               << std::endl;
                ofile << "      decoded  [cnt_u] = e;"   << std::endl;
                ofile << "      decoded_r[cnt_u] = e;"   << std::endl;
                ofile << "      cnt_a += 1;"   << std::endl;
                ofile << "      cnt_u += 1;"   << std::endl;
                ofile << "  }"                                                                               << std::endl;
                ofile << std::endl;
            }
            next_elmnt = curr_elmnt + 1;
            //
            // REPETITION node
            //
        } else if (is_rep_after_f) {
            array[curr_elmnt] = REP_FROM_F;
            next_elmnt        = curr_elmnt + 1;
            if (verbose) {
                indentation(level);
                printf("> Leaf REP_F node found (%d)\n", n);
                indentation(level);
                printf("\e[1;31m REP_FROM_F (Internal(%d) Symbols(%d) Size(%d )) \e[0m\n", p_llrs + n, curr_frozen, n);
            }
            if (level == 1) {
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // middle_node_pruned_rep_after_f [hls_generator:" << __LINE__ << "]" << std::endl;
                //
                int n_2   = (n/2);
                int src_1 = 0;
                int src_2 = (n/2);
                int dst   = n;
                int lastdst = dst;
                bool first = true;
                while ( n_2 != 1 ) {
                    ofile << "  cnt_c = " << dst << "; cnt_a = " <<  src_1    << "; cnt_b = " << src_2 << ";" << std::endl;
                    ofile << "  loop_rep_" << (loop_id++) << " : for (s = 0; s < " << (n_2) << "; s += 1) {"     << std::endl;
                    ofile << "#pragma HLS PIPELINE"                                                              << std::endl;
                    ofile << "//#pragma HLS dependence variable=internal_l type=inter false"                                                              << std::endl;
                    ofile << "//#pragma HLS dependence variable=internal_r type=inter false"                                                              << std::endl;
                    ofile << "    lwht_in_a   = internal_l[cnt_a];"   << std::endl;
                    ofile << "    lwht_in_b   = internal_r[cnt_b];"   << std::endl;
                    ofile << "    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, " << (first?"true":"false") << ");"   << std::endl;
                    ofile << "    internal_l[cnt_c] = memo_in_a;"   << std::endl;
                    ofile << "    internal_r[cnt_c] = memo_in_a;"   << std::endl;
                    ofile << "    cnt_c += 1; cnt_a += 1; cnt_b += 1;"   << std::endl;
                    ofile << "  }"                                                                               << std::endl;
                    src_1 += 2 * n_2;
                    src_2  = src_1 + (n_2/2);
                    lastdst = dst;
                    dst   += n_2;
                    n_2 /= 2;
                    first = false;
                }
                ofile << "  cnt_c = " << (lastdst + 2) << "; cnt_a = " << lastdst << "; cnt_b = " << (lastdst + 1) << ";"   << std::endl;
                ofile << "  lwht_in_a   = internal_l[cnt_a];"   << std::endl;
                ofile << "  lwht_in_b   = internal_r[cnt_b];"   << std::endl;
                ofile << "  memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);"          << std::endl;
                ofile << "  internal_l[cnt_c] = memo_in_a;"             << std::endl;
                ofile << "  internal_r[cnt_c] = memo_in_a;"             << std::endl;
                ofile << "  cnt_a = cnt_c;" << std::endl;
                ofile << "  symbol_v = vec_decision( internal_l[cnt_a], false );" << std::endl;
                ofile << "  loop_rep_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"  << std::endl;
                ofile << "    symbols_l[cnt_rw] = symbol_v;" << std::endl;
                ofile << "    symbols_r[cnt_rw] = symbol_v;" << std::endl;
                ofile << "    cnt_rw += 1;"                << std::endl;
                ofile << "    decoded  [cnt_u] = (s == " << (n-1) << ") ?  symbol_v : (ap_uint<log2_gf_size>)0;" << std::endl;
                ofile << "    cnt_u  += 1;"                << std::endl;
                ofile << "  }"                            << std::endl;
                ofile << std::endl;
            }else if ( n == 2 ) {
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // middle_node_pruned_rep_after_f [hls_generator:" << __LINE__ << "]" << std::endl;
                //
                int src_1 = (p_llrs + size);
                int src_2 = (p_llrs + size + n/2);
                int dst   = (p_llrs + size + n);
                ofile << "  cnt_c = " << dst << "; cnt_a = " << src_1 << "; cnt_b = " << src_2 << ";"   << std::endl;
                ofile << "  lwht_in_a   = internal_l[cnt_a];"   << std::endl;
                ofile << "  lwht_in_b   = internal_r[cnt_b];"   << std::endl;
                ofile << "  memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);"          << std::endl;
                ofile << "  internal_l[cnt_c] = memo_in_a;"             << std::endl;
                ofile << "  internal_r[cnt_c] = memo_in_a;"             << std::endl;
                ofile << "  cnt_a = cnt_c;" << std::endl;
                ofile << "  symbol_v = vec_decision( internal_l[cnt_a], false );" << std::endl;
                ofile << "  loop_rep_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"  << std::endl;
                ofile << "    symbols_l[cnt_rw] = symbol_v;" << std::endl;
                ofile << "    symbols_r[cnt_rw] = symbol_v;" << std::endl;
                ofile << "    cnt_rw += 1;"                << std::endl;
                ofile << "    decoded  [cnt_u] = (s == " << (n-1) << ") ?  symbol_v : (ap_uint<log2_gf_size>)0;" << std::endl;
                ofile << "    cnt_u  += 1;"                << std::endl;
                ofile << "  }"                            << std::endl;
                ofile << std::endl;
            }
            else {
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // middle_node_pruned_rep_after_f [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                int n_2 = (n/2);
                int src_1 = (p_llrs + size);
                int src_2 = (p_llrs + size + n/2);
                int dst   = (p_llrs + size + n);
                int lastdst = dst;
                bool first = true;
                while ( n_2 != 1 ) {
                    ofile << "  cnt_c = " << dst << "; cnt_a = " <<  src_1    << "; cnt_b = " << src_2 << ";" << std::endl;
                    ofile << "  loop_rep_" << (loop_id++) << " : for (s = 0; s < " << (n_2) << "; s += 1) {"     << std::endl;
                    ofile << "#pragma HLS PIPELINE"                                                              << std::endl;
                    ofile << "//#pragma HLS dependence variable=internal_l type=inter false"                                                              << std::endl;
                    ofile << "//#pragma HLS dependence variable=internal_r type=inter false"                                                              << std::endl;
                    ofile << "    lwht_in_a   = internal_l[cnt_a];"   << std::endl;
                    ofile << "    lwht_in_b   = internal_r[cnt_b];"   << std::endl;
                    ofile << "    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, " << (first?"true":"false") << ");"   << std::endl;
                    ofile << "    internal_l[cnt_c] = memo_in_a;"   << std::endl;
                    ofile << "    internal_r[cnt_c] = memo_in_a;"   << std::endl;
                    ofile << "    cnt_c += 1; cnt_a += 1; cnt_b += 1;"   << std::endl;
                    ofile << "  }"                                                                               << std::endl;
                    src_1 += 2 * n_2;
                    src_2  = src_1 + (n_2/2);
                    lastdst = dst;
                    dst   += n_2;
                    n_2 /= 2;
                    first = false;
                }
                ofile << "  cnt_c = " << (lastdst + 2) << "; cnt_a = " << lastdst << "; cnt_b = " << (lastdst + 1) << ";"   << std::endl;
                ofile << "  lwht_in_a   = internal_l[cnt_a];"   << std::endl;
                ofile << "  lwht_in_b   = internal_r[cnt_b];"   << std::endl;
                ofile << "  memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);"          << std::endl;
                ofile << "  internal_l[cnt_c] = memo_in_a;"             << std::endl;
                ofile << "  internal_r[cnt_c] = memo_in_a;"             << std::endl;
                ofile << "  cnt_a = cnt_c;" << std::endl;
                ofile << "  symbol_v = vec_decision( internal_l[cnt_a], false );" << std::endl;
                ofile << "  loop_rep_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"  << std::endl;
                ofile << "    symbols_l[cnt_rw] = symbol_v;" << std::endl;
                ofile << "    symbols_r[cnt_rw] = symbol_v;" << std::endl;
                ofile << "    cnt_rw += 1;"                << std::endl;
                ofile << "    decoded  [cnt_u] = (s == " << (n-1) << ") ?  symbol_v : (ap_uint<log2_gf_size>)0;" << std::endl;
                ofile << "    cnt_u  += 1;"                << std::endl;
                ofile << "  }"                            << std::endl;
                ofile << std::endl;
            }
            //
            // REPETITION node
            //
        } else if (is_spc_after_f) {
            array[curr_elmnt] = SPC_FROM_F;
            next_elmnt        = curr_elmnt + 1;
            if (verbose) {
                indentation(level);
                printf("> Leaf SPC_FROM_F node found (%d)\n", n);
                indentation(level);
                printf("\e[1;31m SPC_FROM_F (Internal(%d) Symbols(%d) Size(%d )) \e[0m\n", p_llrs + n, curr_frozen, n);
            }
            if (level == 1) {
                //ofile << "\t" << "middle_node_pruned_spc_after_f<gf_size>(internal + " << p_llrs << ", decoded + " << curr_frozen << ", symbols + " << curr_frozen << ", " << n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // middle_node_pruned_spc_after_f [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  cnt_a = " << p_llrs << ";" << std::endl;
                ofile << "	loop_spc_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"            << std::endl;
                ofile << "#pragma HLS PIPELINE"                                                              << std::endl;
                ofile << "    	symbol_v = vec_decision( internal_l[cnt_a], true );"          << std::endl;
                ofile << "    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);"                    << std::endl;
                ofile << "    	symbols  [cnt_u] = symbol_v;" << std::endl;
                ofile << "      decoded  [cnt_u] = symbol_v;" << std::endl;
                ofile << "      decoded_r[cnt_u] = symbol_v;" << std::endl;
                ofile << "      cnt_a += 1;"                << std::endl;
                ofile << "      cnt_u += 1;"                << std::endl;
                ofile << "  }"                              << std::endl;
                ofile << "  // SPC processing !!!"          << std::endl;
                ofile << "  //if ( symbol_x != 0 ) {"       << std::endl;
                ofile << "  //    printf(\"We have a SPC decoding error ;-)\\n\");" << std::endl;
                ofile << "  //}"                            << std::endl;
                ofile << "  // SPC processing !!!"          << std::endl;
                ofile << "  }"                              << std::endl;
//              ofile << "  local_remove_xors<" << n << ">(decoded, decoded_r, cnt_u - " << n << ");" << std::endl;
                ofile << "  v64_xor_processor(xor_proc_o, xor_proc_i, " << (n-1) << ");" << std::endl;
                ofile << std::endl;
            }
            else {
                //ofile << "\t" << "middle_node_pruned_spc_after_f<gf_size>(internal + " << p_llrs + size << ", decoded + " << curr_frozen << ", symbols + " << curr_frozen << ", " << n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // middle_node_pruned_spc_after_f [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  cnt_a = " << (p_llrs + size) << ";" << std::endl;
                ofile << "	loop_spc_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"            << std::endl;
                ofile << "#pragma HLS PIPELINE"                                                              << std::endl;
                ofile << "    	symbol_v = vec_decision( internal_l[cnt_a], true );"   << std::endl;
                ofile << "    	symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);"                    << std::endl;
                ofile << "    	symbols  [cnt_u] = symbol_v;" << std::endl;
                ofile << "      decoded  [cnt_u] = symbol_v;" << std::endl;
                ofile << "      decoded_r[cnt_u] = symbol_v;" << std::endl;
                ofile << "      cnt_a += 1;"                << std::endl;
                ofile << "      cnt_u += 1;"                << std::endl;
                ofile << "  }"                              << std::endl;
                ofile << "  // SPC processing !!!"          << std::endl;
                ofile << "  //if ( symbol_x != 0 ) {"       << std::endl;
                ofile << "  //    printf(\"We have a SPC decoding error ;-)\\n\");" << std::endl;
                ofile << "  //}"                            << std::endl;
                ofile << "  // SPC processing !!!"          << std::endl;
                ofile << "  }"                              << std::endl;
//              ofile << "  local_remove_xors<" << n << ">(decoded, decoded_r, cnt_u - " << n << ");" << std::endl;
                ofile << "  v64_xor_processor(xor_proc_o, xor_proc_i, " << (n-1) << ");" << std::endl;
                ofile << std::endl;
            }
            //
            // NORMAL node
            //
        } else {
            array[curr_elmnt] = MID_NODE_FROM_F;
            if (verbose) {
                indentation(level);
                printf("> Normal (f) node found (%d)\n", n);
            }
            if (level == 1) {
                // au premier niveau, on doit transmettre (internal = 0) car on a utilise le channel
                next_elmnt = execute(frozen, curr_frozen, array, curr_elmnt + 1, n, p_llrs, level + 1, true);
            } else {
                next_elmnt = execute(frozen, curr_frozen, array, curr_elmnt + 1, n, p_llrs + size, level + 1, true);
            }
        }

        //
        // Analyse de la branche droite
        //
        if (verbose) {
            indentation(level);
            printf("G edge : ");
            for (int i = 0; i < n; i++) {
                printf("%d", frozen[curr_frozen + n + i]);
            }
            printf("\n");
            printf("after_g [%d %d %d %d]\n", is_rate0_after_g, is_rate1_after_g, is_rep_after_g, is_spc_after_g);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //
        // ON TRAITE L'ARC DESCENDANT A DROITE DU NOEUD
        //
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (is_rate0_after_f) {
            //
            // SIMPLIFICATION DU NOEUD G SI ON A EU UN RATE 0 APRES G !
            //
            if (pred_is_f) {
                if (verbose) {
                    indentation(level);
                    printf("\e[1;31m Internal[%d...%d] <= SPECIAL Internal(%d...%d) (g) Internal(%d...%d) \e[0m\n", p_llrs + size, p_llrs + size + n, p_llrs, p_llrs + n, p_llrs + n, p_llrs + n + n);
                }
                //
              //ofile << "\t" << "g_function_freq_in_after_rate_0<gf_size>(internal + " << p_llrs + size << ", internal + " << p_llrs << ", internal + " << p_llrs + n << ", " << n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // g_function_freq_in_after_rate_0 [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  cnt_c = " << (p_llrs + size) << "; cnt_a = " <<  p_llrs    << "; cnt_b = " << (p_llrs+n) << ";" << std::endl;
                ofile << "  loop_g0_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"                         << std::endl;
                ofile << "#pragma HLS PIPELINE"                                                      << std::endl;
                ofile << "//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false" << std::endl;
                ofile << "//#pragma HLS dependence class=array direction=RAW variable=internal_r type=inter distance=1 false" << std::endl;
                ofile << "    symbols_l[cnt_rw] = 0;" << std::endl;
                ofile << "    symbols_r[cnt_rw] = 0;" << std::endl;
                ofile << "    cnt_rw += 1;"           << std::endl;
                ofile << "    decoded  [cnt_u ] = 0;" << std::endl;
                ofile << "    cnt_u  += 1;"           << std::endl;
                ofile << "    lwht_in_a   = internal_l[cnt_a];" << std::endl;
                ofile << "    lwht_in_b   = internal_r[cnt_b];" << std::endl;
                ofile << "    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, true);" << std::endl;
                ofile << "    internal_l[cnt_c] = memo_in_a;"                         << std::endl;
                ofile << "    internal_r[cnt_c] = memo_in_a;"                         << std::endl;
                ofile << "    cnt_c += 1; // dst"     << std::endl;
                ofile << "    cnt_a += 1; // src 1"   << std::endl;
                ofile << "    cnt_b += 1; // src 2"   << std::endl;
                ofile << "  }"                        << std::endl;
                ofile << std::endl;
                //
                //
            } else {
                if (verbose) {
                    indentation(level);
                    printf("\e[1;31m Internal[%d...%d] <= SPECIAL Internal(%d...%d) (g) Internal(%d...%d) \e[0m\n", p_llrs + size, p_llrs + size + n, p_llrs, p_llrs + n, p_llrs + n, p_llrs + n + n);
                }
                if (level == 1) {
                    //
                  //ofile << "\t" << "g_function_proba_in_after_rate_0<gf_size>(internal, channel, channel + " << n << ", " << n << ");" << std::endl;
                    //
                    ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                    ofile << "  //" << std::endl;
                    ofile << "  // g_function_proba_in_after_rate_0 [hls_generator:" << __LINE__ << "]" << std::endl;
                    ofile << "  //" << std::endl;
                    ofile << "  cnt_c = " << 0 << "; cnt_a = " <<  0    << "; cnt_b = " << n << ";" << std::endl;
                    ofile << "  loop_g0_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {" << std::endl;
                    ofile << "#pragma HLS PIPELINE"                                                      << std::endl;
                    ofile << "    symbols_l[cnt_rw] = 0;" << std::endl;
                    ofile << "    symbols_r[cnt_rw] = 0;" << std::endl;
                    ofile << "    cnt_rw += 1;"           << std::endl;
                    ofile << "    decoded  [cnt_u ] = 0;" << std::endl;
                    ofile << "    cnt_u  += 1;"           << std::endl;
                    ofile << "    lwht_in_a   = channel[cnt_a];"   << std::endl;
                    ofile << "    lwht_in_b   = channel[cnt_b];"   << std::endl;
                    ofile << "    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);"   << std::endl;
                    ofile << "    internal_l[cnt_c] = memo_in_a;"   << std::endl;
                    ofile << "    internal_r[cnt_c] = memo_in_a;"   << std::endl;
                    ofile << "    cnt_c += 1; cnt_a += 1; cnt_b += 1;" << std::endl;
                    ofile << "  }"   << std::endl;
                    ofile << std::endl;
                    //
                    //
                }
                else {
                    //
                  //ofile << "\t" << "g_function_proba_in_after_rate_0<gf_size>(internal + " << p_llrs + size << ", internal + " << p_llrs << ", internal + " << p_llrs + n << ", " << n << ");" << std::endl;
                    //
                    ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                    ofile << "  //" << std::endl;
                    ofile << "  // g_function_proba_in_after_rate_0 [hls_generator:" << __LINE__ << "]" << std::endl;
                    ofile << "  //" << std::endl;
                    ofile << "  cnt_c = " << (p_llrs + size) << "; cnt_a = " <<  p_llrs    << "; cnt_b = " << (p_llrs+n) << ";" << std::endl;
                    ofile << "  loop_g0_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"                         << std::endl;
                    ofile << "#pragma HLS PIPELINE"                 << std::endl;
                    ofile << "//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false" << std::endl;
                    ofile << "//#pragma HLS dependence class=array direction=RAW variable=internal_r type=inter distance=1 false" << std::endl;
                    ofile << "    symbols_l[cnt_rw] = 0;"           << std::endl;
                    ofile << "    symbols_r[cnt_rw] = 0;"           << std::endl;
                    ofile << "    cnt_rw += 1;"                     << std::endl;
                    ofile << "    decoded  [cnt_u ] = 0;"           << std::endl;
                    ofile << "    cnt_u  += 1;"                     << std::endl;
                    ofile << "    lwht_in_a   = internal_l[cnt_a];" << std::endl;
                    ofile << "    lwht_in_b   = internal_r[cnt_b];" << std::endl;
                    ofile << "    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);"  << std::endl;
                    ofile << "    internal_l[cnt_c] = memo_in_a;"      << std::endl;
                    ofile << "    internal_r[cnt_c] = memo_in_a;"      << std::endl;
                    ofile << "    cnt_c += 1; // dst"   << std::endl;
                    ofile << "    cnt_a += 1; // src 1" << std::endl;
                    ofile << "    cnt_b += 1; // src 2" << std::endl;
                    ofile << "  }"                                                                       << std::endl;
                    ofile << std::endl;
                    //
                    //
                }
            }
            //
            // NORMAL node
            //
        } else if (level == 1) {
            if (verbose) {
                indentation(level);
                printf("\e[1;31m Internal[%d...%d] <= Channel(%d...%d) (g) Channel(%d...%d) \e[0m\n", 0, n, 0, n, n, n + n);
            }
            //
          //ofile << "\t" << "g_function_proba_in<gf_size>(internal, channel, channel + " << n << ", symbols, " << n << ");" << std::endl;
            //
            ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
            ofile << "  //" << std::endl;
            ofile << "  // g_function_proba_in [hls_generator:" << __LINE__ << "]" << std::endl;
            ofile << "  //" << std::endl;
            ofile << "  cnt_c = " << 0 << "; cnt_a = " << 0 << "; cnt_b = " << n << "; cnt_rd = 0;" << std::endl;
            ofile << "  loop_g_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {" << std::endl;
            ofile << "#pragma HLS PIPELINE"   << std::endl;
            ofile << "//#pragma HLS dependence variable=internal_l type=inter false"   << std::endl;
            ofile << "//#pragma HLS dependence variable=internal_r type=inter false"   << std::endl;
            ofile << "    lwht_in_a   = channel[cnt_a];"   << std::endl;
            ofile << "    lwht_in_b   = channel[cnt_b];"   << std::endl;
            ofile << "    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], false);"   << std::endl;
            ofile << "    internal_l[cnt_c] = memo_in_a;"   << std::endl;
            ofile << "    internal_r[cnt_c] = memo_in_a;"   << std::endl;
            ofile << "    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;" << std::endl;
            ofile << "  }"   << std::endl;
            ofile << std::endl;
            //
            //
        } else {
            if (is_rate0_after_g) {
                //
                // We do not need to perform the computation as their results won't be used !
                //
            } else if (pred_is_f) {
                if (verbose) {
                    indentation(level);
                    printf("\e[1;31m Internal[%d...%d] <= Internal(%d...%d) (g) Internal(%d...%d) \e[0m\n", p_llrs + size, p_llrs + size + n, p_llrs, p_llrs + n, p_llrs + n, p_llrs + n + n);
                }
                //
              //ofile << "\t" << "g_function_freq_in<gf_size>(internal + " << p_llrs + size << ", internal + " << p_llrs << ", internal + " << p_llrs + n << ", symbols + " << curr_frozen << ", " << n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // g_function_freq_in [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  cnt_c = " << (p_llrs + size) << "; cnt_a = " <<  p_llrs    << "; cnt_b = " << (p_llrs+n) << "; cnt_rd = " << curr_frozen << ";" << std::endl;
                ofile << "  loop_g_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"                         << std::endl;
                ofile << "#pragma HLS PIPELINE"                 << std::endl;
                ofile << "//#pragma HLS dependence variable=internal_l type=inter false" << std::endl;
                ofile << "//#pragma HLS dependence variable=internal_r type=inter false" << std::endl;
                ofile << "    lwht_in_a   = internal_l[cnt_a];" << std::endl;
                ofile << "    lwht_in_b   = internal_r[cnt_b];" << std::endl;
                ofile << "    memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], true);"   << std::endl;
                ofile << "    internal_l[cnt_c] = memo_in_a;"   << std::endl;
                ofile << "    internal_r[cnt_c] = memo_in_a;"   << std::endl;
                ofile << "    cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;" << std::endl;
                ofile << "  }"                                                                       << std::endl;
                ofile << std::endl;
                //
                //
            } else {
                if (verbose) {
                    indentation(level);
                    printf("\e[1;31m Internal[%d...%d] <= Internal(%d...%d) (g) Internal(%d...%d) \e[0m\n", p_llrs + size, p_llrs + size + n, p_llrs, p_llrs + n, p_llrs + n, p_llrs + n + n);
                }
                //
              //ofile << "\t" << "g_function_proba_in<gf_size>(internal + " << p_llrs + size << ", internal + " << p_llrs << ", internal + " << p_llrs + n << ", symbols + " << curr_frozen << ", " << n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // g_function_proba_in [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  cnt_c = " << (p_llrs + size) << "; cnt_a = " <<  p_llrs    << "; cnt_b = " << (p_llrs+n) << "; cnt_rd = " << curr_frozen << ";" << std::endl;
                ofile << "	loop_g_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"                         << std::endl;
                ofile << "#pragma HLS PIPELINE"                   << std::endl;
                ofile << "//#pragma HLS dependence class=array direction=RAW variable=internal_l type=inter distance=1 false" << std::endl;
                ofile << "//#pragma HLS dependence class=array direction=RAW variable=internal_r type=inter distance=1 false" << std::endl;
                ofile << "    	lwht_in_a   = internal_l[cnt_a];" << std::endl;
                ofile << "    	lwht_in_b   = internal_r[cnt_b];" << std::endl;
                ofile << "    	memo_in_a   = datapath(lwht_in_a, lwht_in_b, symbols_l[cnt_rd], false);"  << std::endl;
                ofile << "    	internal_l[cnt_c] = memo_in_a;"   << std::endl;
                ofile << "    	internal_r[cnt_c] = memo_in_a;"   << std::endl;
                ofile << "      cnt_c += 1; cnt_a += 1; cnt_b += 1; cnt_rd += 1;" << std::endl;
                ofile << "  }"                                    << std::endl;
                ofile << std::endl;
                //
                //
            }
        }

        //
        //
        //
        int final_offset;
        if (is_rate0_after_g) {
            if (n == 1) {
                indentation(level);
                printf("> Leaf rate-0 node found (size = %d)\n", n);
                indentation(level);
                printf("\e[1;31m LEAF_RATE_0 (Internal(%d) Symbols(%d) Size(%d )) \e[0m\n", p_llrs + n, curr_frozen + n, n);
                ofile << "\t" << "leaf_node_rate_0<gf_size>(decoded + " << curr_frozen << ", symbols + " << curr_frozen + n << ");" << std::endl;
                array[next_elmnt] = LEAF_RATE_0;
            } else {
                indentation(level);
                printf("> Rate-0 node found (size = %d)\n", n);
                indentation(level);
                printf("\e[1;31m RATE_0 (Internal(%d) Symbols(%d) Size(%d )) \e[0m\n", p_llrs + n, curr_frozen + n, n);
                ofile << "\t" << "middle_node_pruned_rate_0<gf_size>(decoded + " << curr_frozen + n << ", symbols + " << curr_frozen + n << ", " << n << ");" << std::endl;
                ofile << std::endl;
                array[next_elmnt] = RATE_0;
            }
            final_offset = next_elmnt + 1;
            //
            //
            //
        } else if (is_rate1_after_g) {
            if (n == 1) {
                indentation(level);
                printf("> Leaf rate-1 node found (size = %d)\n", n);
                indentation(level);
                array[next_elmnt] = LEAF_RATE_1_FROM_G;
                printf("\e[1;31m LEAF_RATE_1_FROM_G (Internal(%d) Symbols(%d) Size(%d )) \e[0m\n", p_llrs + n, curr_frozen, n);
                //ofile << "\t" << "leaf_node_after_g<gf_size>(internal + " << p_llrs + size << ", decoded + " << curr_frozen + n << ", symbols + " << curr_frozen + n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // leaf_node_after_g [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  cnt_a = " << (p_llrs + size) << ";" << std::endl;
                ofile << "  symbol_v = vec_decision( internal_l[cnt_a], false );" << std::endl;
                ofile << "  symbols  [cnt_u] = symbol_v;" << std::endl;
                ofile << "  decoded  [cnt_u] = symbol_v;" << std::endl;
                ofile << "  decoded_r[cnt_u] = symbol_v;" << std::endl;
                ofile << "  cnt_a += 1;"                << std::endl;
                ofile << "  cnt_u += 1;"                << std::endl;
                ofile  << std::endl;
                //
                //
            } else if (level != 1) {
                array[next_elmnt] = RATE_1_FROM_G;
                if (verbose) {
                    indentation(level);
                    printf("> Rate-1 node found (size = %d)\n", n);
                    indentation(level);
                    printf("\e[1;31m RATE_1_FROM_G (Internal(%d) Symbols(%d) Size(%d )) \e[0m\n", p_llrs + n, curr_frozen, n);
                }
                //ofile << "\t" << "middle_node_pruned_rate_1_after_g<gf_size>(internal + " << p_llrs + size << ", decoded + " << curr_frozen + n << ", symbols + " << curr_frozen + n << ", " << n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // middle_node_pruned_rate_1_after_g [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  cnt_a = " << (p_llrs + size) << ";"                << std::endl;
                ofile << "  loop_g_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"                         << std::endl;
                ofile << "#pragma HLS PIPELINE"                                                              << std::endl;
                ofile << "    symbol_v = vec_decision( internal_l[cnt_a], false );" << std::endl;
                ofile << "    cnt_a += 1;"                   << std::endl;
                ofile << "    symbols_l[cnt_rw] = symbol_v;" << std::endl;
                ofile << "    symbols_r[cnt_rw] = symbol_v;" << std::endl;
                ofile << "    cnt_rw += 1;"                  << std::endl;
                ofile << "    xor_proc_i[s]     = symbol_v;" << std::endl;
                ofile << "  }"                               << std::endl;
//              ofile << "  local_remove_xors<" << n << ">(decoded, decoded_r, cnt_u - " << n << ");" << std::endl;
                ofile << "  v64_xor_processor(xor_proc_o, xor_proc_i, " << (n-1) << ");" << std::endl;
                ofile << std::endl;
            } else {
                //ofile << "\t" << "middle_node_pruned_rate_1_after_g<gf_size>(internal + " << 0 << ", decoded + " << curr_frozen + n << ", symbols + " << curr_frozen + n << ", " << n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // middle_node_pruned_rate_1_after_g [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // cnt_a = 0;" << std::endl;
                ofile << "	loop_g_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"                         << std::endl;
                ofile << "#pragma HLS PIPELINE"                                                              << std::endl;
                ofile << "    	symbol_v = vec_decision( internal_l[cnt_a/*s + " << 0 << "*/], false );" << std::endl;
                ofile << "    	symbols  [cnt_u] = symbol_v;"                               << std::endl;
                ofile << "      decoded  [cnt_u] = symbol_v;"   << std::endl;
                ofile << "      decoded_r[cnt_u] = symbol_v;"   << std::endl;
                ofile << "      cnt_u += 1;"   << std::endl;
                ofile << "      cnt_a += 1;"   << std::endl;
                ofile << "  }"                                                                               << std::endl;
//              ofile << "  local_remove_xors<" << n << ">(decoded, decoded_r, cnt_u - " << n << ");" << std::endl;
                ofile << "  v64_xor_processor(xor_proc_o, xor_proc_i, " << (n-1) << ");" << std::endl;
                ofile << std::endl;
            }
            final_offset = next_elmnt + 1;
            //
            //
            //
        } else if (is_spc_after_g) {
            array[next_elmnt] = SPC_FROM_G;
            final_offset      = next_elmnt + 1;
            if (level == 1) {
                if (verbose) {
                    indentation(level);
                    printf("> SPC node found (size = %d)\n", n);
                    indentation(level);
                    printf("\e[1;31m SPC_FROM_G (Internal(%d) Symbols(%d) Size(%d )) \e[0m\n", p_llrs + n, curr_frozen, n);
                }
                //ofile << "  middle_node_pruned_spc_after_g<gf_size>(internal + " << p_llrs << ", decoded + " << curr_frozen + n << ", symbols + " << curr_frozen + n << ", " << n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // middle_node_pruned_spc_after_g [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  cnt_a = " <<  p_llrs << ";" << std::endl;
                ofile << "  loop_spc_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"    << std::endl;
                ofile << "#pragma HLS PIPELINE"                                                      << std::endl;
                ofile << "      symbol_v = vec_decision( internal_l[cnt_a], false );" << std::endl;
                ofile << "      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);"                       << std::endl;
                ofile << "      symbols_l[cnt_rw] = symbol_v;"   << std::endl;
                ofile << "      symbols_r[cnt_rw] = symbol_v;"   << std::endl;
                ofile << "      xor_proc_i[s]     = symbol_v;"   << std::endl;
                ofile << "      cnt_a  += 1;"   << std::endl;
                ofile << "      cnt_rw += 1;"   << std::endl;
                ofile << "  }"                                                                       << std::endl;
                ofile << "  // SPC processing !!!"          << std::endl;
                ofile << "  //if ( symbol_x != 0 ) {"       << std::endl;
                ofile << "  //    printf(\"We have a SPC decoding error ;-)\\n\");" << std::endl;
                ofile << "  //}"                            << std::endl;
                ofile << "  // SPC processing !!!"          << std::endl;
//              ofile << "  local_remove_xors<" << n << ">(decoded, decoded_r, cnt_u - " << n << ");" << std::endl;
                ofile << "  v64_xor_processor(xor_proc_o, xor_proc_i, " << (n-1) << ");" << std::endl;
                ofile << std::endl;

                ofile << "  loop_spc_" << (loop_id++) << " : for (i = 0; i < " << n << "; i += 1) {" << std::endl;
                ofile << "#pragma HLS PIPELINE" << std::endl;
                ofile << "#pragma HLS UNROLL factor=4" << std::endl;
                ofile << "    decoded[cnt_u]  = xor_proc_o[i];" << std::endl;
                ofile << "    cnt_u  += 1;" << std::endl;
                ofile << "  }" << std::endl;
                ofile << std::endl;

            } else {
                if (verbose) {
                    indentation(level);
                    printf("> SPC node found (size = %d)\n", n);
                    indentation(level);
                    printf("\e[1;31m SPC_FROM_G (Internal(%d) Symbols(%d) Size(%d )) \e[0m\n", p_llrs + n, curr_frozen, n);
                }
                //ofile << "  middle_node_pruned_spc_after_g<gf_size>(internal + " << p_llrs + size << ", decoded + " << curr_frozen + n << ", symbols + " << curr_frozen + n << ", " << n << ");" << std::endl;
                //
                ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  // middle_node_pruned_spc_after_g [hls_generator:" << __LINE__ << "]" << std::endl;
                ofile << "  //" << std::endl;
                ofile << "  cnt_a = " <<  (p_llrs + size) << ";" << std::endl;
                ofile << "  loop_spc_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"              << std::endl;
                ofile << "#pragma HLS PIPELINE"                                                                << std::endl;
                ofile << "      symbol_v = vec_decision( internal_l[cnt_a], false );" << std::endl;
                ofile << "      symbol_x = (s == 0) ? symbol_v : (symbol_x ^ symbol_v);"                       << std::endl;
                ofile << "      symbols_l[cnt_rw] = symbol_v;"   << std::endl;
                ofile << "      symbols_r[cnt_rw] = symbol_v;"   << std::endl;
                ofile << "      xor_proc_i[s]     = symbol_v;"   << std::endl;
                ofile << "      cnt_a  += 1;"   << std::endl;
                ofile << "      cnt_rw += 1;"   << std::endl;
                ofile << "  }"                                                                                 << std::endl;
                ofile << "  // SPC processing !!!"          << std::endl;
                ofile << "  //if ( symbol_x != 0 ) {"       << std::endl;
                ofile << "  //    printf(\"We have a SPC decoding error ;-)\\n\");" << std::endl;
                ofile << "  //}"                            << std::endl;
                ofile << "  // SPC processing !!!"          << std::endl;
//              ofile << "  local_remove_xors<" << n << ">(decoded, decoded_r, cnt_u - " << n << ");" << std::endl;
                ofile << "  v64_xor_processor(xor_proc_o, xor_proc_i, " << (n-1) << ");" << std::endl;
                ofile << std::endl;
            }
            //
            //
            //
        } else if (is_rep_after_g) {
            array[curr_elmnt] = REP_FROM_G;
            final_offset      = next_elmnt + 1;
            for (int z = 0; z < level; z += 1)
                printf("+  ");
            printf("> Node REP_G node found (%d)\n", n);
            indentation(level);
            printf("\e[1;31m REP_NodeSymbols (Internal(%d) Symbols(%d) Size(%d )) \e[0m\n", p_llrs + n, curr_frozen, n);
            //ofile << "\t" << "middle_node_pruned_rep_after_g<gf_size>(internal + " << p_llrs + size << ", decoded + " << curr_frozen + n << ", symbols + " << curr_frozen + n << ", " << n << ");" << std::endl;
            //
            ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
            ofile << "  //" << std::endl;
            ofile << "  // middle_node_pruned_rep_after_g [hls_generator:" << __LINE__ << "]" << std::endl;
            ofile << "  //" << std::endl;
            int n_2 = (n/2);
            int src_1 = (p_llrs + size);
            int src_2 = (p_llrs + size + n/2);
            int dst   = (p_llrs + size + n);
            int lastdst = dst;
            bool first = true;
            while ( n_2 != 1 ) {
                ofile << "  cnt_c = " << dst << "; cnt_a = " <<  src_1    << "; cnt_b = " << src_2 << ";" << std::endl;
                ofile << "  loop_rep_" << (loop_id++) << " : for (s = 0; s < " << (n_2) << "; s += 1) {"     << std::endl;
                ofile << "#pragma HLS PIPELINE"                                                              << std::endl;
                ofile << "//#pragma HLS dependence variable=internal_l type=inter false"                                                              << std::endl;
                ofile << "//#pragma HLS dependence variable=internal_r type=inter false"                                                              << std::endl;
                ofile << "    lwht_in_a   = internal_l[cnt_a];"   << std::endl;
                ofile << "    lwht_in_b   = internal_r[cnt_b];"   << std::endl;
                ofile << "    memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);"   << std::endl;
                ofile << "    internal_l[cnt_c] = memo_in_a;"   << std::endl;
                ofile << "    internal_r[cnt_c] = memo_in_a;"   << std::endl;
                ofile << "    cnt_c += 1; cnt_a += 1; cnt_b += 1;"   << std::endl;
                ofile << "  }"                                                                               << std::endl;
                src_1 += 2 * n_2;
                src_2  = src_1 + (n_2/2);
                lastdst = dst;
                dst   += n_2;
                n_2 /= 2;
                first = false;
            }
            ofile << "  cnt_c = " << (lastdst + 2) << "; cnt_a = " << lastdst << "; cnt_b = " << (lastdst + 1) << ";"   << std::endl;
            ofile << "  lwht_in_a   = internal_l[cnt_a];"   << std::endl;
            ofile << "  lwht_in_b   = internal_r[cnt_b];"   << std::endl;
            ofile << "  memo_in_a   = datapath(lwht_in_a, lwht_in_b, 0, false);"          << std::endl;
            ofile << "  internal_l[cnt_c] = memo_in_a;"     << std::endl;
            ofile << "  internal_r[cnt_c] = memo_in_a;"     << std::endl;
            ofile << "  cnt_a = cnt_c;" << std::endl;
            ofile << "  symbol_v = vec_decision( internal_l[cnt_a], false );" << std::endl;
            ofile << "  loop_rep_" << (loop_id++) << " : for (s = 0; s < " << n << "; s += 1) {"  << std::endl;
            ofile << "    symbols_l[cnt_rw] = symbol_v;" << std::endl;
            ofile << "    symbols_r[cnt_rw] = symbol_v;" << std::endl;
            ofile << "    cnt_rw += 1;"                << std::endl;
            ofile << "    decoded  [cnt_u] = (s == " << (n-1) << ") ?  symbol_v : (ap_uint<log2_gf_size>)0;" << std::endl;
            ofile << "    cnt_u  += 1;"                  << std::endl;
            ofile << "  }"                               << std::endl;
            ofile << std::endl;
        } else {
            array[next_elmnt] = MID_NODE_FROM_G;
            if (verbose) {
                indentation(level);
                printf(" > Normal (g) node found (size = %d)\n", n);
            }
            if (level == 1) {
                // au premier niveau, on doit transmettre (internal = 0) car on a utilise le channel
                final_offset = execute(frozen, curr_frozen + n, array, next_elmnt + 1, n, p_llrs, level + 1, false);
            } else {
                final_offset = execute(frozen, curr_frozen + n, array, next_elmnt + 1, n, p_llrs + size, level + 1, false);
            }
        }
        //
        // NORMAL nodes
        //
        if (level == 1) {
            if (verbose) {
                indentation(level);
                printf("\e[1;31m Nothing \e[0m\n");
            }
        } else {
            if (verbose) {
                indentation(level);
                printf("\e[1;31m Symbols[%d...%d] <= Symbols(%d...%d) (xor) Symbols(%d...%d) \e[0m\n", p_llrs + size, p_llrs + size + n, p_llrs, p_llrs + n, p_llrs + n, p_llrs + n + n);
            }
            ofile << "  ////////////////////////////////////////////////////////////////////////////" << std::endl;
            ofile << "  //" << std::endl;
            ofile << "  // xor procesing (level " << level << ", pred_is_f =" << pred_is_f << ") [hls_generator:" << __LINE__ << "]" << std::endl;
            ofile << "  //" << std::endl;
            ofile << "  cnt_rw = " << (curr_frozen) << ";" << std::endl;
            ofile << "  cnt_rd = " << (curr_frozen + n) << ";" << std::endl;
            ofile << "  loop_xor_" << (loop_id++) << " : for(i = 0; i < " << n << "; i += 1){" << std::endl;
            ofile << "#pragma HLS PIPELINE"                                                              << std::endl;
            ofile << "#pragma HLS UNROLL factor=4"                                                              << std::endl;
            ofile << "//#pragma HLS dependence class=array direction=RAW variable=symbols_l type=inter distance=1 false" << std::endl;
            ofile << "//#pragma HLS dependence class=array direction=RAW variable=symbols_r type=inter distance=1 false" << std::endl;
            ofile << "    symbol_v          = symbols_l[cnt_rw] ^ symbols_r[cnt_rd];" << std::endl;
            ofile << "    symbols_l[cnt_rw] = symbol_v;" << std::endl;
            ofile << "    symbols_r[cnt_rw] = symbol_v;" << std::endl;
            ofile << "    cnt_rd += 1;" << std::endl;
            ofile << "    cnt_rw += 1;" << std::endl;
            if ( is_rate1_after_g || is_spc_after_g ) {
                ofile << "    decoded  [cnt_u]  = xor_proc_o[i];" << std::endl;
                ofile << "    cnt_u  += 1;" << std::endl;
            }
            ofile << "  }" << std::endl;
            ofile << std::endl;
            //if ( is_rate1_after_g || is_spc_after_g ) {
            ofile << "  cnt_rw = cnt_u; /* synchro */" << std::endl;
            ofile << std::endl;
            //}
        }
        return final_offset;
    }
};

#endif // FROZEN_TREE_H
