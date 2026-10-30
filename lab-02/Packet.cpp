#include "Packet.hpp"
#include <iostream>

Packet::Packet(const std::string& id, std::size_t maxCapacity)
    : m_id{ id }, m_maxCapacity{ maxCapacity }
{
    std::cout << "Packet создан: " << m_id << std::endl;
}

Packet::~Packet()
{
    std::cout << "Packet уничтожен: " << m_id
              << " (измерения остаются живы)" << std::endl;
}

bool Packet::AddMeasurement(Measurement* measurement)
{
    if (measurement == nullptr)
    {
        std::cerr << "Ошибка: нельзя добавить null-измерение" << std::endl;
        return false;
    }
    if (m_measurements.size() >= m_maxCapacity)
    {
        std::cerr << "Ошибка: пакет " << m_id << " переполнен (максимум "
                  << m_maxCapacity << ")" << std::endl;
        return false;
    }
    m_measurements.push_back(measurement);
    return true;
}

bool Packet::RemoveMeasurement(Measurement* measurement)
{
    for (auto it = m_measurements.begin(); it != m_measurements.end(); ++it)
    {
        if (*it == measurement)
        {
            m_measurements.erase(it);
            return true;
        }
    }
    return false;
}

void Packet::Print() const
{
    std::cout << "Пакет " << m_id << " содержит " << m_measurements.size()
              << " измерений:" << std::endl;
    for (const auto* m : m_measurements)
    {
        m->Print();
    }
}
