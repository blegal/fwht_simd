#pragma once
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "types.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
template<int length> inline
void xor_processor(uint8_t dst[length], uint8_t src[length], const ap_uint<length> stride);

void v2_xor_processor  (uint8_t dst[  2], uint8_t src[  2], const ap_uint<  2> stride);
void v4_xor_processor  (uint8_t dst[  4], uint8_t src[  4], const ap_uint<  4> stride);
void v8_xor_processor  (uint8_t dst[  8], uint8_t src[  8], const ap_uint<  8> stride);
void v16_xor_processor (uint8_t dst[ 16], uint8_t src[ 16], const ap_uint< 16> stride);
void v32_xor_processor (uint8_t dst[ 32], uint8_t src[ 32], const ap_uint< 32> stride);
void v64_xor_processor (uint8_t dst[ 64], uint8_t src[ 64], const ap_uint< 64> stride);
void v128_xor_processor(uint8_t dst[128], uint8_t src[128], const ap_uint<128> stride);
void v256_xor_processor(uint8_t dst[256], uint8_t src[256], const ap_uint<256> stride);

//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//