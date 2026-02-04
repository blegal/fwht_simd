#include "hadamard_i32/Hadamard.hpp"
#include "definitions/code.hpp"

#if (_GF_ == 8) || defined(ALL_GFs)
#include "hadamard_i32/Hadamard_8.hpp"
#endif

#if (_GF_ == 16) || defined(ALL_GFs)
#include "hadamard_i32/Hadamard_16.hpp"
#endif

#if (_GF_ == 32) || defined(ALL_GFs)
#include "hadamard_i32/Hadamard_32.hpp"
#endif

#if (_GF_ == 64) || defined(ALL_GFs)
#include "hadamard_i32/Hadamard_64.hpp"
#endif

#if (_GF_ == 128) || defined(ALL_GFs)
#include "hadamard_i32/Hadamard_128.hpp"
#endif

#if (_GF_ == 256) || defined(ALL_GFs)
#include "hadamard_i32/Hadamard_256.hpp"
#endif

#if (_GF_ == 512) || defined(ALL_GFs)
#include "hadamard_i32/Hadamard_512.hpp"
#endif

#if (_GF_ == 1024) || defined(ALL_GFs)
#include "hadamard_i32/Hadamard_1024.hpp"
#endif

#if (_GF_ == 2048) || defined(ALL_GFs)
#include "hadamard_i32/Hadamard_2048.hpp"
#endif

#if (_GF_ == 4096) || defined(ALL_GFs)
#include "hadamard_i32/Hadamard_4096.hpp"
#endif
