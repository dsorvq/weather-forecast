#pragma once

#include <string>

namespace weather_forecast {
struct Coordinates {
  float latitude{};
  float longitude{};
};

struct Location {
  std::string city_name;
  Coordinates coordinates;
};
}  // namespace weather_forecast
