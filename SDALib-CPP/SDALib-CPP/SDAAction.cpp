/*
 * This program has been developed by students from the bachelor Computer Science at
 * Utrecht University within the Software Project course.
 * © Copyright Utrecht University (Department of Information and Computing Sciences)
 */

#include "SDAAction.hpp"

/// @brief                Serializes the data into a buffer
/// @param  p_buffer	  The buffer that will store the data
/// @param  p_bufferSize  The size of the buffer
/// @param  p_sbufferSize The result size of the string
void SDAAction::Serialize(char* p_buffer, int p_bufferSize, int& p_sbufferSize) const
{
    std::vector<std::string> data;
    data.push_back(std::to_string(Steer));
    data.push_back(std::to_string(Accel));
    data.push_back(std::to_string(Brake));
    data.push_back(std::to_string(Gear));

    msgpack::sbuffer sbuffer;
    msgpack::pack(sbuffer, data);

    sbufferCopy(sbuffer, p_buffer, p_bufferSize);
    p_sbufferSize = static_cast<int>(sbuffer.size());
}

/// @brief			Gets the order of the actions and stores it into the vector
/// @param  p_order The order vector
void SDAAction::GetOrder(std::vector<std::string>& p_order)
{
    p_order.emplace_back("Steer");
    p_order.emplace_back("Accel");
    p_order.emplace_back("Brake");
    p_order.emplace_back("Gear");
}