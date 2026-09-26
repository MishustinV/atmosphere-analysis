#pragma once

namespace atmosphere {

struct SatelliteData {
    double timestamp;
    double latitude;
    double longitude;
    double temperature;
    double pressure;
    double humidity;
};

} // namespace atmosphere