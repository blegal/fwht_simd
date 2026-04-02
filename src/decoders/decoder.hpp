#pragma once
#include <iostream>
#include <ostream>

#include "definitions/custom_types.hpp"
//
//
//
//
//
class decoder {
public:
    virtual ~decoder() {};
    virtual void execute(void * channel, uint16_t * decoded) = 0;
    virtual int  GF()                                        = 0;
    virtual void setResult(const uint16_t * decoded) { }
};
//
//
//
//
//
