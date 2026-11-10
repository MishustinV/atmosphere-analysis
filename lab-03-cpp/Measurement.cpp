#include "Measurement.hpp"
#include <cmath>
#include <iostream>

Measurement::Measurement()
    : Measurement(0.0, 0.0, 0.0, "1970-01-01T00:00", "unknown")
{
}

Measurement::Measurement(double value, double latitude, double longitude,
                         const std::string& timestamp, const std::string& parameter)
    : m_value{ value }, m_latitude{ latitude }, m_longitude{ longitude },
      m_timestamp{ timestamp }, m_parameter{ parameter }
{
    if (std::isnan(m_value) || std::isinf(m_value))
    {
        std::cerr << "Ошибка: некорректное значение измерения, установлено 0" << std::endl;
        m_value = 0.0;
    }
    std::cout << "Measurement создан: " << m_parameter << " = " << m_value << std::endl;
}

Measurement::~Measurement()
{
    std::cout << "Measurement уничтожен: " << m_parameter << " = " << m_value << std::endl;
}

bool Measurement::IsNormal() const
{
    if (m_parameter == "temperature" && (m_value < -273.15 || m_value > 100.0)) return false;
    if (m_parameter == "pressure" && m_value < 0.0) return false;
    if (m_parameter == "humidity" && (m_value < 0.0 || m_value > 100.0)) return false;
    return true;
}

void Measurement::Print() const
{
    std::cout << "Измерение: " << m_parameter
              << " = " << m_value
              << " в (" << m_latitude << ", " << m_longitude << ")"
              << " время: " << m_timestamp
              << (IsNormal() ? " [норма]" : " [аномалия]") << std::endl;
}
