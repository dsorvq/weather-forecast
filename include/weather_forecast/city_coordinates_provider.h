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

struct CityCoordinatesRequest {
  std::string city_name;
};

class CityCoordinatesProvider {
 public:
  explicit CityCoordinatesProvider(std::string api_url, std::string api_key);

  bool FetchCoordinates(const CityCoordinatesRequest& request);

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
