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
#include "decoder_dedicated.hpp"
/**
 *
 * @param n
 * @param frozen_symb
 */
template <int gf_size>
decoder_dedicated<gf_size>::decoder_dedicated(const int n, const int* frozen_symb)
{
    internal = new symbols_s<gf_size>[n];
    symbols  = new uint16_t [n];

    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += (frozen_symb[i] == false);
    }

    if ( N_gen != n ) {
        printf("(EE) Error we should never be there...\n");
        printf("(EE) %s %d\n", __FILE__, __LINE__);
        printf("(EE) An issue was detected on N value between generated decoder and simulated one:\n");
        printf("(EE) - N value in generated decoder  = %d\n", N_gen);
        printf("(EE) - N value in simulated environ. = %d\n", n);
        exit( EXIT_FAILURE );
    }

    if ( K_gen != sum ) {
        printf("(EE) Error we should never be there...\n");
        printf("(EE) %s %d\n", __FILE__, __LINE__);
        printf("(EE) An issue was detected on K value between generated decoder and simulated one:\n");
        printf("(EE) - K value in generated decoder  = %d\n", K_gen);
        printf("(EE) - K value in simulated environ. = %d\n", sum);
        exit( EXIT_FAILURE );
    }
}

/**
 *
 */
template <int gf_size>
decoder_dedicated<gf_size>::decoder_dedicated()
{
    internal = nullptr;
    symbols  = nullptr;
    printf("(EE) Error we should never be there...\n");
    printf("(EE) %s %d\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
}

template <int gf_size>
decoder_dedicated<gf_size>::~decoder_dedicated()
{
    delete[]internal;
    delete[]symbols;
}

#include "dedicated_execute.hpp"

//
//
//
#if _GF_ == 8
    template class decoder_dedicated< 8>;
#elif _GF_ == 16
    template class decoder_dedicated< 16>;
#elif _GF_ == 32
    template class decoder_dedicated< 32>;
#elif _GF_ == 64
    template class decoder_dedicated< 64>;
#elif _GF_ == 128
    template class decoder_dedicated<128>;
#elif _GF_ == 256
    template class decoder_dedicated<256>;
#elif _GF_ == 512
    template class decoder_dedicated<512>;
#elif _GF_ == 1024
    template class decoder_dedicated<1024>;
#elif _GF_ == 2048
    template class decoder_dedicated<2048>;
#elif _GF_ == 4096
    template class decoder_dedicated<4096>;
#endif