/*
 *  Copyright (c) 2022 Bertrand LE GAL
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
//
//
#define CATCH_CONFIG_MAIN
#include "./catch2v3/catch_amalgamated.hpp"
//
//
#include "impl/types.hpp"
#include "impl/f_argmax.hpp"
#include "impl/f_extend.hpp"
#include "impl/f_mult.hpp"
#include "impl/f_fwht.hpp"
#include "impl/f_norm.hpp"
#include "impl/f_xor_processor.hpp"
#include "gen_decoder.hpp"
//
#include "decoders/naive_int32_t/arch/i_fwht.hpp"
//
//
////////////////////////////////////////////////////////////////////
//
//
TEST_CASE( "argmax", "[argmax]" )
{
    //
    // On teste toutes les combinaisons
    //
    for (int i = 0; i < gf_size; i++) {
        //
        // On génere tous les vecteurs de test
        //
        t_i_memo v_in;
        for (int j = 0; j < gf_size; j++) {
            v_in.value[j] = 1;
        }
        v_in.value[i] = 100;
        //
        // On lance le test...
        //
        uint8_t symb = vec_i_argmax(v_in);
        //
        // On verifie la validité du résultat
        //
        REQUIRE( symb == i );
        //
   }
}
//
//
////////////////////////////////////////////////////////////////////
//
//
TEST_CASE( "extend", "[extend]" )
{
    //
    // On teste toutes les combinaisons
    //
    for (int i = 0; i < gf_size; i++) {
        //
        // On génere tous les vecteurs de test
        //
        t_i_memo v_in;
        for (int j = 0; j < gf_size; j++) {
            v_in.value[j] = (rand()%256) - 128;
        }
        //
        // On lance le test...
        //
        t_o_lwht resu = extend(v_in);
        //
        // On verifie la validité du résultat
        //
        for (int i = 0; i < gf_size; i++)
            REQUIRE( v_in.value[i] == resu.value[i] );
        //
    }
}
//
//
////////////////////////////////////////////////////////////////////
//
//
TEST_CASE( "multiply", "[multiply]" )
{
    //
    // On teste toutes les combinaisons
    //
    for (int i = 0; i < gf_size; i++) {
        //
        // On génere tous les vecteurs de test
        //
        int64_t a[gf_size];
        int64_t b[gf_size];
        t_i_mult v_in_a;
        t_i_mult v_in_b;
        for (int j = 0; j < gf_size; j++) {
            a[j] = (rand()%256) - 128;
            b[j] = (rand()%256) - 128;
            v_in_a.value[j] = a[j];
            v_in_b.value[j] = b[j];
        }
        //
        // On lance le test...
        //
        t_o_mult resu = vec_i_mul_g( v_in_a, v_in_b, 0 );
        //
        // On verifie la validité du résultat
        //
        for (int i = 0; i < gf_size; i++) {
            const int64_t c = (a[i] * b[i]);
            REQUIRE( resu.value[i] == c );
        }
        //
    }
}
//
//
////////////////////////////////////////////////////////////////////
//
//
TEST_CASE( "fwht", "[fwht]" )
{
    //
    // On teste toutes les combinaisons
    //
    for (int i = 0; i < gf_size; i++) {
        //
        // On génere tous les vecteurs de test
        //
        int32_t f_in[gf_size];
        t_i_lwht v_in;
        for (int j = 0; j < gf_size; j++) {
            f_in[j]          = (rand()%256) - 128;
            v_in.value[j] = f_in[j];
        }
        //
        // On lance le test...
        //
        t_o_lwht v_ou = fwht( v_in );
        naive_fwht<64>( f_in );
        //
        // On verifie la validité du résultat
        //
        for (int i = 0; i < gf_size; i++) {
            REQUIRE( v_ou.value[i] == f_in[i] );
        }
        //
    }
}
//
//
////////////////////////////////////////////////////////////////////
//
//
TEST_CASE( "scaling", "[scaling]" )
{
    //
    // On teste toutes les combinaisons
    //
    for (int i = 0; i < (o_norm_width - 1); i++) {
        //
        // On génere tous les vecteurs de test
        //
        t_i_norm v_in_a;
        for (int j = 0; j < gf_size; j++) {
            ap_int <i_norm_width> v_test = 0x10000;
            v_in_a.value[j] = (v_test << i);
        }
        //
        // On lance le test...
        //
        t_o_norm resu = vec_i_norm( v_in_a );
        //
        // On verifie la validité du résultat
        //
        for (int j = 0; j < gf_size; j++) {
            REQUIRE( resu.value[j] == 0x00400 ); // 12 bits - 1000 0000 0000
        }
        //
    }
}
//
//
////////////////////////////////////////////////////////////////////
//
//
TEST_CASE( "xor_remove_2", "[xor_remove_2]" )
{
    //
    // On teste toutes les combinaisons
    //
    for (int i = 2; i < 512; i *= 2) {
        //
        // On génere tous les vecteurs de test
        //
        uint8_t* array_i = new uint8_t[i];
        for (int j = 0; j < i; j++) {
            array_i[j] = rand()%256;
        }
        //
        // On lance le test...
        //
        uint8_t* array_r = new uint8_t[i];
        for (int j = 0; j < i; j++) {
            array_r[j] = array_i[j];
        }
        local_remove_xors(array_r, i);
        //
        // On lance le test...
        //

        uint8_t* array_t = new uint8_t[i];
        if ( i ==   2 ) v2_xor_processor  (array_t, array_i, 0x01);
        if ( i ==   4 ) v4_xor_processor  (array_t, array_i, 0x03);
        if ( i ==   8 ) v8_xor_processor  (array_t, array_i, 0x07);
        if ( i ==  16 ) v16_xor_processor (array_t, array_i, 0x0F);
        if ( i ==  32 ) v32_xor_processor (array_t, array_i, 0x1F);
        if ( i ==  64 ) v64_xor_processor (array_t, array_i, 0x3F);
        if ( i == 128 ) v128_xor_processor(array_t, array_i, 0x7F);
        if ( i == 256 ) v256_xor_processor(array_t, array_i, 0xFF);
        //
        // On verifie la validité du résultat
        //
        for (int j = 0; j < i; j++) {
            REQUIRE( array_r[j] == array_t[j] );
        }
        //

        delete[] array_i;
        delete[] array_r;
        delete[] array_t;
    }
}
//
//
////////////////////////////////////////////////////////////////////
//
//
void the_decoder_v2(
            t_i_memo channel[256],
            uint8_t  decoded[256]);
//
//
TEST_CASE( "decoder", "[decoder]" ) {


    printf("i_lwht_width = %d bits\n", i_lwht_width);
    printf("o_lwht_width = %d bits\n", o_lwht_width);
    printf("i_mult_width = %d bits\n", i_mult_width);
    printf("o_mult_width = %d bits\n", o_mult_width);
    printf("i_norm_width = %d bits\n", i_norm_width);
    printf("o_norm_width = %d bits\n", o_norm_width);
    printf("\n");
    printf("Parameter N  = %4d symb (log2 = %d)\n", t_N, t_log2N);
    printf("Parameter K  = %4d symb\n", t_K);
    const float R = 100.f * (float)t_K / (float)t_N;
    printf("Parameter R  = %4d symb\n", (int)R);
    printf("Parameter GF = %4d elmt (log2 = %d)\n", t_GF, t_log2GF);
    printf("\n");
#if (t_N == 64) && (t_K == 16)
    #include "tests/N64_K16.hpp"
#elif (t_N == 64) && (t_K == 21)
    #include "tests/N64_K21.hpp"
#elif (t_N == 64) && (t_K == 26)
    #include "tests/N64_K26.hpp"
#elif (t_N == 64) && (t_K == 32)
    #include "tests/N64_K32.hpp"
#elif (t_N == 64) && (t_K == 48)
    #include "tests/N64_K48.hpp"
#elif (t_N == 64) && (t_K == 51)
    #include "tests/N64_K51.hpp"

#elif (t_N == 128) && (t_K == 32)
    #include "tests/N128_K32.hpp"
#elif (t_N == 128) && (t_K == 42)
    #include "tests/N128_K42.hpp"
#elif (t_N == 128) && (t_K == 51)
    #include "tests/N128_K51.hpp"
#elif (t_N == 128) && (t_K == 64)
    #include "tests/N128_K64.hpp"
#elif (t_N == 128) && (t_K == 96)
    #include "tests/N128_K96.hpp"
#elif (t_N == 128) && (t_K == 102)
    #include "tests/N128_K102.hpp"

#elif (t_N == 256) && (t_K == 64)
    #include "tests/N256_K64.hpp"
#elif (t_N == 256) && (t_K == 84)
    #include "tests/N256_K84.hpp"
#elif (t_N == 256) && (t_K == 102)
    #include "tests/N256_K102.hpp"
#elif (t_N == 256) && (t_K == 128)
    #include "tests/N256_K128.hpp"
#elif (t_N == 256) && (t_K == 192)
    #include "tests/N256_K192.hpp"
#elif (t_N == 256) && (t_K == 205)
    #include "tests/N256_K205.hpp"
#endif

    t_i_memo channel[N];
    uint8_t  decoded[N];

    // Input initialization

    for (int i = 0; i < N; i++) {
        const int symb = i_symb[i];
        for (int j = 0; j < gf_size; j++) {
            channel[i].value[j] = 0;
        }
        channel[i].value[symb] = 2047;
    }

    // Output initialization

    for (int i = 0; i < N; i++) {
        decoded[i] = 0;
    }

    the_decoder_v2(channel, decoded);

    printf("#(II)\n");
    printf("#(II) K decoded symbols (%3d) :\n", N);
    printf("#(II) ------------------------\n");
    printf("#(II)");
    for (int i = 0; i < N; i += 1) {
        if (((i % 16) == 0))
            printf("|\n#(II) %3d ", i);
        if ( (i%8) == 0 ) printf("| ");
        else if ( (i%4) == 0 ) printf("  ");
        if (decoded[i] == o_symb[i]) {
            printf("\e[1;32m%2d\e[0m ", (int)decoded[i]);
        } else {
            printf("\e[1;31m%2d\e[0m ", (int)decoded[i]);
            //printf("\e[1;31m%2d (%2d)\e[0m ", decoded[i], o_symb[i]);
        }
    }
    printf("|\n");

}
