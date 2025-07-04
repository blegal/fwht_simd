#pragma once

#if defined(__ARM_NEON__)
#elif defined(__AVX2__)
#else
#endif

#include "fwht/fwht.hpp"
#include "fwht/fwht_norm.hpp"

#if defined(__ARM_NEON__)
#include "fwht/fwht_neon.hpp"
#include "fwht/fwht_norm_neon.hpp"
#endif

#if defined(__AVX2__)
#include "fwht/fwht_avx2.hpp"
#include "fwht/fwht_norm_avx2.hpp"
#endif

#if defined(__ARM_NEON__)
    #define FWHT_NORM fwht_norm_neon
#elif defined(__AVX2__)
    #define FWHT_NORM fwht_norm_avx2
#else
    #define FWHT_NORM fwht_norm
#endif

#include "argmax/argmax.hpp"

#include "normalize/normalize.hpp"