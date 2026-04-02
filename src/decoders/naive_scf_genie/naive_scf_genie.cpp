/*
 * Copyright Université Rennes and Université Bretagne Sud
 * contributor(s) : Bertrand Le Gal   (2025-2026),
 *                  Abdallah Abdallah (2025-2026),
 *                  Camille  Monière  (2025-2026)
 *
 * bertrand.le-gal@univ-rennes.fr,
 * abdallah.abdallah@univ-ubs.fr,
 * camille.moniere@univ-ubs.fr
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
#include "naive_scf_genie.hpp"
#include "f_function.hpp"
#include "g_function.hpp"
#include <algorithm>
//
//
bool compareByValue(const data &a, const data &b)
{
    return a.v < b.v;
}
//
//
template <int gf_size>
inline void show(float* symb)
{
    for (int i = 0; i < gf_size; i++)
    {
        if (i == 0)
            printf("%3d :", i);
        else if (i % 8 == 0)
            printf("\n%3d :", i);
        printf("%+1.3f ", symb[i]);
    }
    printf("\n");
}
//
//
//
template <int gf_size>
naive_scf_genie<gf_size>::naive_scf_genie(const int n, const int * frozen_symb, const int L) : N(n), nFlips(L) {
    channel  = new symbols_t[N];
    internal = new symbols_t[N];
    symbols  = new uint16_t[N];
    frozen   = new uint32_t[N];
    results  = new uint16_t[N]; // les données pour le mode genie aided !
    reliab.resize(N);
    for (int i = 0; i < N; i++) {
        frozen[i] = frozen_symb[i];
    }
}
//
//
//
//
//
template <int gf_size>
naive_scf_genie<gf_size>::naive_scf_genie() : N(0), nFlips(0) {
    internal = nullptr;
    symbols  = nullptr;
    frozen   = nullptr;
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
naive_scf_genie<gf_size>::~naive_scf_genie() {
    delete[] channel;
    delete[] internal;
    delete[] symbols;
    delete[] frozen;
    delete[] results;
}

bool isCorrectlyCorrected(const uint16_t* dec, const uint16_t* ref, const int N) {
    bool is_correct = true;
    for (int i = 0; i < N; i++) {
        is_correct &= (dec[i] == ref[i]);
    }
    return is_correct;
}

template <int gf_size>
void naive_scf_genie<gf_size>::execute(void * s_channel, uint16_t * decoded) {
    //
    corr_mode    = false;
    smallest_idx = -1;
    //
    for (int i = 0; i < N; i++) {
        reliab[i].v   = 2.f;
        reliab[i].idx = i;
    }
    //
    execute_real(s_channel, decoded);
    //
    //
    if ( isCorrectlyCorrected(decoded, results, N) )
        return; // on arrete le decoding !
    //
    //
    std::sort(reliab.begin(), reliab.end(), compareByValue);
    //
    //
#if 0
    printf("\n");
    for (int i = 0; i < N; i++) {
        if ( frozen[ reliab[i].idx ] == true ) {
            //printf("--- ");
        }else if( reliab[i].v == 1.f ) {
            //printf("\e[1;33m%1.3f\e[0m ", reliab[i].v);
        }else if( decoded[ reliab[i].idx ] == results[ reliab[i].idx ] ) {
            printf("\e[1;32m%1.3f\e[0m ", reliab[i].v);
        } else {
            printf("\e[1;31m%1.3f\e[0m ", reliab[i].v);
        }
    } printf("\n");
    for (int i = 0; i < N; i++) {
        if ( frozen[ reliab[i].idx ] == true ) {
            //printf("--- ");
        }else if( reliab[i].v == 1.f ) {
        }else if( decoded[ reliab[i].idx ] == results[ reliab[i].idx ] ) {
            printf("\e[1;32m%5d\e[0m ", reliab[i].idx);
        } else {
            printf("\e[1;31m%5d\e[0m ", reliab[i].idx);
        }
    } printf("\n");
#endif
    corr_mode = true;
    for(int t = 0; t < nFlips; t += 1) {
        //
        //
        smallest_idx = reliab[t].idx;
        //
        //
        execute_real(s_channel, decoded);
        //
        //
        if ( isCorrectlyCorrected(decoded, results, N) )
            return; // on arrete le decoding !
        //
        //
    }
    //
    //
    //
    //printf("=> NOT DECODED !!!\n");
#if 0
    printf("\n");
    for (int i = 0; i < N; i++) {
        if ( frozen[ reliab[i].idx ] == true ) {
            //printf("--- ");
        }else if( i == 0 ) {
            printf("\e[1;33m%1.3f\e[0m ", reliab[i].v);
        }else if( reliab[i].v == 1.f ) {
            //printf("\e[1;33m%1.3f\e[0m ", reliab[i].v);
        }else if( decoded[ reliab[i].idx ] == results[ reliab[i].idx ] ) {
            printf("\e[1;32m%1.3f\e[0m ", reliab[i].v);
        } else {
            printf("\e[1;31m%1.3f\e[0m ", reliab[i].v);
        }
    } printf("\n");
    for (int i = 0; i < N; i++) {
        if ( frozen[ reliab[i].idx ] == true ) {
            //printf("--- ");
        }else if( i == 0 ) {
            printf("\e[1;33m%5d\e[0m ", reliab[i].idx);
        }else if( reliab[i].v == 1.f ) {
        }else if( decoded[ reliab[i].idx ] == results[ reliab[i].idx ] ) {
            printf("\e[1;32m%5d\e[0m ", reliab[i].idx);
        } else {
            printf("\e[1;31m%5d\e[0m ", reliab[i].idx);
        }
    } printf("\n");
#endif
}

template <int gf_size>
void naive_scf_genie<gf_size>::execute_real(void * s_channel, uint16_t * decoded) {
    //
    //
    //
    symbols_s<gf_size> * i_channel = static_cast<symbols_s<gf_size> *>(s_channel);
    for (int i = 0; i < N; i++) {
        channel[i] = convert_to_symbols_t(i_channel[i].value, gf_size, false);
    }
    const int n = N / 2; // Assuming size is the number of symbols
    //
    for (int i = 0; i < n; i++) {
        f_function(internal + i, channel + i, channel + n + i);
    }
    //
    middle_node(internal, internal + n, decoded, symbols, n, 0); // On descend à gauche
    //
    for (int i = 0; i < n; i++) {
        g_function(internal + i, channel + i, channel + n + i, symbols[i]);
    }
    //
    middle_node(internal, internal + n, decoded, symbols, n, n); // On descend à droite
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
void naive_scf_genie<gf_size>::middle_node(
    symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
    symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
    uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
    int         size,     // Size is the number of symbols (should be a power of 2)
    const int   symbol_id)  // Symbol ID is the index of the FIRST symbol in the symbols array
{
    const int n = size / 2; // Assuming size is the number of symbols
    //
    for (int i = 0; i < n; i++) {
        f_function(internal + i, inputs + i, inputs + n + i);
    }
    //
    if (n == 1) {
        leaf_node(internal, decoded, symbols, symbol_id);
    } else {
        middle_node(internal, internal + n, decoded, symbols, n, symbol_id);
    }
    //
    for (int i = 0; i < n; i++) {
        g_function(internal + i, inputs + i, inputs + n + i, symbols[symbol_id + i]);
    }
    //
    if (n == 1) {
        leaf_node(internal, decoded, symbols, symbol_id + n);
    } else {
        middle_node(internal, internal + n, decoded, symbols, n, symbol_id + n);
    }
    //
    for (int i = 0; i < n; i++) {
        symbols[symbol_id + i] ^= symbols[symbol_id + n + i];
    }
    //
}
template <int gf_size>
void naive_scf_genie<gf_size>::leaf_node(
    symbols_t * var,
    uint16_t *  decoded,
    uint16_t *  symbols,
    const int   symbol_id) {
    //
    // Switch from frequency to time domain if needed
    //
    if (frozen[symbol_id] == true) {
        decoded[symbol_id] = 0;
        symbols[symbol_id] = 0;
        return;
    }

    if (var->is_freq) {
        FWHT<gf_size>(var->value);
        var->is_freq = false;
    }
    normalize<gf_size>(var->value);

    //show<gf_size>(var->value);

    int max_index = argmax<gf_size>(var->value);
    float max1_v = var->value[max_index];
    var->value[max_index] = 0;
    int max_index2 = argmax<gf_size>(var->value);
    float max2_v = var->value[max_index2];

    if ( corr_mode == false ) {
        float diff = max1_v - max2_v;
        reliab[symbol_id].v = diff;
        //printf("argv1 = %d - val1 = %1.3f - argv2 = %d - val2 = %1.3f\n", max_index, max1_v, max_index2, max2_v);
        //printf("reliab[%d] = %f\n", symbol_id, max1_v);
    }

    //
    // On realise un flip !
    //
    if ( (corr_mode == true) && (smallest_idx == symbol_id) ){
        max_index = max_index2;
    }

    decoded[symbol_id]  = max_index;
    symbols[symbol_id]  = max_index;
}
//
//
//
//
//
#if (_GF_ == 8) || defined(ALL_GFs)
    template class naive_scf_genie<8>;
#endif
#if (_GF_ == 16) || defined(ALL_GFs)
    template class naive_scf_genie<16>;
#endif
#if (_GF_ == 32) || defined(ALL_GFs)
    template class naive_scf_genie<32>;
#endif
#if (_GF_ == 64) || defined(ALL_GFs)
    template class naive_scf_genie<64>;
#endif
#if (_GF_ == 128) || defined(ALL_GFs)
    template class naive_scf_genie<128>;
#endif
#if (_GF_ == 256) || defined(ALL_GFs)
    template class naive_scf_genie<256>;
#endif
#if (_GF_ == 512) || defined(ALL_GFs)
    template class naive_scf_genie<512>;
#endif
#if (_GF_ == 1024) || defined(ALL_GFs)
    template class naive_scf_genie<1024>;
#endif
#if (_GF_ == 2048) || defined(ALL_GFs)
    template class naive_scf_genie<2048>;
#endif
#if (_GF_ == 4096) || defined(ALL_GFs)
    template class naive_scf_genie<4096>;
#endif
//
//
//
//
//
