/*
*	Optimized bit-packing and bit-unpacking functions - Copyright (c) 2021 Bertrand LE GAL
*
*  This software is provided 'as-is', without any express or
*  implied warranty. In no event will the authors be held
*  liable for any damages arising from the use of this software.
*
*  Permission is granted to anyone to use this software for any purpose,
*  including commercial applications, and to alter it and redistribute
*  it freely, subject to the following restrictions:
*
*  1. The origin of this software must not be misrepresented;
*  you must not claim that you wrote the original software.
*  If you use this software in a product, an acknowledgment
*  in the product documentation would be appreciated but
*  is not required.
*
*  2. Altered source versions must be plainly marked as such,
*  and must not be misrepresented as being the original software.
*
*  3. This notice may not be removed or altered from any
*  source distribution.
*
*/
#include "../src/fwht/fwht_x86.hpp"
#include "../src/fwht/fwht_norm_x86.hpp"
#include "../src/fwht/fwht_norm_neon.hpp"
#include "../src/fwht/fwht_neon.hpp"
#include "../src/fwht/fwht_norm_avx2.hpp"
#include "../src/fwht/fwht_avx2.hpp"
#include <cstring>
#include <chrono>

#include "../src/structure.hpp"

bool are_equivalent(float* a, float* b, float epsilon, int size) {
    for (int i = 0; i < size; i++) {
        float diff = abs(a[i] - b[i]);
        if (diff > epsilon) {
            printf("- maximum absolute error is : %f\n", diff);
            printf("- a[%d] = %f and b[%d] = %f\n", i, a[i], i, b[i]);
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[])
{

    if( argc == 2 )
        srand( time(NULL) );

#if defined (__APPLE__)
    printf("(II) Benchmarking the FWHT functions on MacOS\n");
#elif defined (__linux__)
    printf("(II) Benchmarking the FWHT functions on Linux\n");
#else
    printf("(II) Benchmarking the FWHT functions on a undefined platform\n");
#endif

#if defined (__clang__)
    printf("(II) Code compiled with LLVM (%d.%d.%d)\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined (__GNUC__)
    printf("(II) Code compiled with GCC (%d.%d.%d)\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#else
    printf("(II) Code compiled with UNKWON compiler\n");
#endif

    const  int32_t nTest = (1024 * 1024); //64 * (1024 * 1024);

    for (int size = 16; size <= 256; size *= 2) {

        float* tab_i = new float[size];
        float* tab_a = new float[size];
        float* tab_b = new float[size];
        float* tab_c = new float[size];
        float* tab_d = new float[size];

        float* tab_e = new float[size];
        float* tab_f = new float[size];

        for (int i = 0; i < size; i++) {
            tab_i[i] = ((float)rand()) / ((float)RAND_MAX) - 0.5f;
            tab_a[i] = tab_i[i];
            tab_b[i] = tab_i[i];
            tab_c[i] = tab_i[i];
            tab_d[i] = tab_i[i];
            tab_e[i] = tab_i[i];
        }
#if 0
        fwht< 64>( tab_a );
        normalize< 64>( tab_a, 0.125f );
        show_symbols( tab_a );
        fwht< 64>( tab_a );
        normalize< 64>( tab_a, 0.125f );
        show_symbols( tab_a );


        fwht_avx2< 64>( tab_d );
        normalize< 64>( tab_d, 0.125f );
        show_symbols( tab_d );
        fwht_avx2< 64>( tab_d );
        normalize< 64>( tab_d, 0.125f );
        show_symbols( tab_d );

        fwht_norm_avx2< 64>( tab_e );
        show_symbols( tab_e );
        fwht_norm_avx2< 64>( tab_e );
        show_symbols( tab_e );

        exit( 0 );
#endif
        printf("+> testing functions [ll = %4d]\n", size);

        auto start_x86 = std::chrono::system_clock::now();
        for(int32_t loop = 0; loop < nTest; loop += 1) {
            if (size ==   8) { fwht<  8>( tab_a ); normalize<  8>( tab_a, 0.35355339059f); fwht<  8>( tab_a ); normalize<  8>( tab_a, 0.35355339059f); }
            if (size ==  16) { fwht< 16>( tab_a ); normalize< 16>( tab_a, 0.25f         ); fwht< 16>( tab_a ); normalize< 16>( tab_a, 0.25f         ); }
            if (size ==  32) { fwht< 32>( tab_a ); normalize< 32>( tab_a, 0.17677669529f); fwht< 32>( tab_a ); normalize< 32>( tab_a, 0.17677669529f); }
            if (size ==  64) { fwht< 64>( tab_a ); normalize< 64>( tab_a, 0.125f        ); fwht< 64>( tab_a ); normalize< 64>( tab_a, 0.125f        ); }
            if (size == 128) { fwht<128>( tab_a ); normalize<128>( tab_a, 0.08838834764f); fwht<128>( tab_a ); normalize<128>( tab_a, 0.08838834764f); }
            if (size == 256) { fwht<256>( tab_a ); normalize<256>( tab_a, 0.0625f       ); fwht<256>( tab_a ); normalize<256>( tab_a, 0.0625f       ); }
        }
        auto stop_x86     = std::chrono::system_clock::now();
        bool ok_x86       = are_equivalent(tab_i, tab_a, 0.002, size );
        uint64_t time_x86 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_x86 - start_x86).count() / nTest;
        if( ok_x86 ){
            printf(" - [GCCV] fwht           \033[32mOK\033[0m [%5d ns]\n", (int32_t)time_x86);
        }else{
            printf(" - [GCCV] fwht           \033[31mKO\033[0m [%5d ns]\n", (int32_t)time_x86);
        }

        start_x86 = std::chrono::system_clock::now();
        for(int32_t loop = 0; loop < nTest; loop += 1) {
            if (size ==   8) { fwht<  8>( tab_e, tab_d ); normalize<  8>( tab_e, 0.35355339059f); fwht<  8>( tab_d, tab_e ); normalize<  8>( tab_d, 0.35355339059f); }
            if (size ==  16) { fwht< 16>( tab_e, tab_d ); normalize< 16>( tab_e, 0.25f         ); fwht< 16>( tab_d, tab_e ); normalize< 16>( tab_d, 0.25f         ); }
            if (size ==  32) { fwht< 32>( tab_e, tab_d ); normalize< 32>( tab_e, 0.17677669529f); fwht< 32>( tab_d, tab_e ); normalize< 32>( tab_d, 0.17677669529f); }
            if (size ==  64) { fwht< 64>( tab_e, tab_d ); normalize< 64>( tab_e, 0.125f        ); fwht< 64>( tab_d, tab_e ); normalize< 64>( tab_d, 0.125f        ); }
            if (size == 128) { fwht<128>( tab_e, tab_d ); normalize<128>( tab_e, 0.08838834764f); fwht<128>( tab_d, tab_e ); normalize<128>( tab_d, 0.08838834764f); }
            if (size == 256) { fwht<256>( tab_e, tab_d ); normalize<256>( tab_e, 0.0625f       ); fwht<256>( tab_d, tab_e ); normalize<256>( tab_d, 0.0625f       ); }
        }
        stop_x86 = std::chrono::system_clock::now();
        ok_x86   = are_equivalent(tab_i, tab_d, 0.002, size );
        time_x86 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_x86 - start_x86).count() / nTest;
        if( ok_x86 ){
            printf(" - [GCCV] fwht (2x)      \033[32mOK\033[0m [%5d ns]\n", (int32_t)time_x86);
        }else{
            printf(" - [GCCV] fwht (2x)      \033[31mKO\033[0m [%5d ns]\n", (int32_t)time_x86);
        }

#if 1
        auto start_x86_n = std::chrono::system_clock::now();
        for(int32_t loop = 0; loop < nTest; loop += 1) {
            if (size ==   8) { fwht_norm<  8>( tab_b ); fwht_norm<  8>( tab_b ); }
            if (size ==  16) { fwht_norm< 16>( tab_b ); fwht_norm< 16>( tab_b ); }
            if (size ==  32) { fwht_norm< 32>( tab_b ); fwht_norm< 32>( tab_b ); }
            if (size ==  64) { fwht_norm< 64>( tab_b ); fwht_norm< 64>( tab_b ); }
            if (size == 128) { fwht_norm<128>( tab_b ); fwht_norm<128>( tab_b ); }
            if (size == 256) { fwht_norm<256>( tab_b ); fwht_norm<256>( tab_b ); }
        }
        auto stop_x86_n = std::chrono::system_clock::now();
        const bool ok_x86_n = are_equivalent(tab_i, tab_b, 0.002, size );
        const uint64_t time_x86_n = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_x86_n - start_x86_n).count() / nTest;
        if( ok_x86_n ){
            printf(" - [GCCV] fwht_norm      \033[32mOK\033[0m [%5d ns]\n", (int32_t)time_x86_n);
        }else{
            printf(" - [GCCV] fwht_norm      \033[31mKO\033[0m [%5d ns]\n", (int32_t)time_x86_n);
        }
#endif

#if defined(__ARM_NEON__)

        auto start_i_neon = std::chrono::system_clock::now();
        for(int32_t loop = 0; loop < nTest; loop += 1) {
            if (size ==   8) { fwht_neon<  8>( tab_c ); fwht_neon<  8>( tab_c ); normalize<  8>( tab_c,  1.f/ 8.f); }
            if (size ==  16) { fwht_neon< 16>( tab_c ); fwht_neon< 16>( tab_c ); normalize< 16>( tab_c,  1.f/ 16.f); }
            if (size ==  32) { fwht_neon< 32>( tab_c ); fwht_neon< 32>( tab_c ); normalize< 32>( tab_c,  1.f/ 32.f); }
            if (size ==  64) { fwht_neon< 64>( tab_c ); fwht_neon< 64>( tab_c ); normalize< 64>( tab_c,  1.f/ 64.f); }
            if (size == 128) { fwht_neon<128>( tab_c ); fwht_neon<128>( tab_c ); normalize<128>( tab_c,  1.f/ 128.f); }
            if (size == 256) { fwht_neon<256>( tab_c ); fwht_neon<256>( tab_c ); normalize<256>( tab_c,  1.f/ 256.f); }
        }
        auto stop_i_neon = std::chrono::system_clock::now();
        const bool ok_neon = are_equivalent(tab_i, tab_c, 0.002, size );
        const uint64_t time_neon = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_i_neon - start_i_neon).count() / nTest;
        if( ok_neon ){
            printf(" - [NEON] fwht           \033[32mOK\033[0m [%5d ns]\n", (int32_t)time_neon);
        }else{
            printf(" - [NEON] fwht           \033[31mKO\033[0m [%5d ns]\n", (int32_t)time_neon);
        }
#endif

#if defined(__ARM_NEON__)
        auto start_i_neon_norm = std::chrono::system_clock::now();
        for(int32_t loop = 0; loop < nTest; loop += 1) {
            if (size ==   8) { fwht_norm_neon<  8>( tab_d ); fwht_norm_neon<  8>( tab_d ); }
            if (size ==  16) { fwht_norm_neon< 16>( tab_d ); fwht_norm_neon< 16>( tab_d ); }
            if (size ==  32) { fwht_norm_neon< 32>( tab_d ); fwht_norm_neon< 32>( tab_d ); }
            if (size ==  64) { fwht_norm_neon< 64>( tab_d ); fwht_norm_neon< 64>( tab_d ); }
            if (size == 128) { fwht_norm_neon<128>( tab_d ); fwht_norm_neon<128>( tab_d ); }
            if (size == 256) { fwht_norm_neon<256>( tab_d ); fwht_norm_neon<256>( tab_d ); }
        }
        auto stop_i_neon_norm = std::chrono::system_clock::now();
        const bool ok_neon_norm = are_equivalent(tab_i, tab_d, 0.002, size );
        const uint64_t time_neon_norm = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_i_neon_norm - start_i_neon_norm).count() / nTest;
        if( ok_x86_n ){
            printf(" - [NEON] fwht_norm_neon \033[32mOK\033[0m [%5d ns]\n", (int32_t)time_neon_norm);
        }else{
            printf(" - [NEON] fwht_norm_neon \033[31mKO\033[0m [%5d ns]\n", (int32_t)time_neon_norm);
        }
#endif

#if defined(__AVX2__)

        auto start_i_neon = std::chrono::system_clock::now();
        for(int32_t loop = 0; loop < nTest; loop += 1) {
            if (size ==   8) { fwht_avx2<  8>( tab_c ); fwht_avx2<  8>( tab_c ); normalize<  8>( tab_c,  1.f/ 8.f); }
            if (size ==  16) { fwht_avx2< 16>( tab_c ); fwht_avx2< 16>( tab_c ); normalize< 16>( tab_c,  1.f/ 16.f); }
            if (size ==  32) { fwht_avx2< 32>( tab_c ); fwht_avx2< 32>( tab_c ); normalize< 32>( tab_c,  1.f/ 32.f); }
            if (size ==  64) { fwht_avx2< 64>( tab_c ); fwht_avx2< 64>( tab_c ); normalize< 64>( tab_c,  1.f/ 64.f); }
            if (size == 128) { fwht_avx2<128>( tab_c ); fwht_avx2<128>( tab_c ); normalize<128>( tab_c,  1.f/ 128.f); }
            if (size == 256) { fwht_avx2<256>( tab_c ); fwht_avx2<256>( tab_c ); normalize<256>( tab_c,  1.f/ 256.f); }
        }
        auto stop_i_neon = std::chrono::system_clock::now();
        const bool ok_neon = are_equivalent(tab_i, tab_c, 0.002, size );
        const uint64_t time_neon = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_i_neon - start_i_neon).count() / nTest;
        if( ok_neon ){
            printf(" - [AVX2] fwht_avx2      \033[32mOK\033[0m [%5d ns]\n", (int32_t)time_neon);
        }else{
            printf(" - [AVX2] fwht_avx2      \033[31mKO\033[0m [%5d ns]\n", (int32_t)time_neon);
        }
#endif

#if defined(__AVX2__)

        auto start_i_avx2 = std::chrono::system_clock::now();
        for(int32_t loop = 0; loop < nTest; loop += 1) {
            if (size ==   8) { fwht_norm_avx2<  8>( tab_d ); fwht_norm_avx2<  8>( tab_d ); }
            if (size ==  16) { fwht_norm_avx2< 16>( tab_d ); fwht_norm_avx2< 16>( tab_d ); }
            if (size ==  32) { fwht_norm_avx2< 32>( tab_d ); fwht_norm_avx2< 32>( tab_d ); }
            if (size ==  64) { fwht_norm_avx2< 64>( tab_d ); fwht_norm_avx2< 64>( tab_d ); }
            if (size == 128) { fwht_norm_avx2<128>( tab_d ); fwht_norm_avx2<128>( tab_d ); }
            if (size == 256) { fwht_norm_avx2<256>( tab_d ); fwht_norm_avx2<256>( tab_d ); }
        }
        auto stop_i_avx2 = std::chrono::system_clock::now();
        const bool ok_norm_avx2 = are_equivalent(tab_i, tab_d, 0.002, size );
        const uint64_t time_norm_avx2 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_i_avx2 - start_i_avx2).count() / nTest;
        if( ok_norm_avx2 ){
            printf(" - [AVX2] fwht_norm_avx2 \033[32mOK\033[0m [%5d ns]\n", (int32_t)time_norm_avx2);
        }else{
            printf(" - [AVX2] fwht_norm_avx2 \033[31mKO\033[0m [%5d ns]\n", (int32_t)time_norm_avx2);
        }
#endif

        delete[] tab_i;
        delete[] tab_a;
        delete[] tab_b;
        delete[] tab_c;
        delete[] tab_d;
        delete[] tab_e;
        delete[] tab_f;
    }

    return EXIT_SUCCESS;
}
    
