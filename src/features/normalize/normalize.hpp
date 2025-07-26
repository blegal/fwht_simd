//
// Created by legal on 23/07/2025.
//
#pragma once

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
    #include "normalize_neon.hpp"
#elif defined(__AVX512F__)
    #include "normalize_avx512.hpp"
#elif defined(__AVX2__)
    #include "normalize_avx2.hpp"
#else
    #include "normalize_c.hpp"
#endif
