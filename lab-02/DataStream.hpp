#pragma once

#include "Packet.hpp"
#include <string>
#include <vector>

class DataStream
{
public:
    explicit DataStream(const std::string& satelliteName, std::size_t maxPackets = 5);

    DataStream() = default;
    DataStream(const DataStream&) = default;
    DataStream& operator=(const DataStream&) = default;
    ~DataStream();

    const std::string& GetSatelliteName() const { return m_satelliteName; }
    std::size_t GetCount() const { return m_packets.size(); }

    bool AddPacket(const Packet& packet);
    void Print() const;

private:
    std::string m_satelliteName;
    std::vector<Packet> m_packets;
    std::size_t m_maxPackets;
};
