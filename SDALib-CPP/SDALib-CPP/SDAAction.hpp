/*
 * This program has been developed by students from the bachelor Computer Science at
 * Utrecht University within the Software Project course.
 * © Copyright Utrecht University (Department of Information and Computing Sciences)
 */

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
