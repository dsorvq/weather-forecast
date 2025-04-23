#include <cpr/cpr.h>
#include <optional>

#include "../../include/weather_forecast/network/city.h"

std::optional<weather_forecast::Coordinates> weather_forecast::GetCityCoordinates(const std::string& city_name) {
  cpr::Response r = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
      cpr::Parameters{{"name", city_name}},
      cpr::Header{{"X-Api-Key", "yv/jaTFCPwyF9aIK5d4d8w==rIx0OfCVhThhG9Kb"}}
      );

  if (r.status_code != 200 || r.text.empty()) {
    return std::nullopt; 
  }

  // TODO: currently a place holder
  // need to process json 
  weather_forecast::Coordinates coordinates{0, 0};

  return coordinates;
}
