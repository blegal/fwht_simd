
#include "../src/definitions/code.hpp"

#include "pruning/decoder_specialized.hpp"

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iomanip>

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

#include "nodes/decoder_naive.hpp"
#include "nodes/decoder_naive_pruning.hpp"

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
    const int  N = _N_;
    const int  K =  (3 * N) / 4;
    const int GF = _GF_;

#ifdef __AVX512BW__
    printf("#(II) Non-binary FFT Successive Cancellation decoder evaluation program (AVX512 version)\n");
#elif __AVX2__
    printf("#(II) Non-binary FFT Successive Cancellation decoder evaluation program (AVX2 version)\n");
#else
    printf("#(II) Non-binary FFT Successive Cancellation decoder evaluation program (ARM NEON version)\n");
#endif

    printf("#(II) + developped by Bertrand LE GAL   in 2025...\n");
    printf("#(II) +        and by Abdallah ABDALLAH in 2025...\n");
    printf("#(II) +        and by Camille MONIERE   in 2025...\n");
    printf("#(II)\n");
    printf("#(II) Binary generated : %s - %s\n", __DATE__, __TIME__);

#if defined(__clang__)
    /* Clang/LLVM. ---------------------------------------------- */
    printf("#(II) + Clang/LLVM version %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(__ICC) || defined(__INTEL_COMPILER)
    /* Intel ICC/ICPC. ------------------------------------------ */
    printf("# + Intel ICC/ICPC version %d.%d\n", __INTEL_COMPILER, __INTEL_COMPILER_BUILD_DATE);
#elif defined(__GNUC__) || defined(__GNUG__)
    /* GNU GCC/G++. --------------------------------------------- */
    printf("# + GNU GCC/G++ version %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    /* Microsoft Visual Studio. --------------------------------- */
    printf("# + Microsoft Visual Studio\n");
#else
    #error "# + Undetected compiler !"
#endif

#if (defined(__ICC) || defined(__INTEL_COMPILER)) == 0
    std::time_t t = std::time(nullptr);
    std::cout << "#(II) + Trace date and time : " << std::put_time(std::localtime(&t), "%c %Z") << '\n';
    printf("#(II)\n");
#endif

    std::cout << "#(II) Polar code parameters" << std::endl;
    std::cout << "#(II) ---------------------" << std::endl;
    std::cout << "#(II)" << std::endl;
    std::cout << "#(II) + GF equals : " << GF << std::endl;
    std::cout << "#(II) +  N equals : " << N << std::endl;
    std::cout << "#(II) +  K equals : " << K << std::endl;
    std::cout << "#(II)" << std::endl;

    int frozen_symbols[N];
    for (int i = 0; i < N; i += 1)
        frozen_symbols[i] = true;
    for (int i = 0; i < K; i += 1)
        frozen_symbols[reliab_seq[i]] = false;

    printf("#(II) Frozen matrix:\n");
    printf("#(II) -------------\n");
    printf("#(II) %3d | ", 0);
    for (int i = 0; i < N; i += 1) {
        if ( ((i % 8) == 0) && (i != 0) )
            printf(" | ");
        if ( ((i % 32) == 0) && (i != 0))
            printf("\n#(II) %3d | ", i);
        printf("%2d ", frozen_symbols[i]);
    }
    printf(" |\n");

    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    std::vector<uint16_t>   symbol_k (K);
    std::vector<uint16_t>   symbol_n (N);
    std::vector<symbols_t>  llrs_n   (N);
    std::vector<uint16_t>   decoded_n(N);
    std::vector<uint16_t>   decoded_k(K);

    //
    //
    //

    for (int i = 0; i < K; i++) {
        symbol_k[i] = rand()%GF;
    }

    printf("\n\nK Generatred symbols (%d):\n", K);
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

    printf("\n\nN Encoded symbols (%d):\n", N);
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
        printf("%2d ", decoded_n[i]);
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

    double nRunTest = 0;
    double nTotalus = 0;

    const int32_t nTest = (1024 * 1024 / GF);
    const auto debut = std::chrono::system_clock::now();
    for (int x = 0; x < 60; x += 1) {
        const auto start_x86 = std::chrono::system_clock::now();
        for(int32_t loop = 0; loop < nTest; loop += 1)
        {
            decoder.execute(llrs_n.data(), decoded_n.data());
        }
        const auto stop_x86 = std::chrono::system_clock::now();

        const auto time_ns= std::chrono::duration_cast<std::chrono::nanoseconds>(stop_x86 - start_x86).count();
        const auto time_sec  = time_ns / 1000.f / 1000.f / 1000.f; // in seconds
        const auto time_msec = time_ns / 1000.f / 1000.f; // in seconds
        const auto time_usec = time_ns / 1000.f; // in seconds

        nRunTest += nTest;
        nTotalus += time_usec;

        const double time_run  = (nTotalus / nRunTest);

        const auto debit = ((double)N * (double)_logGF_) / time_run; // in Ksymbols/s
        if ( x == 0 ) {
            printf("[GF=%d, N=%d, k=%d : SPEC] experiments  : %1.3f sec\n",  GF, N, K, time_sec);
            printf("[GF=%d, N=%d, k=%d : SPEC] experiments  : %1.2f ms\n",   GF, N, K, time_msec);
            printf("[GF=%d, N=%d, k=%d : SPEC] one decoding : %1.2f us\n",   GF, N, K, time_run);
            printf("[GF=%d, N=%d, k=%d : SPEC] debit coded  : %1.2f Mbps\n", GF, N, K, debit);
        }
        const auto curr = std::chrono::system_clock::now();
        const float ctime= std::chrono::duration_cast<std::chrono::seconds>(curr - debut).count();
        if ( ctime > 10 ){
            printf("[GF=%d, N=%d, k=%d : SPEC] experiments  : %1.3f sec\n",  GF, N, K, ctime);
            printf("[GF=%d, N=%d, k=%d : SPEC] debit coded  : %1.2f Mbps\n", GF, N, K, debit);
            break;
        }
    }

    return EXIT_SUCCESS;
}
