#pragma once

#include "Measurement.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class Packet
{
public:
    explicit Packet(const std::string& id, std::size_t maxCapacity = 10);

    Packet() = delete;
    Packet(const Packet&) = default;
    Packet& operator=(const Packet&) = default;
    ~Packet();

    const std::string& GetId() const { return m_id; }
    std::size_t GetCount() const { return m_measurements.size(); }

    bool AddMeasurement(const std::shared_ptr<Measurement>& measurement);
    bool RemoveMeasurement(const std::shared_ptr<Measurement>& measurement);
    void Print() const;

private:
    std::string m_id;
    std::vector<std::weak_ptr<Measurement>> m_measurements;
    std::size_t m_maxCapacity;
};
