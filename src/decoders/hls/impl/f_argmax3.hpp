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

extern qtuple<12, 2> argmax3_gf4  (const t_ram<12,   4> inp);
extern qtuple<12, 3> argmax3_gf8  (const t_ram<12,   8> inp);
extern qtuple<12, 4> argmax3_gf16 (const t_ram<12,  16> inp);
extern qtuple<12, 5> argmax3_gf32 (const t_ram<12,  32> inp);
extern qtuple<12, 6> argmax3_gf64 (const t_ram<12,  64> inp);
extern qtuple<12, 7> argmax3_gf128(const t_ram<12, 128> inp);
extern qtuple<12, 8> argmax3_gf256(const t_ram<12, 256> inp);

//
//
//
//////////////////////////////////////////////////////////////////////
//
//
//
