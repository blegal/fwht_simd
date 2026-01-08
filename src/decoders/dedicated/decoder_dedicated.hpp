#pragma once
#include "decoders/decoder.hpp"
#include "definitions/custom_types.hpp"
#include "features/fwht/fwht_counter.hpp"
#include "frozen_tree.hpp"
#include <vector>

template <int gf_size>
class decoder_dedicated : public decoder {
public:
    decoder_dedicated();
    decoder_dedicated(const int n, const int * frozen_symb);
    virtual ~decoder_dedicated();

    virtual void execute(void * channel, uint16_t * decoded);
    virtual int  GF() { return gf_size; }

private:
    symbols_s<gf_size> * internal;
    uint16_t *           symbols;
    //    int*       frozen;

    //    const int N;
    //    int           f_tree_cnt;

    //    frozen_tree * f_tree;
    //    frozen_tree pruned_tree;

    static const int N_gen;
    static const int K_gen;
};
