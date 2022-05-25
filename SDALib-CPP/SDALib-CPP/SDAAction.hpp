#pragma once
#include "msgpack.hpp"
#include <vector>
#include <string>
#include "sdalib_export.h"

/// @brief				      Copies a msgpack buffer
/// @param  p_sbuffer		  The msgpack buffer
/// @param  p_destination     The destination buffer
///	@param  p_destinationSize The size of the destination buffer
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

/// @brief Struct for the actions the AI can do
struct SDALIB_EXPORT SDAAction
{
public:
    // The needed variables
    float Steer = 0;
    float Brake = 0;
    int Gear = 0;
    float Accel = 0;

    /// @brief				  Serializes the data into a buffer
    /// @param  p_buffer	  The buffer that will store the data
    /// @param  p_bufferSize  The size of the buffer
    /// @param  p_sbufferSize The result size of the string
    void Serialize(char* p_buffer, int p_bufferSize, int& p_sbufferSize) const;

    /// @brief			Gets the order of the actions and stores it into the vector
    /// @param  p_order The order vector
    static void GetOrder(std::vector<std::string>& p_order);
};


