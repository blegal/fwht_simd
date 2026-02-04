#include "decoder_naive_integer.hpp"
#include "f_function.hpp"
// #include "f_fwht.hpp"
#include "features/archi.hpp"
#include "g_function.hpp"

//
//
//
//
//
template <int gf_size>
decoder_naive_integer<gf_size>::decoder_naive_integer(const int n, const int *frozen_symb) : N(n)
{
	symbols = new uint16_t[N];
	frozen = new uint32_t[N];

	f_channel = new symbols_i32<gf_size>[N];
	f_internal = new symbols_i32<gf_size>[N];

	for (int i = 0; i < N; i++)
	{
		frozen[i] = frozen_symb[i];
	}
}
//
//
//
//
//
template <int gf_size>
decoder_naive_integer<gf_size>::decoder_naive_integer() : N(0)
{
	f_internal = nullptr;
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
decoder_naive_integer<gf_size>::~decoder_naive_integer()
{
	delete[] symbols;
	delete[] frozen;

	delete[] f_channel;
	delete[] f_internal;
}

template <int gf_size>
void decoder_naive_integer<gf_size>::execute(void *s_channel, uint16_t *decoded)
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
	//
	//
	//
	for (int i = 0; i < n; i++)
	{
		f_function<gf_size>(f_internal + i, f_channel + i, f_channel + n + i);
	}
	//
	//
	//
	middle_node(f_internal, f_internal + n, decoded, symbols, n, 0); // On descend à gauche
	//
	//
	//
	for (int i = 0; i < n; i++)
	{
		g_function<gf_size>(f_internal + i, f_channel + i, f_channel + n + i, symbols[i]);
	}
	//
	//
	//
	middle_node(f_internal, f_internal + n, decoded, symbols, n, n); // On descend à droite
																	 //
																	 //
																	 //
																	 // No H computations as we are at the top node and we have a non systematic code !!!
																	 //
																	 //
																	 //
}
//
//
//
//
//
template <int gf_size>
void decoder_naive_integer<gf_size>::middle_node(
	symbols_i32<gf_size> *inputs,	// Inputs are the symbols from the channel (from the right)
	symbols_i32<gf_size> *internal, // Internal nodes are the symbols computed during the process (to the left)
	uint16_t *decoded,				// Decoded symbols are the final output of the decoder (done on the left)
	uint16_t *symbols,				// Symbols are the ones going from leafs to root (done on the left)
	int size,						// Size is the number of symbols (should be a power of 2)
	const int symbol_id)			// Symbol ID is the index of the FIRST symbol in the symbols array
{
	const int n = size / 2; // Assuming size is the number of symbols
	//
	//
	//
	for (int i = 0; i < n; i++)
	{
		f_function<gf_size>(internal + i, inputs + i, inputs + n + i);
	}
	//
	//
	//
	if (n == 1)
	{
		leaf_node(internal, decoded, symbols, symbol_id);
	}
	else
	{
		middle_node(internal, internal + n, decoded, symbols, n, symbol_id);
	}
	//
	//
	//
	for (int i = 0; i < n; i++)
	{
		g_function<gf_size>(internal + i, inputs + i, inputs + n + i, symbols[symbol_id + i]);
	}
	//
	//
	//
	if (n == 1)
	{
		leaf_node(internal, decoded, symbols, symbol_id + n);
	}
	else
	{
		middle_node(internal, internal + n, decoded, symbols, n, symbol_id + n);
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
template <int gf_size>
void decoder_naive_integer<gf_size>::leaf_node(
	symbols_i32<gf_size> *var,
	uint16_t *decoded,
	uint16_t *symbols,
	const int symbol_id)
{
	//
	// Switch from frequency to time domain if needed
	//
	if (frozen[symbol_id] == true)
	{
		decoded[symbol_id] = 0;
		symbols[symbol_id] = 0;
		return;
	}

	if (var->is_freq)
	{
		// I32_FWHT<gf_size>(var->value);
		// var->is_freq = false;

		int32_t temp[gf_size];
		for (int i = 0; i < gf_size; i++)
		{
			temp[i] = var->value[i];
		}
		I32_FWHT<gf_size>(temp);
		LZC_normalize<gf_size, I_type::NBITS>(temp);
		const int32_t max_index = f_argmax<gf_size>(temp);
		decoded[symbol_id] = max_index;
		symbols[symbol_id] = max_index;
		return;
		// LZC_normalize<gf_size, I_type::NBITS>(temp);
		// for (int i = 0; i < gf_size; i++) {
		//     var->value[i] = (temp[i]);
		// }
		// var->is_freq = false;
	}

	// for (int i = 0; i < gf_size; i++)
	// {
	//     printf("%d : %.20f\n", i, (float)var->value[i]);
	// }
	const int32_t max_index = argmax<gf_size>(var->value);
	decoded[symbol_id] = max_index;
	symbols[symbol_id] = max_index;
}
//
//
//
//
//
#if _GF_ == 8
template class decoder_naive_integer<8>;
#elif _GF_ == 16
template class decoder_naive_integer<16>;
#elif _GF_ == 32
template class decoder_naive_integer<32>;
#elif _GF_ == 64
template class decoder_naive_integer<64>;
#elif _GF_ == 128
template class decoder_naive_integer<128>;
#elif _GF_ == 256
template class decoder_naive_integer<256>;
#elif _GF_ == 512
template class decoder_naive_integer<512>;
#elif _GF_ == 1024
template class decoder_naive_integer<1024>;
#elif _GF_ == 2048
template class decoder_naive_integer<2048>;
#elif _GF_ == 4096
template class decoder_naive_integer<4096>;
#endif
//
//
//
//
//
