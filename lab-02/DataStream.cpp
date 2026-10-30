#include "DataStream.hpp"
#include <iostream>

DataStream::DataStream(const std::string& satelliteName, std::size_t maxPackets)
    : m_satelliteName{ satelliteName }, m_maxPackets{ maxPackets }
{
    std::cout << "DataStream создан для спутника: " << m_satelliteName << std::endl;
}

DataStream::~DataStream()
{
    std::cout << "DataStream уничтожен: " << m_satelliteName
              << " (все пакеты уничтожены вместе с ним)" << std::endl;
}

bool DataStream::AddPacket(const Packet& packet)
{
    if (m_packets.size() >= m_maxPackets)
    {
        std::cerr << "Ошибка: поток " << m_satelliteName << " переполнен (максимум "
                  << m_maxPackets << " пакетов)" << std::endl;
        return false;
    }
    m_packets.push_back(packet);
    return true;
}

void DataStream::Print() const
{
    std::cout << "Поток спутника " << m_satelliteName
              << " содержит " << m_packets.size() << " пакетов" << std::endl;
    for (const auto& p : m_packets)
    {
        p.Print();
    }
}
