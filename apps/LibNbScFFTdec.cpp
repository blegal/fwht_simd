#include <chrono>
#include <cstdlib>
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#include "decoders/basic/decoder_basic.hpp"
#include "decoders/naive/decoder_naive.hpp"
#include "decoders/naive_cfloat/decoder_naive_cfloat.hpp"
#include "decoders/naive_fixed/decoder_naive_fixed.hpp"
#include "decoders/naive_int32_t/decoder_naive_int32_t.hpp"
#include "decoders/specialized/decoder_specialized.hpp"
#include "decoders/specialized_pruning/decoder_specialized_pruning.hpp"
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#include "encoder/polar_encoder.hpp"
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#ifndef ALL_GFs
 #error " THE FLAG SHOULD BE ENABLE ! #define ALL_GFs"
#endif

extern "C" decoder* allocate_dec(const std::string type,
                                const int N,
                                const int GF,
                                const int* f_vector)
{
    decoder* ptr = nullptr;

    switch (GF)
    {
#if (_GF_ == 8) || defined(ALL_GFs)
        case 8:
            if      (type == "basic")  ptr = new decoder_basic              <8>(N, f_vector);
            else if (type == "naive")  ptr = new decoder_naive              <8>(N, f_vector);
            else if (type == "cfloat") ptr = new decoder_naive_cfloat       <8>(N, f_vector);
            else if (type == "fixed")  ptr = new decoder_naive_fixed        <8>(N, f_vector);
            else if (type == "fixed")  ptr = new decoder_naive_int32_t      <8>(N, f_vector);
            else if (type == "spec")   ptr = new decoder_specialized        <8>(N, f_vector);
            else if (type == "pruned") ptr = new decoder_specialized_pruning<8>(N, f_vector);
            break;
#endif

        case 16:
            if      (type == "basic")  ptr = new decoder_basic       <16>(N, f_vector);
            else if (type == "naive")  ptr = new decoder_basic       <16>(N, f_vector);
            else if (type == "cfloat") ptr = new decoder_naive_cfloat<16>(N, f_vector);
            else if (type == "fixed")  ptr = new decoder_basic       <16>(N, f_vector);
            else if (type == "spec")   ptr = new decoder_basic       <16>(N, f_vector);
            else if (type == "pruned") ptr = new decoder_basic       <16>(N, f_vector);
            break;

        case 32:
            if      (type == "basic")  ptr = new decoder_basic       <32>(N, f_vector);
            else if (type == "naive")  ptr = new decoder_basic       <32>(N, f_vector);
            else if (type == "cfloat") ptr = new decoder_naive_cfloat<32>(N, f_vector);
            else if (type == "fixed")  ptr = new decoder_basic       <32>(N, f_vector);
            else if (type == "spec")   ptr = new decoder_basic       <32>(N, f_vector);
            else if (type == "pruned") ptr = new decoder_basic       <32>(N, f_vector);
            break;

        case 64:
            if      (type == "basic")  ptr = new decoder_basic       <64>(N, f_vector);
            else if (type == "naive")  ptr = new decoder_basic       <64>(N, f_vector);
            else if (type == "cfloat") ptr = new decoder_naive_cfloat<64>(N, f_vector);
            else if (type == "fixed")  ptr = new decoder_basic       <64>(N, f_vector);
            else if (type == "spec")   ptr = new decoder_basic       <64>(N, f_vector);
            else if (type == "pruned") ptr = new decoder_basic       <64>(N, f_vector);
            break;

        case 128:
            if      (type == "basic")  ptr = new decoder_basic       <128>(N, f_vector);
            else if (type == "naive")  ptr = new decoder_basic       <128>(N, f_vector);
            else if (type == "cfloat") ptr = new decoder_naive_cfloat<128>(N, f_vector);
            else if (type == "fixed")  ptr = new decoder_basic       <128>(N, f_vector);
            else if (type == "spec")   ptr = new decoder_basic       <128>(N, f_vector);
            else if (type == "pruned") ptr = new decoder_basic       <128>(N, f_vector);
            break;

        case 256:
            if      (type == "basic")  ptr = new decoder_basic       <256>(N, f_vector);
            else if (type == "naive")  ptr = new decoder_basic       <256>(N, f_vector);
            else if (type == "cfloat") ptr = new decoder_naive_cfloat<256>(N, f_vector);
            else if (type == "fixed")  ptr = new decoder_basic       <256>(N, f_vector);
            else if (type == "spec")   ptr = new decoder_basic       <256>(N, f_vector);
            else if (type == "pruned") ptr = new decoder_basic       <256>(N, f_vector);
            break;

        case 512:
            if      (type == "basic")  ptr = new decoder_basic       <512>(N, f_vector);
            else if (type == "naive")  ptr = new decoder_basic       <512>(N, f_vector);
            else if (type == "cfloat") ptr = new decoder_naive_cfloat<512>(N, f_vector);
            else if (type == "fixed")  ptr = new decoder_basic       <512>(N, f_vector);
            else if (type == "spec")   ptr = new decoder_basic       <512>(N, f_vector);
            else if (type == "pruned") ptr = new decoder_basic       <512>(N, f_vector);
            break;

        case 1024:
            if      (type == "basic")  ptr = new decoder_basic       <1024>(N, f_vector);
            else if (type == "naive")  ptr = new decoder_basic       <1024>(N, f_vector);
            else if (type == "cfloat") ptr = new decoder_naive_cfloat<1024>(N, f_vector);
            else if (type == "fixed")  ptr = new decoder_basic       <1024>(N, f_vector);
            else if (type == "spec")   ptr = new decoder_basic       <1024>(N, f_vector);
            else if (type == "pruned") ptr = new decoder_basic       <1024>(N, f_vector);
            break;

        case 2048:
            if      (type == "basic")  ptr = new decoder_basic       <2048>(N, f_vector);
            else if (type == "naive")  ptr = new decoder_basic       <2048>(N, f_vector);
            else if (type == "cfloat") ptr = new decoder_naive_cfloat<2048>(N, f_vector);
            else if (type == "fixed")  ptr = new decoder_basic       <2048>(N, f_vector);
            else if (type == "spec")   ptr = new decoder_basic       <2048>(N, f_vector);
            else if (type == "pruned") ptr = new decoder_basic       <2048>(N, f_vector);
            break;

        case 4096:
            if      (type == "basic")  ptr = new decoder_basic<4096>(N, f_vector);
            else if (type == "naive")  ptr = new decoder_basic<4096>(N, f_vector);
            else if (type == "cfloat") ptr = new decoder_basic<4096>(N, f_vector);
            else if (type == "fixed")  ptr = new decoder_basic<4096>(N, f_vector);
            else if (type == "spec")   ptr = new decoder_basic<4096>(N, f_vector);
            else if (type == "pruned") ptr = new decoder_basic<4096>(N, f_vector);
            break;

        default:
            break;
    }

    if (ptr == nullptr)
    {
        printf("(EE) Error we should never be there...\n");
        printf("(EE) %s %d\n", __FILE__, __LINE__);
        printf("(EE) An issue was detected with following parameters:\n");
        printf("(EE) - N    = %d\n", N);
        printf("(EE) - GF   = %d\n", GF);
        printf("(EE) - type = %s\n", type.c_str());
        exit(EXIT_FAILURE);
    }

    return ptr;
}

extern "C" decoder* allocate_dec2( const std::string type, const int N, const int GF, const int* f_vector)
{
    decoder* ptr = nullptr;
    if ( type == "basic" ) {
        if ( GF ==    8 ) { ptr =  new decoder_basic<   8>(N, f_vector);
        }else if ( GF ==   16 ) { ptr =  new decoder_basic<  16>(N, f_vector);
        }else if ( GF ==   32 ) { ptr =  new decoder_basic<  32>(N, f_vector);
        }else if ( GF ==   64 ) { ptr =  new decoder_basic<  64>(N, f_vector);
        }else if ( GF ==  128 ) { ptr =  new decoder_basic< 128>(N, f_vector);
        }else if ( GF ==  256 ) { ptr =  new decoder_basic< 256>(N, f_vector);
        }else if ( GF ==  512 ) { ptr =  new decoder_basic< 512>(N, f_vector);
        }else if ( GF == 1024 ) { ptr =  new decoder_basic<1024>(N, f_vector);
        }else if ( GF == 2048 ) { ptr =  new decoder_basic<2048>(N, f_vector);
        }else if ( GF == 4096 ) {
            ptr =  new decoder_basic<4096>(N, f_vector);
        }
    }else if ( type == "naive" ) {
        if ( GF ==    8 ) { ptr =  new decoder_basic<   8>(N, f_vector);
        }else if ( GF ==   16 ) { ptr =  new decoder_basic<  16>(N, f_vector);
        }else if ( GF ==   32 ) { ptr =  new decoder_basic<  32>(N, f_vector);
        }else if ( GF ==   64 ) { ptr =  new decoder_basic<  64>(N, f_vector);
        }else if ( GF ==  128 ) { ptr =  new decoder_basic< 128>(N, f_vector);
        }else if ( GF ==  256 ) { ptr =  new decoder_basic< 256>(N, f_vector);
        }else if ( GF ==  512 ) { ptr =  new decoder_basic< 512>(N, f_vector);
        }else if ( GF == 1024 ) { ptr =  new decoder_basic<1024>(N, f_vector);
        }else if ( GF == 2048 ) { ptr =  new decoder_basic<2048>(N, f_vector);
        }else if ( GF == 4096 ) {
            ptr =  new decoder_basic<4096>(N, f_vector);
        }
    }else if ( type == "cfloat" ) {
        if ( GF ==    8 ) { ptr =  new decoder_basic<   8>(N, f_vector);
        }else if ( GF ==   16 ) { ptr =  new decoder_naive_cfloat<  16>(N, f_vector);
        }else if ( GF ==   32 ) { ptr =  new decoder_naive_cfloat<  32>(N, f_vector);
        }else if ( GF ==   64 ) { ptr =  new decoder_naive_cfloat<  64>(N, f_vector);
        }else if ( GF ==  128 ) { ptr =  new decoder_naive_cfloat< 128>(N, f_vector);
        }else if ( GF ==  256 ) { ptr =  new decoder_naive_cfloat< 256>(N, f_vector);
        }else if ( GF ==  512 ) { ptr =  new decoder_naive_cfloat< 512>(N, f_vector);
        }else if ( GF == 1024 ) { ptr =  new decoder_naive_cfloat<1024>(N, f_vector);
        }else if ( GF == 2048 ) { ptr =  new decoder_naive_cfloat<2048>(N, f_vector);
        }else if ( GF == 4096 ) {
            ptr =  new decoder_basic<4096>(N, f_vector);
        }
    }else if ( type == "fixed" ) {
        if ( GF ==    8 ) { ptr =  new decoder_basic<   8>(N, f_vector);
        }else if ( GF ==   16 ) { ptr =  new decoder_basic<  16>(N, f_vector);
        }else if ( GF ==   32 ) { ptr =  new decoder_basic<  32>(N, f_vector);
        }else if ( GF ==   64 ) { ptr =  new decoder_basic<  64>(N, f_vector);
        }else if ( GF ==  128 ) { ptr =  new decoder_basic< 128>(N, f_vector);
        }else if ( GF ==  256 ) { ptr =  new decoder_basic< 256>(N, f_vector);
        }else if ( GF ==  512 ) { ptr =  new decoder_basic< 512>(N, f_vector);
        }else if ( GF == 1024 ) { ptr =  new decoder_basic<1024>(N, f_vector);
        }else if ( GF == 2048 ) { ptr =  new decoder_basic<2048>(N, f_vector);
        }else if ( GF == 4096 ) {
            ptr =  new decoder_basic<4096>(N, f_vector);
        }
    }else if ( type == "spec" ) {
        if ( GF ==    8 ) { ptr =  new decoder_basic<   8>(N, f_vector);
        }else if ( GF ==   16 ) { ptr =  new decoder_basic<  16>(N, f_vector);
        }else if ( GF ==   32 ) { ptr =  new decoder_basic<  32>(N, f_vector);
        }else if ( GF ==   64 ) { ptr =  new decoder_basic<  64>(N, f_vector);
        }else if ( GF ==  128 ) { ptr =  new decoder_basic< 128>(N, f_vector);
        }else if ( GF ==  256 ) { ptr =  new decoder_basic< 256>(N, f_vector);
        }else if ( GF ==  512 ) { ptr =  new decoder_basic< 512>(N, f_vector);
        }else if ( GF == 1024 ) { ptr =  new decoder_basic<1024>(N, f_vector);
        }else if ( GF == 2048 ) { ptr =  new decoder_basic<2048>(N, f_vector);
        }else if ( GF == 4096 ) {
            ptr =  new decoder_basic<4096>(N, f_vector);
        }
    }else if ( type == "pruned" ) {
        if ( GF ==    8 ) { ptr =  new decoder_basic<   8>(N, f_vector);
        }else if ( GF ==   16 ) { ptr =  new decoder_basic<  16>(N, f_vector);
        }else if ( GF ==   32 ) { ptr =  new decoder_basic<  32>(N, f_vector);
        }else if ( GF ==   64 ) { ptr =  new decoder_basic<  64>(N, f_vector);
        }else if ( GF ==  128 ) { ptr =  new decoder_basic< 128>(N, f_vector);
        }else if ( GF ==  256 ) { ptr =  new decoder_basic< 256>(N, f_vector);
        }else if ( GF ==  512 ) { ptr =  new decoder_basic< 512>(N, f_vector);
        }else if ( GF == 1024 ) { ptr =  new decoder_basic<1024>(N, f_vector);
        }else if ( GF == 2048 ) { ptr =  new decoder_basic<2048>(N, f_vector);
        }else if ( GF == 4096 ) {
            ptr =  new decoder_basic<4096>(N, f_vector);
        }
    }

    if ( ptr == nullptr ) {
        printf("(EE) Error we should never be there...\n");
        printf("(EE) %s %d\n", __FILE__, __LINE__);
        printf("(EE) An issue was detected with following parameters:\n");
        printf("(EE) - N  = %d\n", N);
        printf("(EE) - GF = %d\n", GF);
        exit( EXIT_FAILURE );
    }

    return ptr;
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
extern "C" polar_encoder* allocate_enc( const int N, const int K, const int GF, const int* f_vector)
{
    if ( (GF >= 16) && (GF <= 4096) ) {
        return new polar_encoder(f_vector, K, N);
    }else {
        printf("(EE) Error we should never be there...\n");
        printf("(EE) %s %d\n", __FILE__, __LINE__);
        printf("(EE) An issue was detected with following parameters:\n");
        printf("(EE) - N  = %d\n", N);
        printf("(EE) - K  = %d\n", K);
        printf("(EE) - GF = %d\n", GF);
        exit( EXIT_FAILURE );
    }
    return nullptr;
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
