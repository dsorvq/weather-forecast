#include "../include/weather_forecast/city_coordinates_provider.h"

#include <cpr/cpr.h>

#include <nlohmann/json.hpp>

namespace weather_forecast {
CityCoordinatesProvider::CityCoordinatesProvider(std::string api_url,
                                                 std::string api_key)
    : api_url_(std::move(api_url)),
      api_key_(std::move(api_key)),
      is_ok_(false) {}

bool CityCoordinatesProvider::FetchCoordinates(
    const std::string& city_name) {
  is_ok_ = false;

  cpr::Response response =
      cpr::Get(cpr::Url{api_url_}, cpr::Parameters{{"name", city_name}},
               cpr::Header{{"X-Api-Key", api_key_}});

  if (response.status_code != 200) {
    error_message_ = response.error.message.empty()
                         ? "API request failed with status code: " +
                               std::to_string(response.status_code)
                         : response.error.message;
    return false;
  }

  const nlohmann::json data = nlohmann::json::parse(response.text);

  if (data.empty() || !data.is_array()) {
    error_message_ = "No location data found for city: " + city_name;
    return false;
  }

  if (!data[0].contains("name") || !data[0].contains("latitude") ||
      !data[0].contains("longitude")) {
    error_message_ = "Invalid location data format in API response";
    return false;
  }

  location_ = Location{data[0]["name"],
                       Coordinates{data[0]["latitude"], data[0]["longitude"]}};

  is_ok_ = true;

  return IsOk();
}

Coordinates CityCoordinatesProvider::GetCityCoordinates() const {
  return location_.coordinates;
}

Location CityCoordinatesProvider::GetCityLocation() const { return location_; }

std::string CityCoordinatesProvider::GetErrorMessage() {
  return error_message_;
}

bool CityCoordinatesProvider::IsOk() { return is_ok_; }
}  // namespace weather_forecast
