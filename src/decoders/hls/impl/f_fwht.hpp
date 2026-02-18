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
extern t_o_lwht fwht(const t_i_lwht src);

extern t_ram<14,   4> hls_fwht_gf4  (const t_ram<12,   4> src);
extern t_ram<15,   8> hls_fwht_gf8  (const t_ram<12,   8> src);
extern t_ram<16,  16> hls_fwht_gf16 (const t_ram<12,  16> src);
extern t_ram<17,  32> hls_fwht_gf32 (const t_ram<12,  32> src);
extern t_ram<18,  64> hls_fwht_gf64 (const t_ram<12,  64> src);
extern t_ram<19, 128> hls_fwht_gf128(const t_ram<12, 128> src);
extern t_ram<20, 256> hls_fwht_gf256(const t_ram<12, 256> src);

//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
