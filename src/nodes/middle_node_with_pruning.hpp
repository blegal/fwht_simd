#pragma once
//
//
//
//
//
#include "node_functions/f_function.hpp"
#include "node_functions/g_function.hpp"
#include "nodes/leaf_node.hpp"
#include "fwht/fwht_norm.hpp"

//
//
//
//
//
void remove_xors(uint16_t * values, int size) {
    if (size == 1) {
        return;
    }
    for (int i = 0; i < size / 2; i += 1) {
        values[i] ^= values[i + size / 2];
    }
    remove_xors(values, size / 2);
    remove_xors(values + size / 2, size / 2);
}
//
//
//
//
//
template <int gf_size>
void middle_node_with_pruning(
    symbols_t * inputs,   // Inputs are the symbols from the channel (from the right)
    symbols_t * internal, // Internal nodes are the symbols computed during the process (to the left)
    uint16_t *  decoded,  // Decoded symbols are the final output of the decoder (done on the left)
    uint16_t *  symbols,  // Symbols are the ones going from leafs to root (done on the left)
    int         size,     // Size is the number of symbols (should be a power of 2)
    const int   symbol_id)  // Symbol ID is the index of the FIRST symbol in the symbols array
{
    const int n = size / 2; // Assuming size is the number of symbols

    ///////////////////////////////////////////////////////////////////////////////////////
    //
#if 1
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += frozen_symbols[symbol_id + i];
    }
    if (sum == size) {
#if defined(debug_rate_1)
        printf("Frozen pruning with rate = 0\n");
#endif
        for (int i = 0; i < size; i++) {
            symbols[symbol_id + i] = 0;
            decoded[symbol_id + i] = 0;
        }
        return;
    }
#endif
    //
    ///////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////////
    //
#if 1
    if (sum == 0) {
#if defined(debug_rate_1)
        printf("Frozen pruning with rate = 1\n");
#endif
        for (int i = 0; i < size; i++) {
            int value              = argmax<gf_size>(inputs[i].value);
            symbols[symbol_id + i] = value;
            decoded[symbol_id + i] = value; // should be corrected (it is systematic solution actually)
#if defined(debug_rate_1)
            printf("-> hard decision [%2d] = %d\n", symbol_id + i, symbols[symbol_id + i]);
#endif
        }
        remove_xors(decoded + symbol_id, size);
        return;
    }
#endif
    //
    ///////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////////
    //
    //
    // REPETITION NODE !
    //
#if 1
    int rep = 0;
    if( (sum == (size-1)) && (frozen_symbols[symbol_id + (size-1)] == false) ) {
        rep = 1;
#if !defined(debug_rate_1)
        printf("Frozen pruning in REP mode [%d::%d]\n", symbol_id, size);
#endif
        // la decision dure sur size/2-1 = n-1 ???
#if 1
        for(int i = 0; i < size; i++) {
            if ( inputs[i].is_freq == false ) {
                printf("pppppppppppppppppp\n");
                show_symbols< gf_size >( inputs[i].value ); // show freq
                fwht_norm<gf_size>( inputs[i].value );
                inputs[i].is_freq = true;
                show_symbols< gf_size >( inputs[i].value ); // show freq
            }else {
                fwht_norm<gf_size>( inputs[i].value );
                normalize<gf_size>( inputs[i].value );
                printf("FFFFFFFFFFFFFFFFFFFFFF\n");
                show_symbols< gf_size >( inputs[i].value ); // show freq
                fwht_norm<gf_size>( inputs[i].value );
                show_symbols< gf_size >( inputs[i].value ); // show freq
            }

        }

        float temp[gf_size];
        for (int j = 0; j < gf_size; j++)
            temp[j] = inputs[0].value[j] * inputs[1].value[j];

        for(int i = 2; i < size; i++)
            for (int j = 0; j < gf_size; j++)
                temp[j] *= inputs[i].value[j];

        show_symbols< gf_size >( temp ); // show freq
        fwht_norm<gf_size>( temp );           // freq => proba
        normalize<gf_size>( temp );
        show_symbols< gf_size >( temp ); // show freq

#else
        if ( inputs[0].is_freq == true ) {
            for(int i = 0; i < size; i++) {
                fwht     <gf_size>( inputs[i].value );
                normalize<gf_size>( inputs[i].value );
            }
        }
        for(int i = 1; i < size; i++)
            for (int j = 0; j < gf_size; j++)
                inputs[0].value[j] += inputs[i].value[j];
        show_symbols< gf_size >( inputs[0].value );
#endif
        const int value = argmax<gf_size>( temp );

        for(int i = 0; i < size; i++)
        {
            symbols[symbol_id + i] = value;
            decoded[symbol_id + i] = 0; // should be corrected (it is systematic solution actually)
            printf("  - symbol [%d] = %2d (%f) - Un = %d\n", symbol_id + i, symbols[symbol_id + i], temp[symbols[symbol_id + i]], decoded[i + symbol_id]);
        }
        decoded[symbol_id + (size-1)] = value; // should be corrected (it is systematic solution actually)
        printf("-> hard decision [%2d] = %d\n", symbol_id + (size-1), decoded[symbol_id + (size-1)]);
#if 0
        if ( (sum == (size-1)) && (frozen_symbols[symbol_id + (size-1)] == false) ) {
            printf("> Sum : %d and Size : %d\n", sum, size);
            printf("> FrozenS : ");
            for (int i = 0; i < size; i++)
                printf("%2d ", frozen_symbols[symbol_id + i]);
            printf("\n");
            printf("> Decoded : ");
            for (int i = 0; i < size; i++)
                printf("%2d ", decoded[i]);
            printf("\n");
            printf("> Symbols : ");
            for (int i = 0; i < size; i++)
                printf("%2d ", symbols[i]);
            printf("\n");
        }
        exit( EXIT_FAILURE );
#endif
//      return;
    }
#endif
    //
    ///////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////
    //
    //
    // SINGLE PARITY NODE
    //
#if 0
    int check_node = 0;
    if( (sum == 1) && (frozen_symbols[symbol_id] == true) ) {
#define debug_rate_spc
#if defined(debug_rate_spc)
        printf("Frozen pruning in SPC mode [%d::%d]\n", symbol_id, size);
#endif

        for(int i = 0; i < size; i++)
            if ( inputs[i].is_freq == true ) {
                fwht<gf_size>( inputs[i].value );
                inputs[i].is_freq = false;
            }

        for (int i = 0; i < size; i++) {
            int value              = argmax<gf_size>(inputs[i].value);
            check_node            ^= value;
            symbols[symbol_id + i] = value;
            decoded[symbol_id + i] = value; // should be corrected (it is systematic solution actually)
        }
#if defined(debug_rate_spc)
        if ( check_node == 0 ) {
            printf("-> CN equation is validated !\n");
            remove_xors(decoded + symbol_id, size);
            return;
        } else {
            printf("-> CN equation is NOT validated !\n");
            remove_xors(decoded + symbol_id, size);
            for (int j = 0; j < size; j++) {
                printf("  - symbol [%d :: %d] (%f) - Un = %d\n", j, symbols[symbol_id + j], inputs[j].value[symbols[symbol_id + j]], decoded[j + symbol_id]);
            }
            for (int j = 0; j < size; j++)
                show_symbols< gf_size >( inputs[j].value );
        }
//      |  1  1  1  1  1  1  1  1  |  1  1  1  1  1  1  1  0  |
//      |  1  1  1  0  1  0  0  0  |  1  0  0  0  0  0  0  0  |
//      |  1  1  0  0  0  0  0  0  |  0  0  0  0  0  0  0  0  |
//      |  0  0  0  0  0  0  0  0  |  0  0  0  0  0  0  0  0  |

//          0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 53
//          0  0  0 62  0 55  8 53  0 61 41 36  5 50 24 54
//          0  0 30 23 55 21  4 61 49 59 25  3 30 19 57 54
//          1 33 51 28 30 24 39 38 17 41 27 54 34 45 37 35

#endif
    }
#endif
    //
    ///////////////////////////////////////////////////////////////////////////////////////

    //
    //
    //
    for (int i = 0; i < n; i++) {
        f_function<gf_size>(internal + i, inputs + i, inputs + n + i); // Example operation
    }
    //
    //
    //
    if (n == 1) {
        leaf_node<gf_size>(
            internal, // le symbol souple
            decoded,  // les symboles décodés (output du decodeur)
            symbols,  // le tableau des symboles durs
            symbol_id);
    } else {
        middle_node_with_pruning<gf_size>(
            internal,     // les données d'entrée
            internal + n, // la mémoire interne pour les calculs
            decoded,      // les symboles décodés (output du decodeur)
            symbols,      // les symboles durs
            n,            // le nombre de données en entrée
            symbol_id);   // l'identifiant du symbole (à gauche)
    }
    //
    //
    //
    for (int i = 0; i < n; i++) {
        g_function<gf_size>(
            internal + i,
            inputs + i,
            inputs + n + i,
            symbols[symbol_id + i]); // Example operation
    }
    //
    //
    //
    if (n == 1) {
        leaf_node<gf_size>(
            internal,
            decoded,
            symbols,
            symbol_id + n);
    } else {
        middle_node_with_pruning<gf_size>(
            internal,
            internal + n,
            decoded,
            symbols,
            n,
            symbol_id + n);
    }
    //
    //
    //
    for (int i = 0; i < n; i++) {
        symbols[symbol_id + i] ^= symbols[symbol_id + n + i];
    }
#if 1
    if ( rep ) {
        for (int i = 0; i < size; i++)
            printf("Correct symbols were [%2d] = %d\n", symbol_id + i, symbols[symbol_id + i]);
        printf("\n");
    }
#endif
    //
    //
    //
#if 0
    if ( (sum == (size-1)) && (frozen_symbols[symbol_id + (size-1)] == false) ) {
        printf("> Sum : %d and Size : %d\n", sum, size);
        printf("> FrozenS : ");
        for (int i = 0; i < size; i++)
            printf("%2d ", frozen_symbols[symbol_id + i]);
        printf("\n");
        printf("> Decoded : ");
        for (int i = 0; i < size; i++)
            printf("%2d ", decoded[i]);
        printf("\n");
        printf("> Symbols : ");
        for (int i = 0; i < size; i++)
            printf("%2d ", symbols[i]);
        printf("\n");
    }
#endif
}
//
//
//
//
//
