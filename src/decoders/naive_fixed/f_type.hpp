#pragma once
//
//
//
//
//
#include "include/ap_fixed.h"
//

namespace f_fype
{
constexpr int NINTG = 2;
constexpr int NBITS = 26;

} // namespace f_fype

struct symbols_f
{
	ap_fixed<f_fype::NBITS, f_fype::NINTG> value[_GF_];
	bool is_freq;
};
//
//
//
//
//
inline float is_issue(const ap_fixed<f_fype::NBITS, f_fype::NINTG> *value)
{
	float maxv = 0.f;
	for (size_t i = 0; i < _GF_; i++)
	{
		const float v = value[i].to_float();
		maxv = std::fmaxf(maxv, std::fabs(v));
	}
	return maxv;
}
//
//
//
//
//
