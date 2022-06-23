#pragma once
#include "msgpack.hpp"
#include <vector>
#include <string>
#include "sdalib_export.h"

/// @brief				      Copies a msgpack buffer
/// @param  p_sbuffer		  The msgpack buffer
/// @param  p_destination     The destination buffer
/// @param  p_destinationSize The size of the destination buffer
inline void sbufferCopy(const msgpack::sbuffer& p_sbuffer, char* p_destination, int p_destinationSize)
{
    const size_t size = p_sbuffer.size();
    assert(size < p_destinationSize);
    const char* newData = p_sbuffer.data();
    for (int i = 0; i < size; i++)
    {
        p_destination[i] = newData[i];
    }
    p_destination[size] = '\0';
}