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

bool Packet::AddMeasurement(const std::shared_ptr<Measurement>& measurement)
{
    if (!measurement)
    {
        std::cerr << "Ошибка: нельзя добавить пустое измерение" << std::endl;
        return false;
    }
    if (m_measurements.size() >= m_maxCapacity)
    {
        std::cerr << "Ошибка: пакет " << m_id << " переполнен (максимум "
                  << m_maxCapacity << ")" << std::endl;
        return false;
    }
    m_measurements.emplace_back(measurement);
    return true;
}

bool Packet::RemoveMeasurement(const std::shared_ptr<Measurement>& measurement)
{
    if (!measurement)
    {
        return false;
    }
    for (auto it = m_measurements.begin(); it != m_measurements.end(); ++it)
    {
        if (it->lock() == measurement)
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
    for (const std::weak_ptr<Measurement>& weak : m_measurements)
    {
        if (std::shared_ptr<Measurement> measurement = weak.lock())
        {
            measurement->Print();
        }
        else
        {
            std::cout << "Измерение уничтожено (пакет им не владел)" << std::endl;
        }
    }
}
