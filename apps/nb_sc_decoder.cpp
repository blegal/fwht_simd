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
//
//
//
//
//
struct symbols_t
{
    float llr[64];
    char  gf [64];
    bool  is_freq;
};
//
//
//
//
//
symbols_t f_function(symbols_t a, symbols_t b)
{
    symbols_t result;
    // This function would implement some operation on the symbols.
    // For now, it is just a placeholder that copies the first symbol.
    for (int i = 0; i < 64; i++) {
        result.llr[i] = (a.llr[i] + b.llr[i]) / 2; // Example operation
        result.gf[i] = (a.gf[i] + b.gf[i]) / 2;   // Example operation
    }
    return result;
}
//
//
//
//
//
symbols_t g_function(symbols_t a, symbols_t b)
{
    symbols_t result;
    // This function would implement some operation on the symbols.
    // For now, it is just a placeholder that copies the first symbol.
    for (int i = 0; i < 64; i++) {
        result.llr[i] = (a.llr[i] + b.llr[i]) / 2; // Example operation
        result.gf[i] = (a.gf[i] + b.gf[i]) / 2;   // Example operation
    }
    return result;
}
//
//
//
//
//
void final_node(symbols_t* var)
{
    //
    // Switch from frequency to time domain if needed
    //
    if( var->is_freq ) {
        fwht<64>( var->llr );
        var->is_freq = false;
    }

    int max_index = 0;
    float max_value = var->llr[0];
    for (int i = 1; i < 64; i++) {
        if (var->llr[i] > max_value) {
            max_value = var->llr[i];
            max_index = i;
        }
    }

    return; // ???
}
//
//
//
//
//
void middle_node(symbols_t* inputs, symbols_t* internal, int size)
{
    const int n = size / 2; // Assuming size is the number of symbols
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        internal[i] = f_function( inputs[i], inputs[n + i] ); // Example operation
    }
    //
    // 
    //
    if( size == 1 ) {
        final_node(internal); // If we reach the final node, process it
    }else{
        middle_node(internal, internal + n, n/2); // On descend à gauche
    }
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        internal[i] = g_function( inputs[i], inputs[n + i] ); // Example operation
    }
    //
    // 
    //
    middle_node(internal, internal + n, n/2); // On descend à droite
    //
    // 
    //
    // should we do something here ?
    //
    // 
    //
}
//
//
//
//
//
void top_node(symbols_t* channel, symbols_t* internal, const int size)
{
    const int n = size / 2; // Assuming size is the number of symbols
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        internal[i] = f_function( channel[i], channel[n + i] ); // Example operation
    }
    //
    // 
    //
    middle_node(internal, internal + n, n/2); // On descend à gauche
    //
    // 
    //
    for (int i = 0; i < n; i++) {
        internal[i] = g_function( channel[i], channel[n + i] ); // Example operation
    }
    //
    // 
    //
    middle_node(internal, internal + n, n/2); // On descend à droite
    //
    // 
    //
    // should we do something here ?
    //
    // 
    //
}
//
//
//
//
//
int main(int argc, char* argv[])
{
    const int size = 64;

    symbols_t* channel  = new symbols_t[size];
    symbols_t* internal = new symbols_t[size];

    return EXIT_SUCCESS;
}
    
