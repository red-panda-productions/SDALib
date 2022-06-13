#pragma once
#include <vector>
#include <string>
#include "sdalib_export.h"

/// @brief Struct for the actions the AI can do
struct SDALIB_EXPORT SDAAction
{
public:
    // The needed variables
    float Steer = 0;
    float Brake = 0;
    int Gear = 0;
    float Accel = 0;

    void Serialize(char* p_buffer, int p_bufferSize, int& p_sbufferSize) const;

    static void GetOrder(std::vector<std::string>& p_order);
};
