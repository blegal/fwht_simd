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
extern tuple vec_i_argmax2(const t_i_memo inp);

extern gtuple<12, 2> argmax2_gf4  (const t_ram<12,   4> inp);
extern gtuple<12, 3> argmax2_gf8  (const t_ram<12,   8> inp);
extern gtuple<12, 4> argmax2_gf16 (const t_ram<12,  16> inp);
extern gtuple<12, 5> argmax2_gf32 (const t_ram<12,  32> inp);
extern gtuple<12, 6> argmax2_gf64 (const t_ram<12,  64> inp);
extern gtuple<12, 7> argmax2_gf128(const t_ram<12, 128> inp);
extern gtuple<12, 8> argmax2_gf256(const t_ram<12, 256> inp);

//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
