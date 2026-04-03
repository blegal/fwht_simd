/*
* Copyright Université Rennes and Université Bretagne Sud
 * contributor(s) : Bertrand Le Gal,
 *                  Abdallah Abdallah,
 *                  Camille  Monière (2025-2026)
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
#pragma once
#include "decoders/decoder.hpp"
#include "definitions/custom_types.hpp"
#include "features/fwht/fwht_counter.hpp"
//
//
//
//
//
struct scf_oracle_data{
    float v;
    int idx;
};
//
//
template <int gf_size>
class naive_scf_oracle : public decoder {
public:
    naive_scf_oracle();
    naive_scf_oracle(const int n, const int * frozen_symb, const int L = 2);
    ~naive_scf_oracle();

    virtual void execute(void * channel, uint16_t * decoded);
    virtual int  GF() { return gf_size; }
    virtual void setResult(const uint16_t * decoded) {
        for (int i = 0; i < N; i++) {
            results[i] = decoded[i];
        }
    }

private:
    virtual void execute_real(void * channel, uint16_t * decoded);

    void middle_node(
        symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
        symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
        uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
        uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
        int         size,     // Size is the number of symbols (should be a power of 2)
        const int   symbol_id); // Symbol ID is the index of the FIRST symbol in the symbols array

    void leaf_node(
        symbols_t * var,
        uint16_t *  decoded,
        uint16_t *  symbols,
        const int   symbol_id);

    void g_function(
        symbols_t *__restrict dst,	 // the data to be computed for the left side of the graph
        symbols_t *__restrict src_a, // the upper value set from the right side of the graph
        symbols_t *__restrict src_b, // the lower value set from the right side of the graph
        uint32_t src_c);

    void f_function(
        symbols_t * __restrict dst,
        symbols_t * __restrict src_a,
        symbols_t * __restrict src_b);

    symbols_t * channel;
    symbols_t * internal;
    uint16_t *  symbols;
    uint32_t *  frozen;
    uint16_t *  results;  // les résultats attendus pour le mode GENIE

    const int N;
    const int nFlips;

    //
    // Debug statistics
    //
    int nFixs;
    std::vector<int> distance;
    std::vector<int> error_pos;
};
//
//
//
//
//
