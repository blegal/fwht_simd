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
        uint8_t symb = vec_i_unroll_argmax(v_in);
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
        fwht<64>( f_in );
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
    printf("i_lwht_width = %d bits\n", i_lwht_width);
    printf("o_lwht_width = %d bits\n", o_lwht_width);
    printf("i_mult_width = %d bits\n", i_mult_width);
    printf("o_mult_width = %d bits\n", o_mult_width);
    printf("i_norm_width = %d bits\n", i_norm_width);
    printf("o_norm_width = %d bits\n", o_norm_width);

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
void the_decoder_v2(
            t_i_memo channel[256],
            uint16_t decoded[256]);
//
//
TEST_CASE( "decoder", "[decoder]" ) {

    const int froz[256] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    const int i_symb[256] = {
        26, 30, 63, 21, 17, 53, 50, 59, 58, 52, 22, 46, 39, 35, 13,  3,
         5, 33, 37, 39, 35, 40,  7,  5,  5, 45, 54, 47, 40, 11, 19,  3,
         1, 63, 45,  1, 44, 39, 20, 35, 23, 48, 61, 42, 28, 57, 54, 57,
        15,  2, 60, 46, 26, 62, 22, 13, 20, 16, 35, 50, 19, 13, 38, 43,
        51,  4, 41, 20, 43, 62, 41, 38, 19, 36,  0, 15, 27, 30, 13, 62,
        46,  9, 48, 12, 43, 63, 29, 55, 10,  2, 51, 49, 30, 42, 60, 43,
        59, 51, 12, 45, 31, 39, 34, 47, 54, 53, 14, 50, 12, 15,  1, 52,
        51, 28, 22,  5,  5, 27,  9, 32, 42, 56, 54, 10,  6,  0,  7, 35,
        57,  2, 21, 58, 50, 11, 35, 63, 25, 40, 60,  1,  4, 29, 28,  7,
        38, 61, 15,  8,  0, 22, 22,  1, 38, 49, 28,  0, 11, 53,  2,  7,
        34, 35,  7, 46, 15, 25,  5, 39, 52, 44, 23,  5, 63,  7, 39, 61,
        44, 30, 22,  1, 57,  0,  7,  9, 55, 12,  9, 29, 48, 51, 55, 47,
        16, 24,  3, 59,  8,  0, 56, 34, 48, 56, 42, 32, 56, 32, 28, 58,
        13, 21, 26, 35,  8,  1, 12, 51, 41, 30, 25, 30, 61, 20, 45, 47,
        24, 47, 38,  2, 60, 25, 51, 43, 21, 41, 36, 29, 47, 49, 16, 48,
        16,  0, 60, 42, 38, 37, 24, 36,  9, 36, 28, 37, 37, 62, 22, 39,
    };

    const int o_symb[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0, 50,  9, 16, 43,  8, 58, 21,  4,
        0,  0,  0, 44,  0,  0, 15, 30,  0, 58,  3, 15,  1, 55, 41, 16,
        0, 54, 61, 25, 30, 33, 52, 63, 33, 45, 24,  6, 33, 12, 46, 32,
        0, 34, 17,  3, 44, 58, 27, 36, 25, 51, 21, 37, 45, 62, 19,  5,
       33, 40, 55, 54, 57, 13, 27, 37, 10, 21,  4, 48, 49,  5, 41,  8,
        0, 37, 10, 42, 39, 26, 52, 28, 34, 20, 56,  4,  1, 57, 53,  2,
       11, 12, 33, 45,  8, 17, 48, 31, 37, 35, 13, 51,  1, 34, 51,  8,
       18, 25,  7,  8, 22, 43, 53, 24,  5, 45, 17, 47, 20, 24, 17, 23,
        7, 51, 34, 12, 21, 24, 13,  3, 62, 24,  8,  2, 42, 25, 49, 39
    };

    const int N = 256;

    t_i_memo channel[N];
    uint16_t decoded[N];

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
            printf("\n#(II) %3d | ", i);
        if (decoded[i] == o_symb[i]) {
            printf("\e[1;32m%2d\e[0m ", decoded[i]);
        } else {
            printf("\e[1;31m%2d\e[0m ", decoded[i]);
            //printf("\e[1;31m%2d (%2d)\e[0m ", decoded[i], o_symb[i]);
        }
    }

}
