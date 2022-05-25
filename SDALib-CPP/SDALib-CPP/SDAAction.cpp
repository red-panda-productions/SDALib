#include "SDAAction.hpp"

void SDAAction::Serialize(char* p_buffer, int p_bufferSize, int& p_sbufferSize) const
{
    std::vector<std::string> data;
    data.push_back(std::to_string(Steer));
    data.push_back(std::to_string(Brake));
    data.push_back(std::to_string(Gear));
    data.push_back(std::to_string(Accel));

    msgpack::sbuffer sbuffer;
    msgpack::pack(sbuffer, data);

    sbufferCopy(sbuffer, p_buffer, p_bufferSize);
    p_sbufferSize = static_cast<int>(sbuffer.size());
}

void SDAAction::GetOrder(std::vector<std::string>& p_order)
{
    p_order.emplace_back("Steer");
    p_order.emplace_back("Brake");
    p_order.emplace_back("Gear");
    p_order.emplace_back("Accel");
}