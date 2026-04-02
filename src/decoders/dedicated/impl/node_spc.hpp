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
#pragma once
#include "../fix_xor_list.hpp"
#include "definitions/custom_types.hpp"
#include "features/archi.hpp"
//
//
//
//
//
extern void local_remove_xors(uint16_t * values, int size);
extern void remove_xors(uint16_t * values, int size);
//
//
//
//
//
template <int gf_size>
void middle_node_pruned_spc_after_f(
    symbols_s<gf_size> * __restrict inputs, // Inputs are the symbols from the channel (from the right)
    uint16_t * __restrict decoded,          // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t * __restrict symbols,          // Symbols are the ones going from leafs to root (done on the left)
    int size                                // Size is the number of symbols (should be a power of 2)
) {
    //
    for (int i = 0; i < size; i++) {
        FWHT<gf_size>(inputs[i].value);
    }
#if FWHT_COUNTER_ENABLE
    fwht_call_counter += size;
#endif
    //
    int      check_node = 0;
    uint16_t arg_1[512];
    for (int i = 0; i < size; i++) {
        int value = argmax<gf_size>(inputs[i].value);
        check_node ^= value;
        symbols[i] = value;
        decoded[i] = value; // should be corrected (it is systematic solution actually)
        arg_1[i]   = value;
    }
    //
    if (check_node == 0) {
        remove_xors(decoded, size);
        return;
    }
    //
    uint16_t arg_2[512];
    for (int j = 0; j < size; j++) {
        arg_2[j] = argmax2<gf_size>(inputs[j].value, arg_1[j]);
    }
    //
    fix_xor_list<gf_size>(arg_1, arg_2, inputs, size);
    //
    for (int j = 0; j < size; j++) {
        symbols[j] = arg_1[j];
        decoded[j] = arg_1[j]; // should be corrected (it is systematic solution actually)
    }
    //
    remove_xors(decoded, size);
    //
}
//
//
//
//
//
template <int gf_size>
void middle_node_pruned_spc_after_g(
    symbols_s<gf_size> * __restrict inputs, // Inputs are the symbols from the channel (from the right)
    uint16_t * __restrict decoded,          // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t * __restrict symbols,          // Symbols are the ones going from leafs to root (done on the left)
    const int size                          // Size is the number of symbols (should be a power of 2)
) {
    //
    int      check_node = 0;
    uint16_t arg_1[512];
    for (int i = 0; i < size; i++) {
        int value = argmax<gf_size>(inputs[i].value);
        check_node ^= value;
        symbols[i] = value;
        decoded[i] = value; // should be corrected (it is systematic solution actually)
        arg_1[i]   = value;
    }
    //
    if (check_node == 0) {
        remove_xors(decoded, size);
        return;
    }
    //
    uint16_t arg_2[512];
    for (int j = 0; j < size; j++) {
        arg_2[j] = argmax2<gf_size>(inputs[j].value, arg_1[j]);
    }
    //
    fix_xor_list<gf_size>(arg_1, arg_2, inputs, size);
    //
    for (int j = 0; j < size; j++) {
        symbols[j] = arg_1[j];
        decoded[j] = arg_1[j]; // should be corrected (it is systematic solution actually)
    }
    //
    remove_xors(decoded, size);
    //
}
//
//
//
//
//
