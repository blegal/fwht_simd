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
#include "fwht_x86.hxx"
#include <cstring>
#include <chrono>

int main(int argc, char* argv[])
{

#if defined (__APPLE__)
    printf("(II) Benchmarking the bit_pack/bit_unpack functions on MacOS\n");
#elif defined (__linux__)
    printf("(II) Benchmarking the bit_pack/bit_unpack functions on Linux\n");
#else
    printf("(II) Benchmarking the bit_pack/bit_unpack functions on a undefined platform\n");
#endif

#if defined (__clang__)
    printf("(II) Code compiled with LLVM (%d.%d.%d)\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined (__GNUC__)
    printf("(II) Code compiled with GCC (%d.%d.%d)\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#else
    printf("(II) Code compiled with UNKWON compiler\n");
#endif

    for (int size = 16; size <= 32; size *= 2) {

        float* tab_i = new float[size];

        for (int i = 0; i < size; i++) {
            tab_i[i] = ((float)rand()) / ((float)RAND_MAX) - 0.5f;
        }

        printf("(II) INPUT DATA VALUES");
        for(int i = 0; i < size; i += 1) {
            if ( (i%16) == 0 ) printf("\n");
            printf("%+1.5f ", tab_i[i]);
        } printf("\n");

        if (size ==  16) { fwht< 16>( tab_i ); normalize< 16>( tab_i,  1.f/16.f); }
        if (size ==  32) { fwht< 32>( tab_i ); normalize< 32>( tab_i,  1.f/32.f); }
        if (size ==  64) { fwht< 64>( tab_i ); }//normalize< 64>( tab_i,  1.f/32.f); }
        if (size == 128) { fwht<128>( tab_i ); }//normalize<128>( tab_i,  1.f/64.f); }
        if (size == 256) { fwht<256>( tab_i ); }//normalize<256>( tab_i, 1.f/128.f); }

        printf("(II) OUTPUT DATA VALUES");
        for(int i = 0; i < size; i += 1) {
            if ( (i%16) == 0 ) printf("\n");
            printf("%+1.5f ", tab_i[i]);
        } printf("\n");

        if (size ==  16) fwht< 16>( tab_i );
        if (size ==  32) fwht< 32>( tab_i );
        if (size ==  64) fwht< 64>( tab_i );
        if (size == 128) fwht<128>( tab_i );
        if (size == 256) fwht<256>( tab_i );

        printf("(II) FINAL DATA VALUES");
        for(int i = 0; i < size; i += 1) {
            if ( (i%16) == 0 ) printf("\n");
            printf("%+1.5f ", tab_i[i]);
        } printf("\n");

    }

    return EXIT_SUCCESS;
}
    
