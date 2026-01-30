//!
//!     Copyright (c) 2020-2023, Bertrand LE GAL
//!     All rights reserved.
//!
//!     Redistribution and use in source and binary forms, with or without
//!     modification, are not permitted with written authorization.
//!
//!
#pragma once
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
#include <cstdint>
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
template <int gf_size>
int f_argmax(const int32_t *value)
{
	// std::cout << "\033c" << std::flush;
	// for (int i = 0; i < gf_size; i++)
	// {
	//     printf("arg_max %d : %.20f \n", i, (float)value[i]);
	// }

	int max_index = 0;
	int32_t max_value = value[0];

	for (int i = 1; i < gf_size; i++)
	{
		if (value[i] > max_value)
		{
			max_value = value[i];
			max_index = i;
		}
	}
	return max_index;
}

template <int gf_size>
int f_argmax(const int64_t *value)
{
	// std::cout << "\033c" << std::flush;
	// for (int i = 0; i < gf_size; i++)
	// {
	//     printf("arg_max %d : %.20f \n", i, (float)value[i]);
	// }

	int max_index = 0;
	int64_t max_value = value[0];

	for (int i = 1; i < gf_size; i++)
	{
		if (value[i] > max_value)
		{
			max_value = value[i];
			max_index = i;
		}
	}
	return max_index;
}

template <int gf_size>
inline int f_argmax2(int32_t *arr, const int argmax1)
{
	const int32_t keep = arr[argmax1];
	arr[argmax1] = 0.f;
	const int arg2 = f_argmax<gf_size>(arr);
	arr[argmax1] = keep;
	return arg2;
}
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
