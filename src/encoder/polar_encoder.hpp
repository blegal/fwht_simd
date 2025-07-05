#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>

#include "encoder_1.hpp"

class polar_encoder {
public:
    polar_encoder(const int* vec, const int k, const int n) : K(k), N(n)
    {
        liste.resize( n );
        for (int i = 0; i < k; i++)
            liste[ i ] = vec[i];
    }

    void encode(uint16_t* dst, const uint16_t* src) const {

        for (int i = 0; i < K; i++)
            dst[ reliab_seq[i] ] = src[i];

        for (int i = K; i < N; i++)
            dst[ reliab_seq[i] ] = 0;

             if( N ==   16 ) polar_encode<  16>( dst );
        else if( N ==   32 ) polar_encode<  32>( dst );
        else if( N ==   64 ) polar_encode<  64>( dst );
        else if( N ==  128 ) polar_encode< 128>( dst );
        else if( N ==  256 ) polar_encode< 256>( dst );
        else if( N ==  512 ) polar_encode< 512>( dst );
        else if( N == 1024 ) polar_encode<1024>( dst );        
        else{
            printf("(EE) The encoder does not support N = %d value...\n", N);
            printf("(EE) %s %d\n", __FILE__, __LINE__);
            exit( EXIT_FAILURE );
        }
    }

    void decode(uint16_t* dst, const uint16_t* src) const {
        for (int i = 0; i < K; i++)
            dst[i] = src[ reliab_seq[i] ];
    }

private:
    const int K;
    const int N;
    std::vector<uint8_t> liste;
};
