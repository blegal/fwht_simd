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
#include "types.hpp"
#include "f_argmax.hpp"
#include "f_extend.hpp"
#include "f_mult.hpp"
#include "f_fwht.hpp"
#include "f_norm.hpp"
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
        t_int18b v_in;
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
        t_int18b v_in;
        for (int j = 0; j < gf_size; j++) {
            v_in.value[j] = (rand()%256) - 128;
        }
        //
        // On lance le test...
        //
        t_int24b resu = extend(v_in);
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
        t_int24b v_in_a;
        t_int24b v_in_b;
        for (int j = 0; j < gf_size; j++) {
            a[j] = (rand()%256) - 128;
            b[j] = (rand()%256) - 128;
            v_in_a.value[j] = a[j];
            v_in_b.value[j] = b[j];
        }
        //
        // On lance le test...
        //
        t_int48b resu = vec_i_mul_g( v_in_a, v_in_b, 0 );
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
        t_int18b v_in;
        for (int j = 0; j < gf_size; j++) {
            f_in[j]          = (rand()%256) - 128;
            v_in.value[j] = f_in[j];
        }
        //
        // On lance le test...
        //
        t_int24b v_ou = fwht( v_in );
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
//
//
////////////////////////////////////////////////////////////////////
//
//
#if 0
TEST_CASE( "scaling", "[scaling]" )
{
    //
    // On teste toutes les combinaisons
    //
    for (int i = 0; i < gf_size; i++) {
        //
        // On génere tous les vecteurs de test
        //
        t_int48b v_in_a;
        t_int18b v_in_b;
        for (int j = 0; j < gf_size; j++) {
            v_in_a.value[j] = 0x00001;
            v_in_b.value[j] = 0x20000;
        }
        //
        // On lance le test...
        //
        t_int48b resu = vec_i_mul_g( v_in_a, v_in_b, 0 );
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
#endif
//
//
////////////////////////////////////////////////////////////////////
//
//
