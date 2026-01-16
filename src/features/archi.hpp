#pragma once

#if defined(__ARM_NEON__) || defined(__ARM_NEON)

#elif defined(__AVX2__)

#else

#endif

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#include "fwht/fwht_neon.hpp"
#include "fwht/i64_fwht.hpp"

#endif

#if defined(__AVX512F__)
#include "fwht/fwht_avx512.hpp"
#include "fwht/i64_fwht.hpp"
#endif

#if defined(__AVX2__)
#include "fwht/fwht_avx2.hpp"
#include "fwht/i64_fwht_avx2.hpp"
#endif

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#define FUNCTION_SUFFIX _neon
#define FUNCTION_SUFFIX1
#elif defined(__AVX512F__)
#define FUNCTION_SUFFIX _avx2
#define FUNCTION_SUFFIX1 
#elif defined(__AVX2__)
#define FUNCTION_SUFFIX _avx2
#define FUNCTION_SUFFIX1 _avx2
#else
#define FUNCTION_SUFFIX
#define FUNCTION_SUFFIX1
#endif

#define PASTER(x, y)    x##y
#define EVALUATOR(x, y) PASTER(x, y)

#define FWHT EVALUATOR(fwht, FUNCTION_SUFFIX)
#define I64_FWHT EVALUATOR(i64_fwht, FUNCTION_SUFFIX1)

#include "argmax/argmax.hpp"

#include "normalize/normalize.hpp"
#include "normalize/scale_by_inverse_avx2.hpp"
