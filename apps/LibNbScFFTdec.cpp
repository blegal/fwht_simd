#include <chrono>
#include <cstdlib>
#include <vector>
#include <sstream>
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
#include "decoders/naive_scl_genie/naive_scl_genie.hpp"
#include "decoders/naive_scf/naive_scf.hpp"
#include "decoders/naive_scf_oracle/naive_scf_oracle.hpp"
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
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
bool parseString(const std::string& input, std::string& name, std::vector<std::string>& params) {
    params.clear();

    size_t openBracket = input.find('{');
    size_t closeBracket = input.find('}');

    // Cas 1 : pas de crochets → juste un nom
    if (openBracket == std::string::npos && closeBracket == std::string::npos) {
        name = trim(input);
        return true;
    }

    // Cas invalide : un seul crochet ou mal placé
    if (openBracket == std::string::npos || closeBracket == std::string::npos || closeBracket < openBracket) {
        return false;
    }

    // Extraire le nom
    name = trim(input.substr(0, openBracket));

    // Contenu entre crochets
    std::string inside = input.substr(openBracket + 1, closeBracket - openBracket - 1);

    // Si vide → pas de paramètres
    if (trim(inside).empty()) {
        return true;
    }

    // Parser les paramètres
    std::stringstream ss(inside);
    std::string item;

    while (std::getline(ss, item, '/')) {
        params.push_back(trim(item));
    }

    return true;
}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#ifndef ALL_GFs
 #error " THE FLAG SHOULD BE ENABLE ! #define ALL_GFs"
#endif
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
extern "C" decoder* allocate_dec( const std::string str, const int N, const int GF, const int* f_vector)
{
    std::string type;
    std::vector<std::string> params;
//    std::cout << "Input: " << str << std::endl;

    if (parseString(str, type, params)) {
//        std::cout << "Name: " << type << std::endl;
//        std::cout << "Params (" << params.size() << "):" << std::endl;
//        for (const auto& p : params) {
//            std::cout << " - " << p << std::endl;
//        }
    } else {
        printf("(EE) Error we should never be there [%s:%d]\n", __FILE__, __LINE__);
        printf("(EE) -> decoder specification is invalid !\n");
        exit( EXIT_FAILURE );
    }
//    std::cout << "------------------" << std::endl;

    decoder* ptr = nullptr;
    if ( (type == "basic") || (type == "dec0") ) {
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
    }else if ( (type == "naive") || (type == "dec1") ) {
              if ( GF ==    8 ) { ptr =  new decoder_naive<   8>(N, f_vector);
        }else if ( GF ==   16 ) { ptr =  new decoder_naive<  16>(N, f_vector);
        }else if ( GF ==   32 ) { ptr =  new decoder_naive<  32>(N, f_vector);
        }else if ( GF ==   64 ) { ptr =  new decoder_naive<  64>(N, f_vector);
        }else if ( GF ==  128 ) { ptr =  new decoder_naive< 128>(N, f_vector);
        }else if ( GF ==  256 ) { ptr =  new decoder_naive< 256>(N, f_vector);
        }else if ( GF ==  512 ) { ptr =  new decoder_naive< 512>(N, f_vector);
        }else if ( GF == 1024 ) { ptr =  new decoder_naive<1024>(N, f_vector);
        }else if ( GF == 2048 ) { ptr =  new decoder_naive<2048>(N, f_vector);
        }else if ( GF == 4096 ) {
            ptr =  new decoder_naive<4096>(N, f_vector);
        }
    }else if ( (type == "spec") || (type == "dec3") ) {
              if ( GF ==    8 ) { ptr =  new decoder_specialized<   8>(N, f_vector);
        }else if ( GF ==   16 ) { ptr =  new decoder_specialized<  16>(N, f_vector);
        }else if ( GF ==   32 ) { ptr =  new decoder_specialized<  32>(N, f_vector);
        }else if ( GF ==   64 ) { ptr =  new decoder_specialized<  64>(N, f_vector);
        }else if ( GF ==  128 ) { ptr =  new decoder_specialized< 128>(N, f_vector);
        }else if ( GF ==  256 ) { ptr =  new decoder_specialized< 256>(N, f_vector);
        }else if ( GF ==  512 ) { ptr =  new decoder_specialized< 512>(N, f_vector);
        }else if ( GF == 1024 ) { ptr =  new decoder_specialized<1024>(N, f_vector);
        }else if ( GF == 2048 ) { ptr =  new decoder_specialized<2048>(N, f_vector);
        }else if ( GF == 4096 ) {
            ptr =  new decoder_specialized<4096>(N, f_vector);
        }
    }else if ( (type == "pruned") || (type == "dec4") ) {
              if ( GF ==    8 ) { ptr =  new decoder_specialized_pruning<   8>(N, f_vector);
        }else if ( GF ==   16 ) { ptr =  new decoder_specialized_pruning<  16>(N, f_vector);
        }else if ( GF ==   32 ) { ptr =  new decoder_specialized_pruning<  32>(N, f_vector);
        }else if ( GF ==   64 ) { ptr =  new decoder_specialized_pruning<  64>(N, f_vector);
        }else if ( GF ==  128 ) { ptr =  new decoder_specialized_pruning< 128>(N, f_vector);
        }else if ( GF ==  256 ) { ptr =  new decoder_specialized_pruning< 256>(N, f_vector);
        }else if ( GF ==  512 ) { ptr =  new decoder_specialized_pruning< 512>(N, f_vector);
        }else if ( GF == 1024 ) { ptr =  new decoder_specialized_pruning<1024>(N, f_vector);
        }else if ( GF == 2048 ) { ptr =  new decoder_specialized_pruning<2048>(N, f_vector);
        }else if ( GF == 4096 ) {
            ptr =  new decoder_specialized_pruning<4096>(N, f_vector);
        }
    }else if ( type == "scl-genius" ) {
        if ( params.size() != 1 ) {
            printf("(EE) Error we should never be there [%s:%d]\n", __FILE__, __LINE__);
            printf("(EE) You asked for a [scl-genius] decoder that need one parameter\n");
            printf("(EE) -> scl-genius{#corrected bits}\n");
            exit( EXIT_FAILURE );
        }
        const int size = std::stoi( params[0] );
              if ( GF ==    8 ) { ptr =  new naive_scl_genie<   8>(N, f_vector, size);
        }else if ( GF ==   16 ) { ptr =  new naive_scl_genie<  16>(N, f_vector, size);
        }else if ( GF ==   32 ) { ptr =  new naive_scl_genie<  32>(N, f_vector, size);
        }else if ( GF ==   64 ) { ptr =  new naive_scl_genie<  64>(N, f_vector, size);
        }else if ( GF ==  128 ) { ptr =  new naive_scl_genie< 128>(N, f_vector, size);
        }else if ( GF ==  256 ) { ptr =  new naive_scl_genie< 256>(N, f_vector, size);
        }else if ( GF ==  512 ) { ptr =  new naive_scl_genie< 512>(N, f_vector, size);
        }else if ( GF == 1024 ) { ptr =  new naive_scl_genie<1024>(N, f_vector, size);
        }else if ( GF == 2048 ) { ptr =  new naive_scl_genie<2048>(N, f_vector, size);
        }else if ( GF == 4096 ) {
            ptr =  new naive_scl_genie<4096>(N, f_vector, size);
        }

    }else if ( type == "scf-oracle" ) {
        if ( params.size() != 1 ) {
            printf("(EE) Error we should never be there [%s:%d]\n", __FILE__, __LINE__);
            printf("(EE) You asked for a [scf-oracle] decoder that need one parameter\n");
            printf("(EE) -> scl-oracle{#corrected bits}\n");
            exit( EXIT_FAILURE );
        }
        const int size = std::stoi( params[0] );
        if ( GF ==    8 ) { ptr =  new naive_scf_oracle<   8>(N, f_vector, 1);
        }else if ( GF ==   16 ) { ptr =  new naive_scf_oracle<  16>(N, f_vector, size);
        }else if ( GF ==   32 ) { ptr =  new naive_scf_oracle<  32>(N, f_vector, size);
        }else if ( GF ==   64 ) { ptr =  new naive_scf_oracle<  64>(N, f_vector, size);
        }else if ( GF ==  128 ) { ptr =  new naive_scf_oracle< 128>(N, f_vector, size);
        }else if ( GF ==  256 ) { ptr =  new naive_scf_oracle< 256>(N, f_vector, size);
        }else if ( GF ==  512 ) { ptr =  new naive_scf_oracle< 512>(N, f_vector, size);
        }else if ( GF == 1024 ) { ptr =  new naive_scf_oracle<1024>(N, f_vector, size);
        }else if ( GF == 2048 ) { ptr =  new naive_scf_oracle<2048>(N, f_vector, size);
        }else if ( GF == 4096 ) {
            ptr =  new naive_scf_oracle<4096>(N, f_vector,size);
        }

    }else if ( type == "scf" ) {
        if ( params.size() != 1 ) {
            printf("(EE) Error we should never be there [%s:%d]\n", __FILE__, __LINE__);
            printf("(EE) You asked for a [scf] decoder that need one parameter\n");
            printf("(EE) -> scf{#total iterations}\n");
            exit( EXIT_FAILURE );
        }
        const int size = std::stoi( params[0] );
        if ( GF ==    8 ) { ptr =  new naive_scf_oracle<   8>(N, f_vector, 1);
        }else if ( GF ==   16 ) { ptr =  new naive_scf<  16>(N, f_vector, size);
        }else if ( GF ==   32 ) { ptr =  new naive_scf<  32>(N, f_vector, size);
        }else if ( GF ==   64 ) { ptr =  new naive_scf<  64>(N, f_vector, size);
        }else if ( GF ==  128 ) { ptr =  new naive_scf< 128>(N, f_vector, size);
        }else if ( GF ==  256 ) { ptr =  new naive_scf< 256>(N, f_vector, size);
        }else if ( GF ==  512 ) { ptr =  new naive_scf< 512>(N, f_vector, size);
        }else if ( GF == 1024 ) { ptr =  new naive_scf<1024>(N, f_vector, size);
        }else if ( GF == 2048 ) { ptr =  new naive_scf<2048>(N, f_vector, size);
        }else if ( GF == 4096 ) {
            ptr =  new naive_scf_oracle<4096>(N, f_vector,size);
        }
    }
    if ( ptr == nullptr ) {
        printf("(EE) Error we should never be there [%s:%d]\n", __FILE__, __LINE__);
        printf("(EE) An issue was detected with following parameters:\n");
        printf("(EE) - dec = %s\n", type.c_str());
        printf("(EE) - N   = %d\n", N);
        printf("(EE) - GF  = %d\n", GF);
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
