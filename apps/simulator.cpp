
#include "pruning/decoder_pruned.hpp"

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "definitions/const_config_GF64_N64.hpp"
#include "utilities/utility_functions.hpp"

// Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

#include "nodes/top_node.hpp"
#include "nodes/top_node_with_pruning.hpp"

#include "pruning/decoder_specialized.hpp"

#include "frozen_tree.hpp"
#include "encoder/polar_encoder.hpp"
#include "demodulator/demodulator.hpp"


//
//
//
// In frozen symbol array, the value -1 means the symbol is frozen => (symbol = 0)
//

int main(int, char *[]) {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // initialize the frozen symbols array
    //
    int frozen_symbols[N];
//    uint16_t * symbols = new uint16_t[N];
    for (int i = 0; i < N; i += 1)
        frozen_symbols[i] = true;
    for (int i = 0; i < K; i += 1)
        frozen_symbols[reliab_seq[i]] = false; // i c'est pour le DEBUG, on pourrait mettre 0

    printf("\nFrozen matrix:\n");
    for (int i = 0; i < N; i += 1) {
        if ((i % 8) == 0)
            printf(" | ");
        if ((i % 16) == 0)
            printf("\n | ");
        printf("%2d ", frozen_symbols[i]);
    }
    printf(" |\n");

    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    std::vector<uint16_t>   symbol_k(K);
    std::vector<uint16_t>   symbol_n(N);
    std::vector<symbols_t>  llrs_n(N);
    std::vector<uint16_t>   decoded_n(N);
    std::vector<uint16_t>   decoded_k(K);

    //
    //
    //

    for (int i = 0; i < K; i++) {
        symbol_k[i] = rand()%GF;
    }

    printf("\n\nN Generatred symbols (K):\n");
    for (int i = 0; i < K; i += 1) {
        if ((i % 16) == 0)
            printf("\n ");
        printf("\e[1;32m%2d\e[0m ", symbol_k[i]);
    }
    printf("\n");

    //
    //
    //

    polar_encoder encoder(reliab_seq, K, N);
    encoder.encode( symbol_n.data(), symbol_k.data() ); // dst <= F(src)

    printf("\n\nN Encoded symbols (N):\n");
    for (int i = 0; i < N; i += 1) {
        if ((i % 16) == 0)
            printf("\n ");
        printf("\e[1;32m%2d\e[0m ", symbol_n[i]);
    }
    printf("\n");

    //
    // No modulation and no noise there
    //
#if 0
    std::vector< std::vector< softdata_t >> noisy_sig(N, std::vector<softdata_t>(bin_table[0].size(), (softdata_t)0.0));

    for (int i = 0; i < int(noisy_sig.size()); i++)
        noisy_sig[i] = bin_mod_dict[ NSYMB[i] ];
    // awgn_channel_noise(sigma, RepRndGn, 0, noisy_sig);
    {
        uint16_t q1 = noisy_sig[0].size();
        vector<vector<softdata_t>> noise_table(N, vector<softdata_t>(q1, 0));
        std::normal_distribution<double> norm_dist(0, sigma);
        {
            for (int i = 0; i < N; i++)
            {

                for (int j = 0; j < q1; j++)
                {
                    noise_table[i][j] = (softdata_t)norm_dist(gen);
                }
            }
        }

        for (int i = 0; i < N; i++)
            for (int j = 0; j < q1; j++)
                noisy_sig[i][j] += noise_table[i][j];
    }
    vector<vector<softdata_t>> chan_LLR(N, vector<softdata_t>(q, 0));
    Channel_LLR(noisy_sig, bin_table, q, sigma, chan_LLR);
    for (int i = 0; i < N; i++)
    {
        chan_LLR_sorted[i].intrinsic_LLR = chan_LLR[i];
    }
#endif
    //
    //
    //
    demodulator<GF> demod( N );
    demod.demodulate(llrs_n.data(), symbol_n.data()); // dst <= F(src)
#if 0
    printf("\n\nN demodulated symbol LLRs:\n");
    for (int i = 0; i < N; i += 1) {
        printf("(DD) Symbol n°%d", i);
        show_symbols<GF>( llrs_n[i].value );
    }
    printf("\n");
#endif
    //
    //  decoding
    //
    decoder_specialized<GF> decoder(N, frozen_symbols);
    decoder.execute(llrs_n.data(), decoded_n.data());

    printf("\n\nN decoded symbols:\n");
    for (int i = 0; i < N; i += 1) {
        if ((i % 16) == 0)
            printf("\n ");
        printf("\e[1;32m%2d\e[0m ", decoded_n[i]);
    }
    printf("\n");

    //
    //  Extracting initial K symbols
    //
    encoder.decode( decoded_k.data(), decoded_n.data() ); // dst <= F(src)

    printf("\n\nK decoded symbols:\n");
    for (int i = 0; i < K; i += 1) {
        if ((i % 16) == 0)
            printf("\n ");
        if (symbol_k[i] == decoded_k[i]) {
            printf("\e[1;32m%2d\e[0m ", decoded_k[i]);
        } else {
            printf("\e[1;31m%2d\e[0m ", decoded_k[i]);
        }
    }
    printf("\n");

    //
    //
    ///////////////////////////////////////////////////////////////////////////////
    //
    // Call the top node function to decode the symbols
    //


    // We should have the same symbols
    int nErrors = 0;
    for (int i = 0; i < K; i += 1) {
        if (symbol_k[i] != decoded_k[i]) {
            nErrors += 1;
        }
    }
    if ( nErrors == 0 ) {
        printf("Decoder OK\n");
    }else {
        printf("Decoder ERROR (%d/%d)\n", nErrors, K);
    }

    return EXIT_SUCCESS;
}
