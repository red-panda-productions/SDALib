#pragma once
#include "msgpack.hpp"
#include <vector>
#include <string>
#include "sdalib_export.h"

#define SDA_STEER_VAR   float Steer = 0;
#define SDA_STEER_PACK  data.push_back(std::to_string(Steer));
#define SDA_STEER_ORDER p_order.emplace_back("Steer");

#define SDA_BRAKE_VAR   float Brake = 0;
#define SDA_BRAKE_PACK  data.push_back(std::to_string(Brake));
#define SDA_BRAKE_ORDER p_order.emplace_back("Brake");

#define SDA_GEAR_VAR   int Gear = 0;
#define SDA_GEAR_PACK  data.push_back(std::to_string(Gear));
#define SDA_GEAR_ORDER p_order.emplace_back("Gear");

#define SDA_ACCEL_VAR   float Accel = 0;
#define SDA_ACCEL_PACK  data.push_back(std::to_string(Accel));
#define SDA_ACCEL_ORDER p_order.emplace_back("Accel");

/// @brief				      Copies a msgpack buffer
/// @param  p_sbuffer		  The msgpack buffer
/// @param  p_destination     The destination buffer
///	@param  p_destinationSize The size of the destination buffer
inline void sbufferCopy(const msgpack::sbuffer& p_sbuffer, char* p_destination, int p_destinationSize)
{
    int size = static_cast<int>(p_sbuffer.size());
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
    SDA_STEER_VAR
    SDA_BRAKE_VAR
    SDA_GEAR_VAR
    SDA_ACCEL_VAR

    /// @brief				  Serializes the data into a buffer
    /// @param  p_buffer	  The buffer that will store the data
    /// @param  p_bufferSize  The size of the buffer
    /// @param  p_sbufferSize The result size of the string
    void Serialize(char* p_buffer, int p_bufferSize, int& p_sbufferSize) const
    {
        std::vector<std::string> data;
        SDA_STEER_PACK
        SDA_ACCEL_PACK
        SDA_BRAKE_PACK
        SDA_GEAR_PACK

        msgpack::sbuffer sbuffer;
        msgpack::pack(sbuffer, data);

        sbufferCopy(sbuffer, p_buffer, p_bufferSize);
        p_sbufferSize = sbuffer.size();
    }

    /// @brief			Gets the order of the actions and stores it into the vector
    /// @param  p_order The order vector
    static void GetOrder(std::vector<std::string>& p_order)
    {
        SDA_STEER_ORDER
        SDA_ACCEL_ORDER
        SDA_BRAKE_ORDER
        SDA_GEAR_ORDER
    }
};
