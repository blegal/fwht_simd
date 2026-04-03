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

#include "definitions/code.hpp"
#include "features/archi.hpp"
#include "utilities/utility_functions.hpp"

#include "hadamard/Hadamard.hpp"
//
//
//
//
//

template <int gf_size>
inline void proba_zero_removal(float *s1)
{
	for (int i = 0; i < gf_size; i++)
	{
		if (s1[i] <= 1e-10)
		{
			s1[i] = 1e-10;
		}
	}
}


template <int gf_size>
void naive_scf<gf_size>::g_function(
	symbols_t *__restrict dst,	 // the data to be computed for the left side of the graph
	symbols_t *__restrict src_a, // the upper value set from the right side of the graph
	symbols_t *__restrict src_b, // the lower value set from the right side of the graph
	uint32_t src_c)				 // the computed symbols coming from the left side of the graph
{
	if (src_a->is_freq == true)
	{
		const float *H = gen_Hadamard_line<gf_size>(src_c);
		for (size_t i = 0; i < gf_size; i++)
		{
			dst->value[i] = src_a->value[i] * H[i];
		}

		FWHT<gf_size>(dst->value);
		proba_zero_removal<gf_size>(dst->value);
		dst->is_freq = false;
	}
	else
	{
		for (size_t i = 0; i < gf_size; i++)
		{
			const int idx = src_c ^ i;
			dst->value[idx] = src_a->value[i];
		}
		dst->is_freq = false;
	}

	if (src_b->is_freq == true)
	{
		FWHT<gf_size>(src_b->value);
		proba_zero_removal<gf_size>(src_b->value);
#if FWHT_COUNTER_ENABLE
		fwht_call_counter += 1;
#endif
		src_b->is_freq = false;
	}

	for (size_t i = 0; i < gf_size; i++)
	{
		dst->value[i] = dst->value[i] * src_b->value[i];
	}
	normalize<gf_size>(dst->value); // temporal
	dst->is_freq = false;
}
//
//
//
//
//