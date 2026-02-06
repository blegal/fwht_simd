//
//
//
//////////////////////////////////////////////////////////////////////
//
//
#include "f_xor_processor.hpp"
#include <cstdlib>
//
//
////////////////////////////////////////////////////////////////////////////
//
//
template<int length>
void xor_processor(uint8_t dst[length], uint8_t src[length], const ap_uint<length> stride) {
    if( stride == 0 ) exit( EXIT_FAILURE );
    if( src[0] == 0 ) exit( EXIT_FAILURE );
    if( dst[0] == 0 ) exit( EXIT_FAILURE );    
}
//
//
template<>
void xor_processor<2>(uint8_t dst[2], uint8_t src[2], const ap_uint<2> stride) {
#pragma HLS INLINE

#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete

    dst[0] = stride[0] ? (src[0] ^ src[1]) : src[0];
    dst[1] = src[1];
}
//
//
template<>
void xor_processor<4>(uint8_t dst[4], uint8_t src[4], const ap_uint<4> stride) {
#pragma HLS INLINE

#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete

    uint8_t s1[4];
#pragma HLS ARRAY_PARTITION variable=s1 complete
    s1[0] = stride[0] ? (src[0] ^ src[1]) : src[0];
    s1[1] = src[1];
    s1[2] = stride[0] ? (src[2] ^ src[3]) : src[2];
    s1[3] = src[3];

    dst[0] = stride[1] ? (s1[0] ^ s1[2]) : s1[0];
    dst[1] = stride[1] ? (s1[1] ^ s1[3]) : s1[1];
    dst[2] = s1[2];
    dst[3] = s1[3];
}
//
//
template<>
void xor_processor<8>(uint8_t dst[8], uint8_t src[8], const ap_uint<8> stride) {
#pragma HLS INLINE

#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete

    uint8_t s1[8];
#pragma HLS ARRAY_PARTITION variable=s1 complete
    s1[0] = stride[0] ? (src[0] ^ src[1]) : src[0];
    s1[1] = src[1];
    s1[2] = stride[0] ? (src[2] ^ src[3]) : src[2];
    s1[3] = src[3];
    s1[4] = stride[0] ? (src[4] ^ src[5]) : src[4];
    s1[5] = src[5];
    s1[6] = stride[0] ? (src[6] ^ src[7]) : src[6];
    s1[7] = src[7];

    uint8_t s2[8];
#pragma HLS ARRAY_PARTITION variable=s2 complete
    s2[0] = stride[1] ? (s1[0] ^ s1[2]) : s1[0];
    s2[1] = stride[1] ? (s1[1] ^ s1[3]) : s1[1];
    s2[2] = s1[2];
    s2[3] = s1[3];
    s2[4] = stride[1] ? (s1[4] ^ s1[6]) : s1[4];
    s2[5] = stride[1] ? (s1[5] ^ s1[7]) : s1[5];
    s2[6] = s1[6];
    s2[7] = s1[7];

    dst[0] = stride[2] ? (s2[0] ^ s2[4]) : s2[0];
    dst[1] = stride[2] ? (s2[1] ^ s2[5]) : s2[1];
    dst[2] = stride[2] ? (s2[2] ^ s2[6]) : s2[2];
    dst[3] = stride[2] ? (s2[3] ^ s2[7]) : s2[3];
    dst[4] = s2[4];
    dst[5] = s2[5];
    dst[6] = s2[6];
    dst[7] = s2[7];
}
//
//
template<>
void xor_processor<16>(uint8_t dst[16], uint8_t src[16], const ap_uint<16> stride) {
#pragma HLS INLINE

#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete

    uint8_t s1[16];
#pragma HLS ARRAY_PARTITION variable=s1 complete
    s1[ 0] = stride[0] ? (src[ 0] ^ src[ 1]) : src[ 0];
    s1[ 1] = src[ 1];
    s1[ 2] = stride[0] ? (src[ 2] ^ src[ 3]) : src[ 2];
    s1[ 3] = src[ 3];
    s1[ 4] = stride[0] ? (src[ 4] ^ src[ 5]) : src[ 4];
    s1[ 5] = src[ 5];
    s1[ 6] = stride[0] ? (src[ 6] ^ src[ 7]) : src[ 6];
    s1[ 7] = src[ 7];
    s1[ 8] = stride[0] ? (src[ 8] ^ src[ 9]) : src[ 8];
    s1[ 9] = src[ 9];
    s1[10] = stride[0] ? (src[10] ^ src[11]) : src[10];
    s1[11] = src[11];
    s1[12] = stride[0] ? (src[12] ^ src[13]) : src[12];
    s1[13] = src[13];
    s1[14] = stride[0] ? (src[14] ^ src[15]) : src[14];
    s1[15] = src[15];

    uint8_t s2[16];
#pragma HLS ARRAY_PARTITION variable=s2 complete
    s2[ 0] = stride[1] ? (s1[ 0] ^ s1[ 2]) : s1[ 0];
    s2[ 1] = stride[1] ? (s1[ 1] ^ s1[ 3]) : s1[ 1];
    s2[ 2] = s1[ 2];
    s2[ 3] = s1[ 3];
    s2[ 4] = stride[1] ? (s1[ 4] ^ s1[ 6]) : s1[ 4];
    s2[ 5] = stride[1] ? (s1[ 5] ^ s1[ 7]) : s1[ 5];
    s2[ 6] = s1[ 6];
    s2[ 7] = s1[ 7];
    s2[ 8] = stride[1] ? (s1[ 8] ^ s1[10]) : s1[ 8];
    s2[ 9] = stride[1] ? (s1[ 9] ^ s1[11]) : s1[ 9];
    s2[10] = s1[10];
    s2[11] = s1[11];
    s2[12] = stride[1] ? (s1[12] ^ s1[14]) : s1[12];
    s2[13] = stride[1] ? (s1[13] ^ s1[15]) : s1[13];
    s2[14] = s1[14];
    s2[15] = s1[15];

    uint8_t s3[16];
#pragma HLS ARRAY_PARTITION variable=s3 complete
    s3[ 0] = stride[2] ? (s2[0] ^ s2[4]) : s2[0];
    s3[ 1] = stride[2] ? (s2[1] ^ s2[5]) : s2[1];
    s3[ 2] = stride[2] ? (s2[2] ^ s2[6]) : s2[2];
    s3[ 3] = stride[2] ? (s2[3] ^ s2[7]) : s2[3];
    s3[ 4] = s2[4];
    s3[ 5] = s2[5];
    s3[ 6] = s2[6];
    s3[ 7] = s2[7];
    s3[ 8] = stride[2] ? (s2[ 8] ^ s2[12]) : s2[ 8];
    s3[ 9] = stride[2] ? (s2[ 9] ^ s2[13]) : s2[ 9];
    s3[10] = stride[2] ? (s2[10] ^ s2[14]) : s2[10];
    s3[11] = stride[2] ? (s2[11] ^ s2[15]) : s2[11];
    s3[12] = s2[12];
    s3[13] = s2[13];
    s3[14] = s2[14];
    s3[15] = s2[15];

    dst[ 0] = stride[3] ? (s3[0] ^ s3[ 8]) : s3[0];
    dst[ 1] = stride[3] ? (s3[1] ^ s3[ 9]) : s3[1];
    dst[ 2] = stride[3] ? (s3[2] ^ s3[10]) : s3[2];
    dst[ 3] = stride[3] ? (s3[3] ^ s3[11]) : s3[3];
    dst[ 4] = stride[3] ? (s3[4] ^ s3[12]) : s3[4];
    dst[ 5] = stride[3] ? (s3[5] ^ s3[13]) : s3[5];
    dst[ 6] = stride[3] ? (s3[6] ^ s3[14]) : s3[6];
    dst[ 7] = stride[3] ? (s3[7] ^ s3[15]) : s3[7];
    dst[ 8] = s3[ 8];
    dst[ 9] = s3[ 9];
    dst[10] = s3[10];
    dst[11] = s3[11];
    dst[12] = s3[12];
    dst[13] = s3[13];
    dst[14] = s3[14];
    dst[15] = s3[15];
}
//
//
template<>
void xor_processor<32>(uint8_t dst[32], uint8_t src[32], const ap_uint<32> stride) {
#pragma HLS INLINE

#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete

    uint8_t s1[32];
#pragma HLS ARRAY_PARTITION variable=s1 complete
    s1[ 0] = stride[0] ? (src[ 0] ^ src[ 1]) : src[ 0];
    s1[ 1] = src[ 1];
    s1[ 2] = stride[0] ? (src[ 2] ^ src[ 3]) : src[ 2];
    s1[ 3] = src[ 3];
    s1[ 4] = stride[0] ? (src[ 4] ^ src[ 5]) : src[ 4];
    s1[ 5] = src[ 5];
    s1[ 6] = stride[0] ? (src[ 6] ^ src[ 7]) : src[ 6];
    s1[ 7] = src[ 7];
    s1[ 8] = stride[0] ? (src[ 8] ^ src[ 9]) : src[ 8];
    s1[ 9] = src[ 9];
    s1[10] = stride[0] ? (src[10] ^ src[11]) : src[10];
    s1[11] = src[11];
    s1[12] = stride[0] ? (src[12] ^ src[13]) : src[12];
    s1[13] = src[13];
    s1[14] = stride[0] ? (src[14] ^ src[15]) : src[14];
    s1[15] = src[15];
    s1[16] = stride[0] ? (src[16] ^ src[17]) : src[16];
    s1[17] = src[17];
    s1[18] = stride[0] ? (src[18] ^ src[19]) : src[18];
    s1[19] = src[19];
    s1[20] = stride[0] ? (src[20] ^ src[21]) : src[20];
    s1[21] = src[21];
    s1[22] = stride[0] ? (src[22] ^ src[23]) : src[22];
    s1[23] = src[23];
    s1[24] = stride[0] ? (src[24] ^ src[25]) : src[24];
    s1[25] = src[25];
    s1[26] = stride[0] ? (src[26] ^ src[27]) : src[26];
    s1[27] = src[27];
    s1[28] = stride[0] ? (src[28] ^ src[29]) : src[28];
    s1[29] = src[29];
    s1[30] = stride[0] ? (src[30] ^ src[31]) : src[30];
    s1[31] = src[31];

    uint8_t s2[32];
#pragma HLS ARRAY_PARTITION variable=s2 complete
    s2[ 0] = stride[1] ? (s1[ 0] ^ s1[ 2]) : s1[ 0];
    s2[ 1] = stride[1] ? (s1[ 1] ^ s1[ 3]) : s1[ 1];
    s2[ 2] = s1[ 2];
    s2[ 3] = s1[ 3];
    s2[ 4] = stride[1] ? (s1[ 4] ^ s1[ 6]) : s1[ 4];
    s2[ 5] = stride[1] ? (s1[ 5] ^ s1[ 7]) : s1[ 5];
    s2[ 6] = s1[ 6];
    s2[ 7] = s1[ 7];
    s2[ 8] = stride[1] ? (s1[ 8] ^ s1[10]) : s1[ 8];
    s2[ 9] = stride[1] ? (s1[ 9] ^ s1[11]) : s1[ 9];
    s2[10] = s1[10];
    s2[11] = s1[11];
    s2[12] = stride[1] ? (s1[12] ^ s1[14]) : s1[12];
    s2[13] = stride[1] ? (s1[13] ^ s1[15]) : s1[13];
    s2[14] = s1[14];
    s2[15] = s1[15];
    s2[16] = stride[1] ? (s1[16] ^ s1[18]) : s1[16];
    s2[17] = stride[1] ? (s1[17] ^ s1[19]) : s1[17];
    s2[18] = s1[18];
    s2[19] = s1[19];
    s2[20] = stride[1] ? (s1[20] ^ s1[22]) : s1[20];
    s2[21] = stride[1] ? (s1[21] ^ s1[23]) : s1[21];
    s2[22] = s1[22];
    s2[23] = s1[23];
    s2[24] = stride[1] ? (s1[24] ^ s1[26]) : s1[24];
    s2[25] = stride[1] ? (s1[25] ^ s1[27]) : s1[25];
    s2[26] = s1[26];
    s2[27] = s1[27];
    s2[28] = stride[1] ? (s1[28] ^ s1[30]) : s1[28];
    s2[29] = stride[1] ? (s1[29] ^ s1[31]) : s1[29];
    s2[30] = s1[30];
    s2[31] = s1[31];

    uint8_t s3[32];
#pragma HLS ARRAY_PARTITION variable=s3 complete
    s3[ 0] = stride[2] ? (s2[0] ^ s2[4]) : s2[0];
    s3[ 1] = stride[2] ? (s2[1] ^ s2[5]) : s2[1];
    s3[ 2] = stride[2] ? (s2[2] ^ s2[6]) : s2[2];
    s3[ 3] = stride[2] ? (s2[3] ^ s2[7]) : s2[3];
    s3[ 4] = s2[4];
    s3[ 5] = s2[5];
    s3[ 6] = s2[6];
    s3[ 7] = s2[7];
    s3[ 8] = stride[2] ? (s2[ 8] ^ s2[12]) : s2[ 8];
    s3[ 9] = stride[2] ? (s2[ 9] ^ s2[13]) : s2[ 9];
    s3[10] = stride[2] ? (s2[10] ^ s2[14]) : s2[10];
    s3[11] = stride[2] ? (s2[11] ^ s2[15]) : s2[11];
    s3[12] = s2[12];
    s3[13] = s2[13];
    s3[14] = s2[14];
    s3[15] = s2[15];
    s3[16] = stride[2] ? (s2[16] ^ s2[20]) : s2[16];
    s3[17] = stride[2] ? (s2[17] ^ s2[21]) : s2[17];
    s3[18] = stride[2] ? (s2[18] ^ s2[22]) : s2[18];
    s3[19] = stride[2] ? (s2[19] ^ s2[23]) : s2[19];
    s3[20] = s2[20];
    s3[21] = s2[21];
    s3[22] = s2[22];
    s3[23] = s2[23];
    s3[24] = stride[2] ? (s2[24] ^ s2[28]) : s2[24];
    s3[25] = stride[2] ? (s2[25] ^ s2[29]) : s2[25];
    s3[26] = stride[2] ? (s2[26] ^ s2[30]) : s2[26];
    s3[27] = stride[2] ? (s2[27] ^ s2[31]) : s2[27];
    s3[28] = s2[28];
    s3[29] = s2[29];
    s3[30] = s2[30];
    s3[31] = s2[31];

    uint8_t s4[32];
#pragma HLS ARRAY_PARTITION variable=s4 complete
    s4[ 0] = stride[3] ? (s3[ 0] ^ s3[ 8]) : s3[ 0];
    s4[ 1] = stride[3] ? (s3[ 1] ^ s3[ 9]) : s3[ 1];
    s4[ 2] = stride[3] ? (s3[ 2] ^ s3[10]) : s3[ 2];
    s4[ 3] = stride[3] ? (s3[ 3] ^ s3[11]) : s3[ 3];
    s4[ 4] = stride[3] ? (s3[ 4] ^ s3[12]) : s3[ 4];
    s4[ 5] = stride[3] ? (s3[ 5] ^ s3[13]) : s3[ 5];
    s4[ 6] = stride[3] ? (s3[ 6] ^ s3[14]) : s3[ 6];
    s4[ 7] = stride[3] ? (s3[ 7] ^ s3[15]) : s3[ 7];
    s4[ 8] = s3[ 8];
    s4[ 9] = s3[ 9];
    s4[10] = s3[10];
    s4[11] = s3[11];
    s4[12] = s3[12];
    s4[13] = s3[13];
    s4[14] = s3[14];
    s4[15] = s3[15];
    s4[16] = stride[3] ? (s3[16] ^ s3[24]) : s3[16];
    s4[17] = stride[3] ? (s3[17] ^ s3[25]) : s3[17];
    s4[18] = stride[3] ? (s3[18] ^ s3[26]) : s3[18];
    s4[19] = stride[3] ? (s3[19] ^ s3[27]) : s3[19];
    s4[20] = stride[3] ? (s3[20] ^ s3[28]) : s3[20];
    s4[21] = stride[3] ? (s3[21] ^ s3[29]) : s3[21];
    s4[22] = stride[3] ? (s3[22] ^ s3[30]) : s3[22];
    s4[23] = stride[3] ? (s3[23] ^ s3[31]) : s3[23];
    s4[24] = s3[24];
    s4[25] = s3[25];
    s4[26] = s3[26];
    s4[27] = s3[27];
    s4[28] = s3[28];
    s4[29] = s3[29];
    s4[30] = s3[30];
    s4[31] = s3[31];

    dst[ 0] = stride[4] ? (s4[ 0] ^ s4[16]) : s4[ 0];
    dst[ 1] = stride[4] ? (s4[ 1] ^ s4[17]) : s4[ 1];
    dst[ 2] = stride[4] ? (s4[ 2] ^ s4[18]) : s4[ 2];
    dst[ 3] = stride[4] ? (s4[ 3] ^ s4[19]) : s4[ 3];
    dst[ 4] = stride[4] ? (s4[ 4] ^ s4[20]) : s4[ 4];
    dst[ 5] = stride[4] ? (s4[ 5] ^ s4[21]) : s4[ 5];
    dst[ 6] = stride[4] ? (s4[ 6] ^ s4[22]) : s4[ 6];
    dst[ 7] = stride[4] ? (s4[ 7] ^ s4[23]) : s4[ 7];
    dst[ 8] = stride[4] ? (s4[ 8] ^ s4[24]) : s4[ 8];
    dst[ 9] = stride[4] ? (s4[ 9] ^ s4[25]) : s4[ 9];
    dst[10] = stride[4] ? (s4[10] ^ s4[26]) : s4[10];
    dst[11] = stride[4] ? (s4[11] ^ s4[27]) : s4[11];
    dst[12] = stride[4] ? (s4[12] ^ s4[28]) : s4[12];
    dst[13] = stride[4] ? (s4[13] ^ s4[29]) : s4[13];
    dst[14] = stride[4] ? (s4[14] ^ s4[30]) : s4[14];
    dst[15] = stride[4] ? (s4[15] ^ s4[31]) : s4[15];
    dst[16] = s4[16];
    dst[17] = s4[17];
    dst[18] = s4[18];
    dst[19] = s4[19];
    dst[20] = s4[20];
    dst[21] = s4[21];
    dst[22] = s4[22];
    dst[23] = s4[23];
    dst[24] = s4[24];
    dst[25] = s4[25];
    dst[26] = s4[26];
    dst[27] = s4[27];
    dst[28] = s4[28];
    dst[29] = s4[29];
    dst[30] = s4[30];
    dst[31] = s4[31];
}
//
//
//
//
template<>
void xor_processor<64>(uint8_t dst[64], uint8_t src[64], const ap_uint<64> stride) {
#pragma HLS INLINE

#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete

    uint8_t s1[64];
#pragma HLS ARRAY_PARTITION variable=s1 complete
    s1[ 0] = stride[0] ? (src[ 0] ^ src[ 1]) : src[ 0];
    s1[ 1] = src[ 1];
    s1[ 2] = stride[0] ? (src[ 2] ^ src[ 3]) : src[ 2];
    s1[ 3] = src[ 3];
    s1[ 4] = stride[0] ? (src[ 4] ^ src[ 5]) : src[ 4];
    s1[ 5] = src[ 5];
    s1[ 6] = stride[0] ? (src[ 6] ^ src[ 7]) : src[ 6];
    s1[ 7] = src[ 7];
    s1[ 8] = stride[0] ? (src[ 8] ^ src[ 9]) : src[ 8];
    s1[ 9] = src[ 9];
    s1[10] = stride[0] ? (src[10] ^ src[11]) : src[10];
    s1[11] = src[11];
    s1[12] = stride[0] ? (src[12] ^ src[13]) : src[12];
    s1[13] = src[13];
    s1[14] = stride[0] ? (src[14] ^ src[15]) : src[14];
    s1[15] = src[15];
    s1[16] = stride[0] ? (src[16] ^ src[17]) : src[16];
    s1[17] = src[17];
    s1[18] = stride[0] ? (src[18] ^ src[19]) : src[18];
    s1[19] = src[19];
    s1[20] = stride[0] ? (src[20] ^ src[21]) : src[20];
    s1[21] = src[21];
    s1[22] = stride[0] ? (src[22] ^ src[23]) : src[22];
    s1[23] = src[23];
    s1[24] = stride[0] ? (src[24] ^ src[25]) : src[24];
    s1[25] = src[25];
    s1[26] = stride[0] ? (src[26] ^ src[27]) : src[26];
    s1[27] = src[27];
    s1[28] = stride[0] ? (src[28] ^ src[29]) : src[28];
    s1[29] = src[29];
    s1[30] = stride[0] ? (src[30] ^ src[31]) : src[30];
    s1[31] = src[31];
    s1[32] = stride[0] ? (src[32] ^ src[33]) : src[32];
    s1[33] = src[33];
    s1[34] = stride[0] ? (src[34] ^ src[35]) : src[34];
    s1[35] = src[35];
    s1[36] = stride[0] ? (src[36] ^ src[37]) : src[36];
    s1[37] = src[37];
    s1[38] = stride[0] ? (src[38] ^ src[39]) : src[38];
    s1[39] = src[39];
    s1[40] = stride[0] ? (src[40] ^ src[41]) : src[40];
    s1[41] = src[41];
    s1[42] = stride[0] ? (src[42] ^ src[43]) : src[42];
    s1[43] = src[43];
    s1[44] = stride[0] ? (src[44] ^ src[45]) : src[44];
    s1[45] = src[45];
    s1[46] = stride[0] ? (src[46] ^ src[47]) : src[46];
    s1[47] = src[47];
    s1[48] = stride[0] ? (src[48] ^ src[49]) : src[48];
    s1[49] = src[49];
    s1[50] = stride[0] ? (src[50] ^ src[51]) : src[50];
    s1[51] = src[51];
    s1[52] = stride[0] ? (src[52] ^ src[53]) : src[52];
    s1[53] = src[53];
    s1[54] = stride[0] ? (src[54] ^ src[55]) : src[54];
    s1[55] = src[55];
    s1[56] = stride[0] ? (src[56] ^ src[57]) : src[56];
    s1[57] = src[57];
    s1[58] = stride[0] ? (src[58] ^ src[59]) : src[58];
    s1[59] = src[59];
    s1[60] = stride[0] ? (src[60] ^ src[61]) : src[60];
    s1[61] = src[61];
    s1[62] = stride[0] ? (src[62] ^ src[63]) : src[62];
    s1[63] = src[63];

    uint8_t s2[64];
#pragma HLS ARRAY_PARTITION variable=s2 complete
    s2[ 0] = stride[1] ? (s1[ 0] ^ s1[ 2]) : s1[ 0];
    s2[ 1] = stride[1] ? (s1[ 1] ^ s1[ 3]) : s1[ 1];
    s2[ 2] = s1[ 2];
    s2[ 3] = s1[ 3];
    s2[ 4] = stride[1] ? (s1[ 4] ^ s1[ 6]) : s1[ 4];
    s2[ 5] = stride[1] ? (s1[ 5] ^ s1[ 7]) : s1[ 5];
    s2[ 6] = s1[ 6];
    s2[ 7] = s1[ 7];
    s2[ 8] = stride[1] ? (s1[ 8] ^ s1[10]) : s1[ 8];
    s2[ 9] = stride[1] ? (s1[ 9] ^ s1[11]) : s1[ 9];
    s2[10] = s1[10];
    s2[11] = s1[11];
    s2[12] = stride[1] ? (s1[12] ^ s1[14]) : s1[12];
    s2[13] = stride[1] ? (s1[13] ^ s1[15]) : s1[13];
    s2[14] = s1[14];
    s2[15] = s1[15];
    s2[16] = stride[1] ? (s1[16] ^ s1[18]) : s1[16];
    s2[17] = stride[1] ? (s1[17] ^ s1[19]) : s1[17];
    s2[18] = s1[18];
    s2[19] = s1[19];
    s2[20] = stride[1] ? (s1[20] ^ s1[22]) : s1[20];
    s2[21] = stride[1] ? (s1[21] ^ s1[23]) : s1[21];
    s2[22] = s1[22];
    s2[23] = s1[23];
    s2[24] = stride[1] ? (s1[24] ^ s1[26]) : s1[24];
    s2[25] = stride[1] ? (s1[25] ^ s1[27]) : s1[25];
    s2[26] = s1[26];
    s2[27] = s1[27];
    s2[28] = stride[1] ? (s1[28] ^ s1[30]) : s1[28];
    s2[29] = stride[1] ? (s1[29] ^ s1[31]) : s1[29];
    s2[30] = s1[30];
    s2[31] = s1[31];
    s2[32] = stride[1] ? (s1[32] ^ s1[34]) : s1[32];
    s2[33] = stride[1] ? (s1[33] ^ s1[35]) : s1[33];
    s2[34] = s1[34];
    s2[35] = s1[35];
    s2[36] = stride[1] ? (s1[36] ^ s1[38]) : s1[36];
    s2[37] = stride[1] ? (s1[37] ^ s1[39]) : s1[37];
    s2[38] = s1[38];
    s2[39] = s1[39];
    s2[40] = stride[1] ? (s1[40] ^ s1[42]) : s1[40];
    s2[41] = stride[1] ? (s1[41] ^ s1[43]) : s1[41];
    s2[42] = s1[42];
    s2[43] = s1[43];
    s2[44] = stride[1] ? (s1[44] ^ s1[46]) : s1[44];
    s2[45] = stride[1] ? (s1[45] ^ s1[47]) : s1[45];
    s2[46] = s1[46];
    s2[47] = s1[47];
    s2[48] = stride[1] ? (s1[48] ^ s1[50]) : s1[48];
    s2[49] = stride[1] ? (s1[49] ^ s1[51]) : s1[49];
    s2[50] = s1[50];
    s2[51] = s1[51];
    s2[52] = stride[1] ? (s1[52] ^ s1[54]) : s1[52];
    s2[53] = stride[1] ? (s1[53] ^ s1[55]) : s1[53];
    s2[54] = s1[54];
    s2[55] = s1[55];
    s2[56] = stride[1] ? (s1[56] ^ s1[58]) : s1[56];
    s2[57] = stride[1] ? (s1[57] ^ s1[59]) : s1[57];
    s2[58] = s1[58];
    s2[59] = s1[59];
    s2[60] = stride[1] ? (s1[60] ^ s1[62]) : s1[60];
    s2[61] = stride[1] ? (s1[61] ^ s1[63]) : s1[61];
    s2[62] = s1[62];
    s2[63] = s1[63];

    uint8_t s3[64];
#pragma HLS ARRAY_PARTITION variable=s3 complete
    s3[ 0] = stride[2] ? (s2[0] ^ s2[4]) : s2[0];
    s3[ 1] = stride[2] ? (s2[1] ^ s2[5]) : s2[1];
    s3[ 2] = stride[2] ? (s2[2] ^ s2[6]) : s2[2];
    s3[ 3] = stride[2] ? (s2[3] ^ s2[7]) : s2[3];
    s3[ 4] = s2[4];
    s3[ 5] = s2[5];
    s3[ 6] = s2[6];
    s3[ 7] = s2[7];
    s3[ 8] = stride[2] ? (s2[ 8] ^ s2[12]) : s2[ 8];
    s3[ 9] = stride[2] ? (s2[ 9] ^ s2[13]) : s2[ 9];
    s3[10] = stride[2] ? (s2[10] ^ s2[14]) : s2[10];
    s3[11] = stride[2] ? (s2[11] ^ s2[15]) : s2[11];
    s3[12] = s2[12];
    s3[13] = s2[13];
    s3[14] = s2[14];
    s3[15] = s2[15];
    s3[16] = stride[2] ? (s2[16] ^ s2[20]) : s2[16];
    s3[17] = stride[2] ? (s2[17] ^ s2[21]) : s2[17];
    s3[18] = stride[2] ? (s2[18] ^ s2[22]) : s2[18];
    s3[19] = stride[2] ? (s2[19] ^ s2[23]) : s2[19];
    s3[20] = s2[20];
    s3[21] = s2[21];
    s3[22] = s2[22];
    s3[23] = s2[23];
    s3[24] = stride[2] ? (s2[24] ^ s2[28]) : s2[24];
    s3[25] = stride[2] ? (s2[25] ^ s2[29]) : s2[25];
    s3[26] = stride[2] ? (s2[26] ^ s2[30]) : s2[26];
    s3[27] = stride[2] ? (s2[27] ^ s2[31]) : s2[27];
    s3[28] = s2[28];
    s3[29] = s2[29];
    s3[30] = s2[30];
    s3[31] = s2[31];
    s3[32] = stride[2] ? (s2[32] ^ s2[36]) : s2[32];
    s3[33] = stride[2] ? (s2[33] ^ s2[37]) : s2[33];
    s3[34] = stride[2] ? (s2[34] ^ s2[38]) : s2[34];
    s3[35] = stride[2] ? (s2[35] ^ s2[39]) : s2[35];
    s3[36] = s2[36];
    s3[37] = s2[37];
    s3[38] = s2[38];
    s3[39] = s2[39];
    s3[40] = stride[2] ? (s2[40] ^ s2[44]) : s2[40];
    s3[41] = stride[2] ? (s2[41] ^ s2[45]) : s2[41];
    s3[42] = stride[2] ? (s2[42] ^ s2[46]) : s2[42];
    s3[43] = stride[2] ? (s2[43] ^ s2[47]) : s2[43];
    s3[44] = s2[44];
    s3[45] = s2[45];
    s3[46] = s2[46];
    s3[47] = s2[47];
    s3[48] = stride[2] ? (s2[48] ^ s2[52]) : s2[48];
    s3[49] = stride[2] ? (s2[49] ^ s2[53]) : s2[49];
    s3[50] = stride[2] ? (s2[50] ^ s2[54]) : s2[50];
    s3[51] = stride[2] ? (s2[51] ^ s2[55]) : s2[51];
    s3[52] = s2[52];
    s3[53] = s2[53];
    s3[54] = s2[54];
    s3[55] = s2[55];
    s3[56] = stride[2] ? (s2[56] ^ s2[60]) : s2[56];
    s3[57] = stride[2] ? (s2[57] ^ s2[61]) : s2[57];
    s3[58] = stride[2] ? (s2[58] ^ s2[62]) : s2[58];
    s3[59] = stride[2] ? (s2[59] ^ s2[63]) : s2[59];
    s3[60] = s2[60];
    s3[61] = s2[61];
    s3[62] = s2[62];
    s3[63] = s2[63];

    uint8_t s4[64];
#pragma HLS ARRAY_PARTITION variable=s4 complete
    s4[ 0] = stride[3] ? (s3[ 0] ^ s3[ 8]) : s3[ 0];
    s4[ 1] = stride[3] ? (s3[ 1] ^ s3[ 9]) : s3[ 1];
    s4[ 2] = stride[3] ? (s3[ 2] ^ s3[10]) : s3[ 2];
    s4[ 3] = stride[3] ? (s3[ 3] ^ s3[11]) : s3[ 3];
    s4[ 4] = stride[3] ? (s3[ 4] ^ s3[12]) : s3[ 4];
    s4[ 5] = stride[3] ? (s3[ 5] ^ s3[13]) : s3[ 5];
    s4[ 6] = stride[3] ? (s3[ 6] ^ s3[14]) : s3[ 6];
    s4[ 7] = stride[3] ? (s3[ 7] ^ s3[15]) : s3[ 7];
    s4[ 8] = s3[ 8];
    s4[ 9] = s3[ 9];
    s4[10] = s3[10];
    s4[11] = s3[11];
    s4[12] = s3[12];
    s4[13] = s3[13];
    s4[14] = s3[14];
    s4[15] = s3[15];
    s4[16] = stride[3] ? (s3[16] ^ s3[24]) : s3[16];
    s4[17] = stride[3] ? (s3[17] ^ s3[25]) : s3[17];
    s4[18] = stride[3] ? (s3[18] ^ s3[26]) : s3[18];
    s4[19] = stride[3] ? (s3[19] ^ s3[27]) : s3[19];
    s4[20] = stride[3] ? (s3[20] ^ s3[28]) : s3[20];
    s4[21] = stride[3] ? (s3[21] ^ s3[29]) : s3[21];
    s4[22] = stride[3] ? (s3[22] ^ s3[30]) : s3[22];
    s4[23] = stride[3] ? (s3[23] ^ s3[31]) : s3[23];
    s4[24] = s3[24];
    s4[25] = s3[25];
    s4[26] = s3[26];
    s4[27] = s3[27];
    s4[28] = s3[28];
    s4[29] = s3[29];
    s4[30] = s3[30];
    s4[31] = s3[31];
    s4[32] = stride[3] ? (s3[32] ^ s3[40]) : s3[32];
    s4[33] = stride[3] ? (s3[33] ^ s3[41]) : s3[33];
    s4[34] = stride[3] ? (s3[34] ^ s3[42]) : s3[34];
    s4[35] = stride[3] ? (s3[35] ^ s3[43]) : s3[35];
    s4[36] = stride[3] ? (s3[36] ^ s3[44]) : s3[36];
    s4[37] = stride[3] ? (s3[37] ^ s3[45]) : s3[37];
    s4[38] = stride[3] ? (s3[38] ^ s3[46]) : s3[38];
    s4[39] = stride[3] ? (s3[39] ^ s3[47]) : s3[39];
    s4[40] = s3[40];
    s4[41] = s3[41];
    s4[42] = s3[42];
    s4[43] = s3[43];
    s4[44] = s3[44];
    s4[45] = s3[45];
    s4[46] = s3[46];
    s4[47] = s3[47];
    s4[48] = stride[3] ? (s3[48] ^ s3[56]) : s3[48];
    s4[49] = stride[3] ? (s3[49] ^ s3[57]) : s3[49];
    s4[50] = stride[3] ? (s3[50] ^ s3[58]) : s3[50];
    s4[51] = stride[3] ? (s3[51] ^ s3[59]) : s3[51];
    s4[52] = stride[3] ? (s3[52] ^ s3[60]) : s3[52];
    s4[53] = stride[3] ? (s3[53] ^ s3[61]) : s3[53];
    s4[54] = stride[3] ? (s3[54] ^ s3[62]) : s3[54];
    s4[55] = stride[3] ? (s3[55] ^ s3[63]) : s3[55];
    s4[56] = s3[56];
    s4[57] = s3[57];
    s4[58] = s3[58];
    s4[59] = s3[59];
    s4[60] = s3[60];
    s4[61] = s3[61];
    s4[62] = s3[62];
    s4[63] = s3[63];

    uint8_t s5[64];
#pragma HLS ARRAY_PARTITION variable=s5 complete
    s5[ 0] = stride[4] ? (s4[ 0] ^ s4[16]) : s4[ 0];
    s5[ 1] = stride[4] ? (s4[ 1] ^ s4[17]) : s4[ 1];
    s5[ 2] = stride[4] ? (s4[ 2] ^ s4[18]) : s4[ 2];
    s5[ 3] = stride[4] ? (s4[ 3] ^ s4[19]) : s4[ 3];
    s5[ 4] = stride[4] ? (s4[ 4] ^ s4[20]) : s4[ 4];
    s5[ 5] = stride[4] ? (s4[ 5] ^ s4[21]) : s4[ 5];
    s5[ 6] = stride[4] ? (s4[ 6] ^ s4[22]) : s4[ 6];
    s5[ 7] = stride[4] ? (s4[ 7] ^ s4[23]) : s4[ 7];
    s5[ 8] = stride[4] ? (s4[ 8] ^ s4[24]) : s4[ 8];
    s5[ 9] = stride[4] ? (s4[ 9] ^ s4[25]) : s4[ 9];
    s5[10] = stride[4] ? (s4[10] ^ s4[26]) : s4[10];
    s5[11] = stride[4] ? (s4[11] ^ s4[27]) : s4[11];
    s5[12] = stride[4] ? (s4[12] ^ s4[28]) : s4[12];
    s5[13] = stride[4] ? (s4[13] ^ s4[29]) : s4[13];
    s5[14] = stride[4] ? (s4[14] ^ s4[30]) : s4[14];
    s5[15] = stride[4] ? (s4[15] ^ s4[31]) : s4[15];
    s5[16] = s4[16];
    s5[17] = s4[17];
    s5[18] = s4[18];
    s5[19] = s4[19];
    s5[20] = s4[20];
    s5[21] = s4[21];
    s5[22] = s4[22];
    s5[23] = s4[23];
    s5[24] = s4[24];
    s5[25] = s4[25];
    s5[26] = s4[26];
    s5[27] = s4[27];
    s5[28] = s4[28];
    s5[29] = s4[29];
    s5[30] = s4[30];
    s5[31] = s4[31];
    s5[32] = stride[4] ? (s4[32] ^ s4[48]) : s4[32];
    s5[33] = stride[4] ? (s4[33] ^ s4[49]) : s4[33];
    s5[34] = stride[4] ? (s4[34] ^ s4[50]) : s4[34];
    s5[35] = stride[4] ? (s4[35] ^ s4[51]) : s4[35];
    s5[36] = stride[4] ? (s4[36] ^ s4[52]) : s4[36];
    s5[37] = stride[4] ? (s4[37] ^ s4[53]) : s4[37];
    s5[38] = stride[4] ? (s4[38] ^ s4[54]) : s4[38];
    s5[39] = stride[4] ? (s4[39] ^ s4[55]) : s4[39];
    s5[40] = stride[4] ? (s4[40] ^ s4[56]) : s4[40];
    s5[41] = stride[4] ? (s4[41] ^ s4[57]) : s4[41];
    s5[42] = stride[4] ? (s4[42] ^ s4[58]) : s4[42];
    s5[43] = stride[4] ? (s4[43] ^ s4[59]) : s4[43];
    s5[44] = stride[4] ? (s4[44] ^ s4[60]) : s4[44];
    s5[45] = stride[4] ? (s4[45] ^ s4[61]) : s4[45];
    s5[46] = stride[4] ? (s4[46] ^ s4[62]) : s4[46];
    s5[47] = stride[4] ? (s4[47] ^ s4[63]) : s4[47];
    s5[48] = s4[48];
    s5[49] = s4[49];
    s5[50] = s4[50];
    s5[51] = s4[51];
    s5[52] = s4[52];
    s5[53] = s4[53];
    s5[54] = s4[54];
    s5[55] = s4[55];
    s5[56] = s4[56];
    s5[57] = s4[57];
    s5[58] = s4[58];
    s5[59] = s4[59];
    s5[60] = s4[60];
    s5[61] = s4[61];
    s5[62] = s4[62];
    s5[63] = s4[63];

    dst[ 0] = stride[5] ? (s5[ 0] ^ s5[32]) : s5[ 0];
    dst[ 1] = stride[5] ? (s5[ 1] ^ s5[33]) : s5[ 1];
    dst[ 2] = stride[5] ? (s5[ 2] ^ s5[34]) : s5[ 2];
    dst[ 3] = stride[5] ? (s5[ 3] ^ s5[35]) : s5[ 3];
    dst[ 4] = stride[5] ? (s5[ 4] ^ s5[36]) : s5[ 4];
    dst[ 5] = stride[5] ? (s5[ 5] ^ s5[37]) : s5[ 5];
    dst[ 6] = stride[5] ? (s5[ 6] ^ s5[38]) : s5[ 6];
    dst[ 7] = stride[5] ? (s5[ 7] ^ s5[39]) : s5[ 7];
    dst[ 8] = stride[5] ? (s5[ 8] ^ s5[40]) : s5[ 8];
    dst[ 9] = stride[5] ? (s5[ 9] ^ s5[41]) : s5[ 9];
    dst[10] = stride[5] ? (s5[10] ^ s5[42]) : s5[10];
    dst[11] = stride[5] ? (s5[11] ^ s5[43]) : s5[11];
    dst[12] = stride[5] ? (s5[12] ^ s5[44]) : s5[12];
    dst[13] = stride[5] ? (s5[13] ^ s5[45]) : s5[13];
    dst[14] = stride[5] ? (s5[14] ^ s5[46]) : s5[14];
    dst[15] = stride[5] ? (s5[15] ^ s5[47]) : s5[15];
    dst[16] = stride[5] ? (s5[16] ^ s5[48]) : s5[16];
    dst[17] = stride[5] ? (s5[17] ^ s5[49]) : s5[17];
    dst[18] = stride[5] ? (s5[18] ^ s5[50]) : s5[18];
    dst[19] = stride[5] ? (s5[19] ^ s5[51]) : s5[19];
    dst[20] = stride[5] ? (s5[20] ^ s5[52]) : s5[20];
    dst[21] = stride[5] ? (s5[21] ^ s5[53]) : s5[21];
    dst[22] = stride[5] ? (s5[22] ^ s5[54]) : s5[22];
    dst[23] = stride[5] ? (s5[23] ^ s5[55]) : s5[23];
    dst[24] = stride[5] ? (s5[24] ^ s5[56]) : s5[24];
    dst[25] = stride[5] ? (s5[25] ^ s5[57]) : s5[25];
    dst[26] = stride[5] ? (s5[26] ^ s5[58]) : s5[26];
    dst[27] = stride[5] ? (s5[27] ^ s5[59]) : s5[27];
    dst[28] = stride[5] ? (s5[28] ^ s5[60]) : s5[28];
    dst[29] = stride[5] ? (s5[29] ^ s5[61]) : s5[29];
    dst[30] = stride[5] ? (s5[30] ^ s5[62]) : s5[30];
    dst[31] = stride[5] ? (s5[31] ^ s5[63]) : s5[31];
    dst[32] = s5[32];
    dst[33] = s5[33];
    dst[34] = s5[34];
    dst[35] = s5[35];
    dst[36] = s5[36];
    dst[37] = s5[37];
    dst[38] = s5[38];
    dst[39] = s5[39];
    dst[40] = s5[40];
    dst[41] = s5[41];
    dst[42] = s5[42];
    dst[43] = s5[43];
    dst[44] = s5[44];
    dst[45] = s5[45];
    dst[46] = s5[46];
    dst[47] = s5[47];
    dst[48] = s5[48];
    dst[49] = s5[49];
    dst[50] = s5[50];
    dst[51] = s5[51];
    dst[52] = s5[52];
    dst[53] = s5[53];
    dst[54] = s5[54];
    dst[55] = s5[55];
    dst[56] = s5[56];
    dst[57] = s5[57];
    dst[58] = s5[58];
    dst[59] = s5[59];
    dst[60] = s5[60];
    dst[61] = s5[61];
    dst[62] = s5[62];
    dst[63] = s5[63];
}
//
//
////////////////////////////////////////////////////////////////////////////
//
//
template<>
void xor_processor<128>(uint8_t dst[128], uint8_t src[128], const ap_uint<128> stride)
{
#pragma HLS INLINE
    uint8_t s1[128];
    uint8_t s2[128];
    uint8_t s3[128];
    uint8_t s4[128];
    uint8_t s5[128];
    uint8_t s6[128];

#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete

#pragma HLS ARRAY_PARTITION variable=s1 complete
#pragma HLS ARRAY_PARTITION variable=s2 complete
#pragma HLS ARRAY_PARTITION variable=s3 complete
#pragma HLS ARRAY_PARTITION variable=s4 complete
#pragma HLS ARRAY_PARTITION variable=s5 complete
#pragma HLS ARRAY_PARTITION variable=s6 complete

    /* ===================== s1 : pas = 1 ===================== */
    for (int i = 0; i < 128; i += 2) {
#pragma HLS UNROLL
        s1[i]   = stride[0] ? (src[i] ^ src[i+1]) : src[i];
        s1[i+1] = src[i+1];
    }

    /* ===================== s2 : pas = 2 ===================== */
    for (int i = 0; i < 128; i += 4) {
#pragma HLS UNROLL
        s2[i+0] = stride[1] ? (s1[i+0] ^ s1[i+2]) : s1[i+0];
        s2[i+1] = stride[1] ? (s1[i+1] ^ s1[i+3]) : s1[i+1];
        s2[i+2] = s1[i+2];
        s2[i+3] = s1[i+3];
    }

    /* ===================== s3 : pas = 4 ===================== */
    for (int i = 0; i < 128; i += 8) {
#pragma HLS UNROLL
        s3[i+0] = stride[2] ? (s2[i+0] ^ s2[i+4]) : s2[i+0];
        s3[i+1] = stride[2] ? (s2[i+1] ^ s2[i+5]) : s2[i+1];
        s3[i+2] = stride[2] ? (s2[i+2] ^ s2[i+6]) : s2[i+2];
        s3[i+3] = stride[2] ? (s2[i+3] ^ s2[i+7]) : s2[i+3];
        s3[i+4] = s2[i+4];
        s3[i+5] = s2[i+5];
        s3[i+6] = s2[i+6];
        s3[i+7] = s2[i+7];
    }

    /* ===================== s4 : pas = 8 ===================== */
    for (int i = 0; i < 128; i += 16) {
#pragma HLS UNROLL
        for (int j = 0; j < 8; j++) {
#pragma HLS UNROLL
            s4[i+j] = stride[3] ? (s3[i+j] ^ s3[i+j+8]) : s3[i+j];
        }
        for (int j = 8; j < 16; j++) {
#pragma HLS UNROLL
            s4[i+j] = s3[i+j];
        }
    }

    /* ===================== s5 : pas = 16 ===================== */
    for (int i = 0; i < 128; i += 32) {
#pragma HLS UNROLL
        for (int j = 0; j < 16; j++) {
#pragma HLS UNROLL
            s5[i+j] = stride[4] ? (s4[i+j] ^ s4[i+j+16]) : s4[i+j];
        }
        for (int j = 16; j < 32; j++) {
#pragma HLS UNROLL
            s5[i+j] = s4[i+j];
        }
    }

    /* ===================== s6 : pas = 32 ===================== */
    for (int i = 0; i < 128; i += 64) {
#pragma HLS UNROLL
        for (int j = 0; j < 32; j++) {
#pragma HLS UNROLL
            s6[i+j] = stride[5] ? (s5[i+j] ^ s5[i+j+32]) : s5[i+j];
        }
        for (int j = 32; j < 64; j++) {
#pragma HLS UNROLL
            s6[i+j] = s5[i+j];
        }
    }

    /* ===================== dst : pas = 64 ===================== */
    for (int i = 0; i < 64; i++) {
#pragma HLS UNROLL
        dst[i] = stride[6] ? (s6[i] ^ s6[i+64]) : s6[i];
    }
    for (int i = 64; i < 128; i++) {
#pragma HLS UNROLL
        dst[i] = s6[i];
    }
}
//
//
////////////////////////////////////////////////////////////////////////////
//
//
template<>
void xor_processor<256>(uint8_t dst[256], uint8_t src[256], const ap_uint<256> stride) {
#pragma HLS INLINE
    uint8_t s1[256];
    uint8_t s2[256];
    uint8_t s3[256];
    uint8_t s4[256];
    uint8_t s5[256];
    uint8_t s6[256];
    uint8_t s7[256];

#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete

#pragma HLS ARRAY_PARTITION variable=s1 complete
#pragma HLS ARRAY_PARTITION variable=s2 complete
#pragma HLS ARRAY_PARTITION variable=s3 complete
#pragma HLS ARRAY_PARTITION variable=s4 complete
#pragma HLS ARRAY_PARTITION variable=s5 complete
#pragma HLS ARRAY_PARTITION variable=s6 complete
#pragma HLS ARRAY_PARTITION variable=s7 complete

    /* ===================== s1 : pas = 1 ===================== */
    for (int i = 0; i < 256; i += 2) {
#pragma HLS UNROLL
        s1[i]   = stride[0] ? (src[i] ^ src[i+1]) : src[i];
        s1[i+1] = src[i+1];
    }

    /* ===================== s2 : pas = 2 ===================== */
    for (int i = 0; i < 256; i += 4) {
#pragma HLS UNROLL
        s2[i+0] = stride[1] ? (s1[i+0] ^ s1[i+2]) : s1[i+0];
        s2[i+1] = stride[1] ? (s1[i+1] ^ s1[i+3]) : s1[i+1];
        s2[i+2] = s1[i+2];
        s2[i+3] = s1[i+3];
    }

    /* ===================== s3 : pas = 4 ===================== */
    for (int i = 0; i < 256; i += 8) {
#pragma HLS UNROLL
        for (int j = 0; j < 4; j++) {
#pragma HLS UNROLL
            s3[i+j] = stride[2] ? (s2[i+j] ^ s2[i+j+4]) : s2[i+j];
        }
        for (int j = 4; j < 8; j++) {
#pragma HLS UNROLL
            s3[i+j] = s2[i+j];
        }
    }

    /* ===================== s4 : pas = 8 ===================== */
    for (int i = 0; i < 256; i += 16) {
#pragma HLS UNROLL
        for (int j = 0; j < 8; j++) {
#pragma HLS UNROLL
            s4[i+j] = stride[3] ? (s3[i+j] ^ s3[i+j+8]) : s3[i+j];
        }
        for (int j = 8; j < 16; j++) {
#pragma HLS UNROLL
            s4[i+j] = s3[i+j];
        }
    }

    /* ===================== s5 : pas = 16 ===================== */
    for (int i = 0; i < 256; i += 32) {
#pragma HLS UNROLL
        for (int j = 0; j < 16; j++) {
#pragma HLS UNROLL
            s5[i+j] = stride[4] ? (s4[i+j] ^ s4[i+j+16]) : s4[i+j];
        }
        for (int j = 16; j < 32; j++) {
#pragma HLS UNROLL
            s5[i+j] = s4[i+j];
        }
    }

    /* ===================== s6 : pas = 32 ===================== */
    for (int i = 0; i < 256; i += 64) {
#pragma HLS UNROLL
        for (int j = 0; j < 32; j++) {
#pragma HLS UNROLL
            s6[i+j] = stride[5] ? (s5[i+j] ^ s5[i+j+32]) : s5[i+j];
        }
        for (int j = 32; j < 64; j++) {
#pragma HLS UNROLL
            s6[i+j] = s5[i+j];
        }
    }

    /* ===================== s7 : pas = 64 ===================== */
    for (int i = 0; i < 256; i += 128) {
#pragma HLS UNROLL
        for (int j = 0; j < 64; j++) {
#pragma HLS UNROLL
            s7[i+j] = stride[6] ? (s6[i+j] ^ s6[i+j+64]) : s6[i+j];
        }
        for (int j = 64; j < 128; j++) {
#pragma HLS UNROLL
            s7[i+j] = s6[i+j];
        }
    }

    /* ===================== dst : pas = 128 ===================== */
    for (int i = 0; i < 128; i++) {
#pragma HLS UNROLL
        dst[i] = stride[7] ? (s7[i] ^ s7[i+128]) : s7[i];
    }
    for (int i = 128; i < 256; i++) {
#pragma HLS UNROLL
        dst[i] = s7[i];
    }
}
//
//
////////////////////////////////////////////////////////////////////////////
//
//
void v2_xor_processor(uint8_t dst[2], uint8_t src[2], const ap_uint<2> stride)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete
#ifdef _COMPONENT_
    #pragma HLS interface mode=ap_none
#endif
    xor_processor<2>(dst, src, stride);
}
//
//
void v4_xor_processor(uint8_t dst[4], uint8_t src[4], const ap_uint<4> stride)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete
#ifdef _COMPONENT_
    #pragma HLS interface mode=ap_none
#endif
    xor_processor<4>(dst, src, stride);
}
//
//
void v8_xor_processor(uint8_t dst[8], uint8_t src[8], const ap_uint<8> stride)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete
#ifdef _COMPONENT_
    #pragma HLS interface mode=ap_none
#endif
    xor_processor<8>(dst, src, stride);
}
//
//
void v16_xor_processor(uint8_t dst[16], uint8_t src[16], const ap_uint<16> stride)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete
#ifdef _COMPONENT_
    #pragma HLS interface mode=ap_none
#endif
    xor_processor<16>(dst, src, stride);
}
//
//
void v32_xor_processor(uint8_t dst[32], uint8_t src[32], const ap_uint<32> stride)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete
#ifdef _COMPONENT_
    #pragma HLS interface mode=ap_none
#endif
    xor_processor<32>(dst, src, stride);
}
//
//
void v64_xor_processor(uint8_t dst[64], uint8_t src[64], const ap_uint<64> stride)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete
#ifdef _COMPONENT_
    #pragma HLS interface mode=ap_none
#endif
    xor_processor<64>(dst, src, stride);
}
//
//
void v128_xor_processor(uint8_t dst[128], uint8_t src[128], const ap_uint<128> stride)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete
#ifdef _COMPONENT_
    #pragma HLS interface mode=ap_none
#endif
    xor_processor<128>(dst, src, stride);
}
//
//
void v256_xor_processor(uint8_t dst[256], uint8_t src[256], const ap_uint<256> stride)
{
#pragma HLS INLINE off
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=src complete
#pragma HLS ARRAY_PARTITION variable=dst complete
#ifdef _COMPONENT_
    #pragma HLS interface mode=ap_none
#endif
    xor_processor<256>(dst, src, stride);
}
//
//
////////////////////////////////////////////////////////////////////////////
//
//
