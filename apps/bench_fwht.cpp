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
#include "../src/fwht_x86.hxx"
#include "../src/fwht_neon.hxx"
#include "../src/fwht_avx2.hxx"
#include <cstring>
#include <chrono>

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

    const  int32_t nTest = 256 * (1024 * 1024);

    for (int size = 8; size <= 256; size *= 2) {

        float* tab_i = new float[size];
        float* tab_a = new float[size];
        float* tab_b = new float[size];

        for (int i = 0; i < size; i++) {
            tab_i[i] = ((float)rand()) / ((float)RAND_MAX) - 0.5f;
            tab_a[i] = tab_i[i];
            tab_b[i] = tab_i[i];
        }

        printf("+> testing functions [ll = %4d]\n", size);

        auto start_x86 = std::chrono::system_clock::now();
        for(int32_t loop = 0; loop < nTest; loop += 1) {
            if (size ==   8) { fwht<  8>( tab_a ); fwht<  8>( tab_a ); normalize<  8>( tab_a,  1.f/ 8.f); }
            if (size ==  16) { fwht< 16>( tab_a ); fwht< 16>( tab_a ); normalize< 16>( tab_a,  1.f/ 16.f); }
            if (size ==  32) { fwht< 32>( tab_a ); fwht< 32>( tab_a ); normalize< 32>( tab_a,  1.f/ 32.f); }
            if (size ==  64) { fwht< 64>( tab_a ); fwht< 64>( tab_a ); normalize< 64>( tab_a,  1.f/ 64.f); }
            if (size == 128) { fwht<128>( tab_a ); fwht<128>( tab_a ); normalize<128>( tab_a,  1.f/ 128.f); }
            if (size == 256) { fwht<256>( tab_a ); fwht<256>( tab_a ); normalize<256>( tab_a,  1.f/ 256.f); }
        }
        auto stop_x86 = std::chrono::system_clock::now();
        const bool ok_x86 = are_equivalent(tab_i, tab_a, 0.002, size );
        const uint64_t time_x86 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_x86 - start_x86).count() / nTest;
        if( ok_x86 ){
            printf(" - [GCCV] fwht \033[32mOK\033[0m [%5d ns]\n", (int32_t)time_x86);
        }else{
            printf(" - [GCCV] fwht \033[31mKO\033[0m [%5d ns]\n", (int32_t)time_x86);
#if 0
            printf("(II) OUTPUT DATA VALUES");
            for (int i = 0; i < size; i += 1) {
                if ((i % 16) == 0) printf("\n [1] ");
                printf("%+6.1f ", tab_i[i]);
            }
            for (int i = 0; i < size; i += 1) {
                if ((i % 16) == 0) printf("\n [2] ");
                printf("%+6.1f ", tab_a[i]);
            }
            printf("\n");
            exit( EXIT_FAILURE );
#endif
        }


#if defined(__ARM_NEON__)

        auto start_i_neon = std::chrono::system_clock::now();
        for(int32_t loop = 0; loop < nTest; loop += 1) {
            if (size ==   8) { fwht_neon<  8>( tab_b ); fwht_neon<  8>( tab_b ); normalize<  8>( tab_b,  1.f/ 8.f); }
            if (size ==  16) { fwht_neon< 16>( tab_b ); fwht_neon< 16>( tab_b ); normalize< 16>( tab_b,  1.f/ 16.f); }
            if (size ==  32) { fwht_neon< 32>( tab_b ); fwht_neon< 32>( tab_b ); normalize< 32>( tab_b,  1.f/ 32.f); }
            if (size ==  64) { fwht_neon< 64>( tab_b ); fwht_neon< 64>( tab_b ); normalize< 64>( tab_b,  1.f/ 64.f); }
            if (size == 128) { fwht_neon<128>( tab_b ); fwht_neon<128>( tab_b ); normalize<128>( tab_b,  1.f/ 128.f); }
            if (size == 256) { fwht_neon<256>( tab_b ); fwht_neon<256>( tab_b ); normalize<256>( tab_b,  1.f/ 256.f); }
        }
        auto stop_i_neon = std::chrono::system_clock::now();
        const bool ok_neon = are_equivalent(tab_i, tab_b, 0.002, size );
        const uint64_t time_neon = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_i_neon - start_i_neon).count() / nTest;
        if( ok_neon ){
            printf(" - [NEON] fwht \033[32mOK\033[0m [%5d ns]\n", (int32_t)time_neon);
        }else{
            printf(" - [NEON] fwht \033[31mKO\033[0m [%5d ns]\n", (int32_t)time_neon);
        }
#endif

#if defined(__AVX2__)

        auto start_i_neon = std::chrono::system_clock::now();
        for(int32_t loop = 0; loop < nTest; loop += 1) {
            if (size ==   8) { fwht_avx2<  8>( tab_b ); fwht_avx2<  8>( tab_b ); normalize<  8>( tab_b,  1.f/ 8.f); }
            if (size ==  16) { fwht_avx2< 16>( tab_b ); fwht_avx2< 16>( tab_b ); normalize< 16>( tab_b,  1.f/ 16.f); }
            if (size ==  32) { fwht_avx2< 32>( tab_b ); fwht_avx2< 32>( tab_b ); normalize< 32>( tab_b,  1.f/ 32.f); }
            if (size ==  64) { fwht_avx2< 64>( tab_b ); fwht_avx2< 64>( tab_b ); normalize< 64>( tab_b,  1.f/ 64.f); }
            if (size == 128) { fwht_avx2<128>( tab_b ); fwht_avx2<128>( tab_b ); normalize<128>( tab_b,  1.f/ 128.f); }
            if (size == 256) { fwht_avx2<256>( tab_b ); fwht_avx2<256>( tab_b ); normalize<256>( tab_b,  1.f/ 256.f); }
        }
        auto stop_i_neon = std::chrono::system_clock::now();
        const bool ok_neon = are_equivalent(tab_i, tab_b, 0.002, size );
        const uint64_t time_neon = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_i_neon - start_i_neon).count() / nTest;
        if( ok_neon ){
            printf(" - [AVX2] fwht \033[32mOK\033[0m [%5d ns]\n", (int32_t)time_neon);
        }else{
            printf(" - [AVX2] fwht \033[31mKO\033[0m [%5d ns]\n", (int32_t)time_neon);
        }
#endif

        delete[] tab_i;
        delete[] tab_a;
        delete[] tab_b;
    }

    return EXIT_SUCCESS;
}
    
