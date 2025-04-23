#include <cpr/cpr.h>

#include "../../include/weather_forecast/network/city_coordinates_provider.h"

namespace weather_forecast {
CityCoordinatesProvider::CityCoordinatesProvider(
    std::string api_url, std::string api_key) 
  : api_url_(std::move(api_url)),
    api_key_(std::move(api_key)), 
    is_ok_(false) {}

bool CityCoordinatesProvider::FetchCoordinates(
    const CityCoordinatesRequest& request) {
  is_ok_ = false;

  cpr::Response response = cpr::Get(cpr::Url{api_url_},
      cpr::Parameters{{"name", request.city_name}},
      cpr::Header{{"X-Api-Key", api_key_}}
      );

  if (response.status_code != 200 || response.text.empty()) {
    error_message_ = response.error.message;
    return false;
  }

  // TODO: currently a place holder
  // need to process json 
  location_ = Location{request.city_name, {0, 0}};

  is_ok_ = true;

  return IsOk();
}

Coordinates CityCoordinatesProvider::GetCityCoordinates() const {
  return location_.coordinates;
}

Location CityCoordinatesProvider::GetCityLocation() const {
  return location_;
}

std::string CityCoordinatesProvider::GetErrorMessage() {
  return error_message_;
}

bool CityCoordinatesProvider::IsOk() {
  return is_ok_;
}
}
