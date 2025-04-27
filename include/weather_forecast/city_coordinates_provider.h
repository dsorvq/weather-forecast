#pragma once
#include <string>

#include "location.h"

namespace weather_forecast {
class CityCoordinatesProvider {
 public:
  CityCoordinatesProvider(std::string api_url, std::string api_key);

  bool FetchCoordinates(const std::string& city_name);

  Coordinates GetCityCoordinates() const;

  Location GetCityLocation() const;

  std::string GetErrorMessage();

  bool IsOk();

 private:
  const std::string api_url_;
  const std::string api_key_;
  bool is_ok_{false};
  Location location_;
  std::string error_message_;
};
}  // namespace weather_forecast
