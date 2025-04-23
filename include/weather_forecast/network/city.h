#include <optional>
#include <string_view>

namespace weather_forecast {
  struct Coordinates {
    float latitude{};
    float longitude{};
  };

  std::optional<Coordinates> GetCityCoordinates(std::string_view city_name);
}
