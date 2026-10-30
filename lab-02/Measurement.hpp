#pragma once

#include <string>

class Measurement
{
public:
    Measurement(double value, double latitude, double longitude,
                const std::string& timestamp, const std::string& parameter);

    Measurement() = delete;
    Measurement(const Measurement&) = default;
    Measurement& operator=(const Measurement&) = default;
    ~Measurement();

    double GetValue() const { return m_value; }
    double GetLatitude() const { return m_latitude; }
    double GetLongitude() const { return m_longitude; }
    const std::string& GetTimestamp() const { return m_timestamp; }
    const std::string& GetParameter() const { return m_parameter; }

    bool IsNormal() const;
    void Print() const;

private:
    double m_value;
    double m_latitude;
    double m_longitude;
    std::string m_timestamp;
    std::string m_parameter;
};
