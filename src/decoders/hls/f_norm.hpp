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
inline t_int18b vec_i_norm(const t_int48b src)
{
#pragma HLS INLINE
//#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value

	int48b absv[64];
    for (int i = 0; i < gf_size; i++)
    {
    	const int48b value = src.value[i];
    	if(value < 0)	absv[i] = -value;
    	else			absv[i] =  value;
    }

    //
    // On calcule le premier facteur de scaling basé sur le maximum
    //
    int48b loc[4];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=loc
    loc[0] = ((absv[ 0] | absv[ 1]) | (absv[ 2] | absv[ 3])) | ((absv[ 4] | absv[ 5]) | (absv[ 6] | absv[ 7])) |
             ((absv[ 8] | absv[ 9]) | (absv[10] | absv[11])) | ((absv[12] | absv[13]) | (absv[14] | absv[15]));

    loc[1] = ((absv[16] | absv[17]) | (absv[18] | absv[19])) | ((absv[20] | absv[21]) | (absv[22] | absv[23])) |
             ((absv[24] | absv[25]) | (absv[26] | absv[27])) | ((absv[28] | absv[29]) | (absv[30] | absv[31]));

    loc[2] = ((absv[32] | absv[33]) | (absv[34] | absv[35])) | ((absv[36] | absv[37]) | (absv[38] | absv[39])) |
             ((absv[40] | absv[41]) | (absv[42] | absv[43])) | ((absv[44] | absv[45]) | (absv[46] | absv[47]));

    loc[3] = ((absv[48] | absv[49]) | (absv[50] | absv[51])) | ((absv[52] | absv[53]) | (absv[54] | absv[55])) |
             ((absv[56] | absv[57]) | (absv[58] | absv[59])) | ((absv[60] | absv[61]) | (absv[62] | absv[63]));
    const int48b sum = loc[0] | loc[1] | loc[2] | loc[3];
	printf("sum = %llu (0x%16.16llX)\n", sum.to_uint64(), sum.to_uint64());

    //
    // On calcule le premier facteur de scaling basé sur la position du MSB
    //

    ap_int<6> factor;
    if      (sum.get_bit(i_norm_width- 1) == 1) factor =  31;
    else if (sum.get_bit(i_norm_width- 2) == 1) factor =  30;
    else if (sum.get_bit(i_norm_width- 3) == 1) factor =  29;
    else if (sum.get_bit(i_norm_width- 4) == 1) factor =  28;
    else if (sum.get_bit(i_norm_width- 5) == 1) factor =  27;
    else if (sum.get_bit(i_norm_width- 6) == 1) factor =  26;
    else if (sum.get_bit(i_norm_width- 7) == 1) factor =  25;
    else if (sum.get_bit(i_norm_width- 8) == 1) factor =  24;
    else if (sum.get_bit(i_norm_width- 9) == 1) factor =  23;
    else if (sum.get_bit(i_norm_width-10) == 1) factor =  22;
    else if (sum.get_bit(i_norm_width-11) == 1) factor =  21;
    else if (sum.get_bit(i_norm_width-12) == 1) factor =  20;
    else if (sum.get_bit(i_norm_width-13) == 1) factor =  19;
    else if (sum.get_bit(i_norm_width-14) == 1) factor =  18;
    else if (sum.get_bit(i_norm_width-15) == 1) factor =  17;
    else if (sum.get_bit(i_norm_width-16) == 1) factor =  16;
    else if (sum.get_bit(i_norm_width-17) == 1) factor =  15;
	else if (sum.get_bit(i_norm_width-18) == 1) factor =  14;
    else if (sum.get_bit(i_norm_width-19) == 1) factor =  13;
    else if (sum.get_bit(i_norm_width-20) == 1) factor =  12;
    else if (sum.get_bit(i_norm_width-21) == 1) factor =  11;
    else if (sum.get_bit(i_norm_width-22) == 1) factor =  10;
    else if (sum.get_bit(i_norm_width-23) == 1) factor =   9;
    else if (sum.get_bit(i_norm_width-24) == 1) factor =   8;
    else if (sum.get_bit(i_norm_width-25) == 1) factor =   7;
    else if (sum.get_bit(i_norm_width-26) == 1) factor =   6;
    else if (sum.get_bit(i_norm_width-27) == 1) factor =   5;
    else if (sum.get_bit(i_norm_width-28) == 1) factor =   4;
    else if (sum.get_bit(i_norm_width-29) == 1) factor =   3;
    else if (sum.get_bit(i_norm_width-30) == 1) factor =   2;
    else if (sum.get_bit(i_norm_width-31) == 1) factor =   1;
    else if (sum.get_bit(i_norm_width-32) == 1) factor =   0;
    else if (sum.get_bit(i_norm_width-33) == 1) factor =  -1;
    else if (sum.get_bit(i_norm_width-34) == 1) factor =  -2;
    else if (sum.get_bit(i_norm_width-35) == 1) factor =  -3;
    else if (sum.get_bit(i_norm_width-36) == 1) factor =  -4;
    else if (sum.get_bit(i_norm_width-37) == 1) factor =  -5;
    else if (sum.get_bit(i_norm_width-38) == 1) factor =  -6;
    else if (sum.get_bit(i_norm_width-39) == 1) factor =  -7;
    else if (sum.get_bit(i_norm_width-40) == 1) factor =  -8;
    else if (sum.get_bit(i_norm_width-41) == 1) factor =  -9;
    else if (sum.get_bit(i_norm_width-42) == 1) factor = -10;
    else if (sum.get_bit(i_norm_width-43) == 1) factor = -11;
    else if (sum.get_bit(i_norm_width-44) == 1) factor = -12;
    else if (sum.get_bit(i_norm_width-45) == 1) factor = -13;
    else if (sum.get_bit(i_norm_width-46) == 1) factor = -14;
    else if (sum.get_bit(i_norm_width-47) == 1) factor = -15;
    else //if (sum.get_bit(i_norm_width-46) == 1)
    	factor = -16;
    //else if (sum.get_bit(i_norm_width-47) == 1) factor = -18;
    //else factor = 47;

	for (int i = 0; i < i_norm_width; i++) {
		printf("%d", sum.get_bit(i_norm_width-1-i));
	}printf("\n");

	printf("factor = %d\n", factor.to_uint());

    t_int18b dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < gf_size; i++) {
    	int48b value = src.value[i] >> factor;
        dst.value[i] = value.range(17, 0);
    }
    return dst;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
