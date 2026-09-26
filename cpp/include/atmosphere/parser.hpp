#pragma once
#include "types.hpp"
#include <string>

namespace atmosphere {

class Parser {
public:
    SatelliteData parse(const std::string& raw);
};

} // namespace atmosphere