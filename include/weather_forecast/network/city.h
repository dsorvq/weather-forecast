#pragma once
#include <optional>
#include <string>

namespace weather_forecast {
  struct Coordinates {
    float latitude{};
    float longitude{};
  };

  // TOOD: implements this
  /*
  struct Location {
    std::string city_name;
    Coordinates coordinates;
  };

  class CityCoordinateProvider {
   public:
    explicit CityCoordinateProvider(std::string api_key);

    bool FetchCoordinates();
    bool FetchCoordinates(const std::string& city_name);

    Coordinates GetCityCoordinates();

    bool IsOk();

   private:
    bool is_ok_{false};
    Location location_;
  };
  */

  std::optional<Coordinates> GetCityCoordinates(const std::string& city_name);
}
