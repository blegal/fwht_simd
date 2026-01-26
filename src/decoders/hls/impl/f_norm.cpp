//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
#include "f_norm.hpp"
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
ap_int <i_norm_width> barrel_shift(const ap_int <i_norm_width> src, const ap_uint<6> factor)
{
#pragma HLS INLINE
	ap_int <i_norm_width> resu;
	switch (factor.to_uint()) {
		case 35: resu = src.range(35, 24); break;
		case 34: resu = src.range(34, 23); break;
		case 33: resu = src.range(33, 22); break;
		case 32: resu = src.range(32, 21); break;
		case 31: resu = src.range(31, 20); break;
		case 30: resu = src.range(30, 19); break;
		case 29: resu = src.range(29, 18); break;
		case 28: resu = src.range(28, 17); break;
		case 27: resu = src.range(27, 16); break;
		case 26: resu = src.range(26, 15); break;
		case 25: resu = src.range(25, 14); break;
		case 24: resu = src.range(24, 13); break;
		case 23: resu = src.range(23, 12); break;
		case 22: resu = src.range(22, 11); break;
		case 21: resu = src.range(21, 10); break;
		case 20: resu = src.range(20, 9);  break;
		case 19: resu = src.range(19, 8);  break;
		case 18: resu = src.range(18, 7);  break;
		case 17: resu = src.range(17, 6);  break;
		case 16: resu = src.range(16, 5);  break;
		case 15: resu = src.range(15, 4);  break;
		case 14: resu = src.range(14, 3);  break;
		case 13: resu = src.range(13, 2);  break;
		case 12: resu = src.range(12, 1);  break;
		case 11: resu = src.range(11, 0);  break;
		default: resu = 0; break;
	}
	return resu;
}
//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
t_o_norm vec_i_norm(const t_i_norm src)
{
#ifdef _COMPONENT_
    #pragma HLS PIPELINE
#else
    #pragma HLS INLINE
#endif
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=src.value

	// (12 bits) => (18 bits) => (36 bits) => (12 bits)
	ap_int<i_norm_width> absv[64];
    for (int i = 0; i < gf_size; i++)
    {
    	const ap_int<i_norm_width> value = src.value[i];
    	if(value < 0)	absv[i] = -value;
    	else			absv[i] =  value;
    }

    //
    // On calcule le premier facteur de scaling basé sur le maximum
    //
    ap_int<i_norm_width> loc[4];
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=loc
    loc[0] = ((absv[ 0] | absv[ 1]) | (absv[ 2] | absv[ 3])) | ((absv[ 4] | absv[ 5]) | (absv[ 6] | absv[ 7])) |
             ((absv[ 8] | absv[ 9]) | (absv[10] | absv[11])) | ((absv[12] | absv[13]) | (absv[14] | absv[15]));

    loc[1] = ((absv[16] | absv[17]) | (absv[18] | absv[19])) | ((absv[20] | absv[21]) | (absv[22] | absv[23])) |
             ((absv[24] | absv[25]) | (absv[26] | absv[27])) | ((absv[28] | absv[29]) | (absv[30] | absv[31]));

    loc[2] = ((absv[32] | absv[33]) | (absv[34] | absv[35])) | ((absv[36] | absv[37]) | (absv[38] | absv[39])) |
             ((absv[40] | absv[41]) | (absv[42] | absv[43])) | ((absv[44] | absv[45]) | (absv[46] | absv[47]));

    loc[3] = ((absv[48] | absv[49]) | (absv[50] | absv[51])) | ((absv[52] | absv[53]) | (absv[54] | absv[55])) |
             ((absv[56] | absv[57]) | (absv[58] | absv[59])) | ((absv[60] | absv[61]) | (absv[62] | absv[63]));
    const ap_int<i_norm_width> sum = loc[0] | loc[1] | loc[2] | loc[3];

    //
    // On calcule le premier facteur de scaling basé sur la position du MSB
    //
	// (12 bits) => (18 bits) => (36 bits) => (12 bits)
    ap_uint<6> factor;
    // if   (sum.get_bit(i_norm_width- 1) == 1) factor =  31; // NOT POSSIBLE !
         if (sum.get_bit(i_norm_width- 2) == 1) factor =  35; // 34 + sign
    else if (sum.get_bit(i_norm_width- 3) == 1) factor =  34; // 33 + sign
    else if (sum.get_bit(i_norm_width- 4) == 1) factor =  33; // ...
    else if (sum.get_bit(i_norm_width- 5) == 1) factor =  32;
    else if (sum.get_bit(i_norm_width- 6) == 1) factor =  31;
    else if (sum.get_bit(i_norm_width- 7) == 1) factor =  30;
    else if (sum.get_bit(i_norm_width- 8) == 1) factor =  29;
    else if (sum.get_bit(i_norm_width- 9) == 1) factor =  28;
    else if (sum.get_bit(i_norm_width-10) == 1) factor =  27;
    else if (sum.get_bit(i_norm_width-11) == 1) factor =  26;
    else if (sum.get_bit(i_norm_width-12) == 1) factor =  25;
    else if (sum.get_bit(i_norm_width-13) == 1) factor =  24;
    else if (sum.get_bit(i_norm_width-14) == 1) factor =  23;
    else if (sum.get_bit(i_norm_width-15) == 1) factor =  22;
    else if (sum.get_bit(i_norm_width-16) == 1) factor =  21;
    else if (sum.get_bit(i_norm_width-17) == 1) factor =  20;
	else if (sum.get_bit(i_norm_width-18) == 1) factor =  19;
    else if (sum.get_bit(i_norm_width-19) == 1) factor =  18;
    else if (sum.get_bit(i_norm_width-20) == 1) factor =  17;
    else if (sum.get_bit(i_norm_width-21) == 1) factor =  16;
    else if (sum.get_bit(i_norm_width-22) == 1) factor =  15;
    else if (sum.get_bit(i_norm_width-23) == 1) factor =  14;
    else if (sum.get_bit(i_norm_width-24) == 1) factor =  13;
    else if (sum.get_bit(i_norm_width-25) == 1) factor =  12;
    else
    	factor = 11;
//	printf("factor = %d\n", factor.to_uint());
/*
    else if (sum.get_bit(i_norm_width-25) == 1) factor =  11;
    else if (sum.get_bit(i_norm_width-26) == 1) factor =  10;
    else if (sum.get_bit(i_norm_width-27) == 1) factor =   9;
    else if (sum.get_bit(i_norm_width-28) == 1) factor =   8;
    else if (sum.get_bit(i_norm_width-29) == 1) factor =   7;
    else if (sum.get_bit(i_norm_width-30) == 1) factor =   7;
    else if (sum.get_bit(i_norm_width-31) == 1) factor =   5;
    else if (sum.get_bit(i_norm_width-32) == 1) factor =   4;
    else if (sum.get_bit(i_norm_width-33) == 1) factor =   3;
    else if (sum.get_bit(i_norm_width-34) == 1) factor =   2;
    else if (sum.get_bit(i_norm_width-35) == 1) factor =   1;
    else
    	factor = 0;
*/
    t_o_norm dst;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=dst.value
    for (int i = 0; i < gf_size; i++) {
        dst.value[i] = barrel_shift(src.value[i], factor);
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