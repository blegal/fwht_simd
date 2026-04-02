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
#include "definitions/custom_types.hpp"
#include "features/archi.hpp"
#include "hadamard/Hadamard.hpp"
//
//
//
//
//
template <int gf_size>
void g_function_freq_in(
    symbols_s<gf_size> * __restrict dst,
    symbols_s<gf_size> * __restrict src_a,
    symbols_s<gf_size> * __restrict src_b,
    const uint16_t * __restrict src_c,
    const int n_symbols) {
    for (int s = 0; s < n_symbols; s++) {
        const int     idx = src_c[s];
        const float * H   = gen_Hadamard_line<gf_size>(idx);
        for (size_t i = 0; i < gf_size; i++)
            dst[s].value[i] = src_a[s].value[i] * H[i];

        FWHT<gf_size>(dst[s].value);
        FWHT<gf_size>(src_b[s].value);

#if FWHT_COUNTER_ENABLE
        fwht_call_counter += 2;
#endif
        for (size_t i = 0; i < gf_size; i++)
            dst[s].value[i] = dst[s].value[i] * src_b[s].value[i];
        normalize<gf_size>(dst[s].value);
    }
}
//
//
//
//
//
template <int gf_size>
void g_function_proba_in(
    symbols_s<gf_size> * __restrict dst,
    const symbols_s<gf_size> * __restrict src_a,
    const symbols_s<gf_size> * __restrict src_b,
    const uint16_t * __restrict src_c,
    const int n_symbols) {
    for (int s = 0; s < n_symbols; s++) {
        for (int i = 0; i < gf_size; i += 1) {
            const int idx     = src_c[s] ^ i;
            dst[s].value[idx] = src_a[s].value[i] * src_b[s].value[idx];
        }
        normalize<gf_size>(dst[s].value); // temporal
    }
}
//
//
//
//
//
template <int gf_size>
void g_function_freq_in_after_rate_0(
    symbols_s<gf_size> * __restrict dst,
    symbols_s<gf_size> * __restrict src_a,
    symbols_s<gf_size> * __restrict src_b,
    const int n_symbols) {
    for (int s = 0; s < n_symbols; s++) {
        FWHT<gf_size>(src_a[s].value);
        FWHT<gf_size>(src_b[s].value);
#if FWHT_COUNTER_ENABLE
        fwht_call_counter += 2;
#endif

        for (int i = 0; i < gf_size; i++) {
            const float val = src_a[s].value[i] * src_b[s].value[/*idx*/ i];
            dst[s].value[i] = val;
        }
        normalize<gf_size>(dst[s].value);
    }
}
//
//
//
//
//
template <int gf_size>
void g_function_proba_in_after_rate_0(
    symbols_s<gf_size> * __restrict dst,
    const symbols_s<gf_size> * __restrict src_a,
    const symbols_s<gf_size> * __restrict src_b,
    const int n_symbols) {
    for (int s = 0; s < n_symbols; s++) {
        for (int i = 0; i < gf_size; i += 1) {
            dst[s].value[i] = src_a[s].value[i] * src_b[s].value[i];
        }
        normalize<gf_size>(dst[s].value);
    }
}
//
//
//
//
//
