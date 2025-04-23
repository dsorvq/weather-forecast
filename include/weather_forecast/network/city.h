#pragma once
#include <optional>
#include <string>

namespace weather_forecast {
  struct Coordinates {
    float latitude{};
    float longitude{};
  };

  std::optional<Coordinates> GetCityCoordinates(const std::string& city_name);
}
