#pragma once
#include "definitions/custom_types.hpp"
//
//
//
//
//
template<int gf_size>
class decoder {
public:
    virtual ~decoder() {};
    virtual void execute(void* channel, uint16_t *  decoded) = 0;
    int GF() const {return gf_size;};
};
//
//
//
//
//
