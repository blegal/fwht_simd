#pragma once
#include "definitions/custom_types.hpp"
//
//
//
//
//
class decoder {
public:
    virtual ~decoder() {};
    virtual void execute(void* channel, uint16_t *  decoded) = 0;
    virtual int GF() = 0;
};
//
//
//
//
//
