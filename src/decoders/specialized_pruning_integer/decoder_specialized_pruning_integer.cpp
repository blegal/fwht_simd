#include "decoder_specialized_pruning_integer.hpp"
//
//
//
//
#include "impl/f_function.hpp"
#include "impl/g_function.hpp"
//
#include "impl/node_leaf.hpp"
#include "impl/node_rate_0.hpp"
#include "impl/node_rate_1.hpp"
#include "impl/node_rep.hpp"
#include "impl/node_spc.hpp"
//
//
//
//
//
template <int gf_size>
decoder_specialized_pruning_integer<gf_size>::decoder_specialized_pruning_integer(const int n, const int *frozen_symb)
	: N(n), f_tree_cnt(0), f_tree(nullptr), pruned_tree(_N_)
{
	internal = new symbols_i32<gf_size>[N];
	symbols = new uint16_t[N];
	f_channel = new symbols_i32<gf_size>[N];
	f_internal = new symbols_i32<gf_size>[N];
	frozen = new int[N];

	for (int i = 0; i < N; i++)
	{
		frozen[i] = frozen_symb[i];
	}

	pruned_tree.analyze(frozen, _N_);
	pruned_tree.dump();
	f_tree = &pruned_tree; // Ici
}
//
//
//
//
//
template <int gf_size>
decoder_specialized_pruning_integer<gf_size>::decoder_specialized_pruning_integer() : N(0), f_tree_cnt(0), f_tree(nullptr), pruned_tree(_N_)
{
	internal = nullptr;
	symbols = nullptr;
	frozen = nullptr;
	printf("(EE) Error we should never be there...\n");
	printf("(EE) %s %d\n", __FILE__, __LINE__);
	exit(EXIT_FAILURE);
}
//
//
//
//
//
template <int gf_size>
decoder_specialized_pruning_integer<gf_size>::~decoder_specialized_pruning_integer()
{
	delete[] internal;
	delete[] symbols;
	delete[] frozen;
}
//
//
//
//
//
template <int gf_size>
void decoder_specialized_pruning_integer<gf_size>::execute(void *s_channel, uint16_t *decoded)
{
	symbols_s<gf_size> *channel = static_cast<symbols_s<gf_size> *>(s_channel);
	const int n = N / 2; // Assuming size is the number of symbols
	//
	//
	//
	for (int i = 0; i < N; i++)
	{
		f_channel[i] = convert_to_symbols_i32<gf_size>(channel[i]);
	}

	f_tree_cnt = 0;
	//
	f_function_proba_in<gf_size>(internal, f_channel, f_channel + n, n);
	//
	next_node left_edge = f_tree->next_node_status[f_tree_cnt++];
	if (left_edge == RATE_0)
	{
		middle_node_pruned_rate_0(internal, internal + n, decoded, symbols, n, 0); // On descend à gauche
	}
	else if (left_edge == REP_FROM_F)
	{
		middle_node_pruned_rep_after_f<gf_size>(internal, internal + n, decoded, symbols, n, 0); // On descend à gauche
	}
	else if (left_edge == SPC_FROM_F)
	{
		middle_node_pruned_spc_after_f /*<gf_size>*/ (internal, decoded, symbols, n, 0); // On descend à gauche
	}
	else if (left_edge == MID_NODE_FROM_F)
	{
		middle_node_pruned_after_f(internal, internal + n, decoded, symbols, n, 0); // On descend à gauche
	}
	else
	{
		printf("(EE) Error we should never be there...\n");
		printf("(EE) %s %d\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	//
	g_function_proba_in<gf_size>(internal, f_channel, f_channel + n, symbols, n);
	//
	next_node right_edge = f_tree->next_node_status[f_tree_cnt++];
	if (right_edge == RATE_1_FROM_G)
	{
		middle_node_pruned_rate_1_after_g(internal, internal + n, decoded, symbols, n, n); // On descend à droite
	}
	else if (right_edge == SPC_FROM_G)
	{
		middle_node_pruned_spc_after_g(internal, decoded, symbols, n, n); // On descend à droite
	}
	else if (right_edge == REP_FROM_G)
	{
		middle_node_pruned_rep_after_g<gf_size>(internal, /*internal + n,*/ decoded, symbols, n, n); // On descend à droite
	}
	else if (right_edge == MID_NODE_FROM_G)
	{
		middle_node_pruned_after_g(internal, internal + n, decoded, symbols, n, n); // On descend à droite
	}
	else
	{
		printf("(EE) Error we should never be there...\n");
		printf("(EE) %s %d\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	// No H computations as we are at the top node and we have a non systematic code !!!
}
//
//
//
//
//
template <int gf_size>
void decoder_specialized_pruning_integer<gf_size>::middle_node_pruned_after_f(
	symbols_i32<gf_size> *__restrict inputs,   // Add * and __restrict
	symbols_i32<gf_size> *__restrict internal, // Add * and __restrict
	uint16_t *__restrict decoded,
	uint16_t *__restrict symbols,
	int size,
	const int symbol_id)
{
	const int n = size / 2; // Assuming size is the number of symbols
	//
	f_function_freq_in<gf_size>(internal, inputs, inputs + n, n); // Example operation
	//
	next_node left_edge = f_tree->next_node_status[f_tree_cnt++];
	if (left_edge == RATE_0)
	{
		middle_node_pruned_rate_0(internal, internal + n, decoded, symbols, n, symbol_id);
	}
	else if (left_edge == RATE_1_FROM_F)
	{
		middle_node_pruned_rate_1_after_f(internal, internal + n, decoded, symbols, n, symbol_id);
	}
	else if (left_edge == LEAF_RATE_0)
	{
		leaf_node_rate_0<gf_size>(decoded, symbols, symbol_id);
	}
	else if (left_edge == LEAF_RATE_1_FROM_F)
	{
		leaf_node_after_f<gf_size>(internal, decoded, symbols, symbol_id, frozen[symbol_id]);
	}
	else if (left_edge == MID_NODE_FROM_F)
	{
		middle_node_pruned_after_f(internal, internal + n, decoded, symbols, n, symbol_id);
	}
	else if (left_edge == REP_FROM_F)
	{
		middle_node_pruned_rep_after_f<gf_size>(internal, internal + n, decoded, symbols, n, symbol_id);
	}
	else if (left_edge == SPC_FROM_F)
	{
		middle_node_pruned_spc_after_f(internal, decoded, symbols, n, symbol_id);
#if 0
    }else if( left_edge == LEAF_RATE_1_FROM_G) { printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__); exit( EXIT_FAILURE );
    }else if( left_edge == RATE_1_FROM_G)      { printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__); exit( EXIT_FAILURE );
    }else if( left_edge == MID_NODE_FROM_G)    { printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__); exit( EXIT_FAILURE );
#endif
	}
	else
	{
		printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	//
	g_function_freq_in<gf_size>(internal, inputs, inputs + n, symbols + symbol_id, n);
	//
	next_node right_edge = f_tree->next_node_status[f_tree_cnt++];
	if (right_edge == RATE_0)
	{
		middle_node_pruned_rate_0(internal, internal + n, decoded, symbols, n, symbol_id + n);
	}
	else if (right_edge == RATE_1_FROM_G)
	{
		middle_node_pruned_rate_1_after_g(internal, internal + n, decoded, symbols, n, symbol_id + n);
	}
	else if (right_edge == LEAF_RATE_0)
	{
		leaf_node_rate_0<gf_size>(decoded, symbols, symbol_id + n);
	}
	else if (right_edge == LEAF_RATE_1_FROM_G)
	{
		leaf_node_after_g<gf_size>(internal, decoded, symbols, symbol_id + n, frozen[symbol_id + n]);
	}
	else if (right_edge == REP_FROM_G)
	{
		middle_node_pruned_rep_after_g<gf_size>(internal, decoded, symbols, n, symbol_id + n);
	}
	else if (right_edge == SPC_FROM_G)
	{
		middle_node_pruned_spc_after_g(internal, decoded, symbols, n, symbol_id + n);
	}
	else if (right_edge == MID_NODE_FROM_G)
	{
		middle_node_pruned_after_g(internal, internal + n, decoded, symbols, n, symbol_id + n);
#if 0
    }else if( right_edge == RATE_1_FROM_F)      { printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__); exit( EXIT_FAILURE );
    }else if( right_edge == LEAF_RATE_1_FROM_F) { printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__); exit( EXIT_FAILURE );
    }else if( right_edge == MID_NODE_FROM_F)    { printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__); exit( EXIT_FAILURE );
#endif
	}
	else
	{
		printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	//
	for (int i = 0; i < n; i++)
	{
		symbols[symbol_id + i] ^= symbols[symbol_id + n + i];
	}
	//
}
//
//
//
//
//

template <int gf_size>
void decoder_specialized_pruning_integer<gf_size>::middle_node_pruned_after_g(
	symbols_i32<gf_size> *__restrict inputs,   // Inputs are the symbols from the channel (from the right)
	symbols_i32<gf_size> *__restrict internal, // Internal nodes are the symbols computed during the process (to the left)
	uint16_t *__restrict decoded,			   // Decoded symbols are the final output of the decoder (done on the left)
	uint16_t *__restrict symbols,			   // Symbols are the ones going from leafs to root (done on the left)
	int size,								   // Size is the number of symbols (should be a power of 2)
	const int symbol_id)					   // Symbol ID is the index of the FIRST symbol in the symbols array
{
	const int n = size / 2; // Assuming size is the number of symbols
	//
	f_function_proba_in<gf_size>(internal, inputs, inputs + n, n); // Example operation
	//
	next_node left_edge = f_tree->next_node_status[f_tree_cnt++];
	if (left_edge == RATE_0)
	{
		middle_node_pruned_rate_0(internal, internal + n, decoded, symbols, n, symbol_id);
	}
	else if (left_edge == RATE_1_FROM_F)
	{
		middle_node_pruned_rate_1_after_f(internal, internal + n, decoded, symbols, n, symbol_id);
	}
	else if (left_edge == LEAF_RATE_0)
	{
		leaf_node_rate_0<gf_size>(decoded, symbols, symbol_id);
	}
	else if (left_edge == LEAF_RATE_1_FROM_F)
	{
		leaf_node_after_f<gf_size>(internal, decoded, symbols, symbol_id, frozen[symbol_id]);
	}
	else if (left_edge == REP_FROM_F)
	{
		middle_node_pruned_rep_after_f<gf_size>(internal, internal + n, decoded, symbols, n, symbol_id);
	}
	else if (left_edge == SPC_FROM_F)
	{
		middle_node_pruned_spc_after_f(internal, decoded, symbols, n, symbol_id);
	}
	else if (left_edge == MID_NODE_FROM_F)
	{
		middle_node_pruned_after_f(internal, internal + n, decoded, symbols, n, symbol_id); // l'identifiant du symbole (à gauche)
	}
	else if (left_edge == RATE_1_FROM_G)
	{
		printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	else if (left_edge == LEAF_RATE_1_FROM_G)
	{
		printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	else if (left_edge == MID_NODE_FROM_G)
	{
		printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	//
	//
	//
	g_function_proba_in<gf_size>(internal, inputs, inputs + n, symbols + symbol_id, n); // Example operation
	//
	//
	//
	//  printf("-> middle_node_pruned_after_g (size = %d, f_tree_cnt = %d) - right = %s\n", size, f_tree_cnt, s_next_node[f_tree->next_node_status[f_tree_cnt]].c_str());
	next_node right_edge = f_tree->next_node_status[f_tree_cnt++];
	if (right_edge == RATE_0)
	{
		middle_node_pruned_rate_0(internal, internal + n, decoded, symbols, n, symbol_id + n);
	}
	else if (right_edge == RATE_1_FROM_G)
	{
		middle_node_pruned_rate_1_after_g(internal, internal + n, decoded, symbols, n, symbol_id + n);
	}
	else if (right_edge == LEAF_RATE_0)
	{
		leaf_node_rate_0<gf_size>(decoded, symbols, symbol_id + n);
	}
	else if (right_edge == LEAF_RATE_1_FROM_G)
	{
		leaf_node_after_g<gf_size>(internal, decoded, symbols, symbol_id + n, frozen[symbol_id + n]);
	}
	else if (right_edge == REP_FROM_G)
	{
		middle_node_pruned_rep_after_g<gf_size>(internal, decoded, symbols, n, symbol_id + n);
	}
	else if (right_edge == SPC_FROM_G)
	{
		middle_node_pruned_spc_after_g(internal, decoded, symbols, n, symbol_id + n);
	}
	else if (right_edge == MID_NODE_FROM_G)
	{
		middle_node_pruned_after_g(internal, internal + n, decoded, symbols, n, symbol_id + n);
	}
	else if (right_edge == LEAF_RATE_1_FROM_F)
	{
		printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	else if (right_edge == MID_NODE_FROM_F)
	{
		printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	else if (right_edge == RATE_1_FROM_F)
	{
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("(EE) We should never be there (%s, %d)\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	//
	//
	//
	for (int i = 0; i < n; i++)
	{
		symbols[symbol_id + i] ^= symbols[symbol_id + n + i];
	}
	//
	//
	//
}
//
//
//
//
//
#if (_GF_ == 8) || defined(ALL_GFs)
template class decoder_specialized_pruning_integer<8>;
#endif

#if (_GF_ == 16) || defined(ALL_GFs)
template class decoder_specialized_pruning_integer<16>;
#endif

#if (_GF_ == 32) || defined(ALL_GFs)
template class decoder_specialized_pruning_integer<32>;
#endif

#if (_GF_ == 64) || defined(ALL_GFs)
template class decoder_specialized_pruning_integer<64>;
#endif

#if (_GF_ == 128) || defined(ALL_GFs)
template class decoder_specialized_pruning_integer<128>;
#endif

#if (_GF_ == 256) || defined(ALL_GFs)
template class decoder_specialized_pruning_integer<256>;
#endif

#if (_GF_ == 512) || defined(ALL_GFs)
template class decoder_specialized_pruning_integer<512>;
#endif

#if (_GF_ == 1024) || defined(ALL_GFs)
template class decoder_specialized_pruning_integer<1024>;
#endif

#if (_GF_ == 2048) || defined(ALL_GFs)
template class decoder_specialized_pruning_integer<2048>;
#endif

#if (_GF_ == 4096) || defined(ALL_GFs)
template class decoder_specialized_pruning_integer<4096>;
#endif