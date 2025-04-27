#include "weather_forecast/weather_forecast_provider.h"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <string>

#include "weather_forecast/city_coordinates_provider.h"
#include "weather_forecast/location.h"

namespace weather_forecast {
WeatherForecastProvider::WeatherForecastProvider(std::string weather_api_url,
                                                 std::string weather_api_key,
                                                 std::string city_api_url,
                                                 std::string city_api_key)
    : api_url_(std::move(weather_api_url)),
      api_key_(std::move(weather_api_key)),
      city_coordinates_provider_(std::move(city_api_url),
                                 std::move(city_api_key)),
      is_ok_(false) {}

bool WeatherForecastProvider::FetchForecastData(const ForecastRequest request) {
  is_ok_ = false;
  forecast_data_.clear();

  if (!city_coordinates_provider_.FetchCoordinates(request.city_name)) {
    error_message_ = city_coordinates_provider_.GetErrorMessage();
    return false;
  }

  cpr::Response response = cpr::Get(
      cpr::Url{api_url_},
      cpr::Parameters{
          {"latitude",
           std::to_string(
               city_coordinates_provider_.GetCityCoordinates().latitude)},
          {"longitude",
           std::to_string(
               city_coordinates_provider_.GetCityCoordinates().longitude)},
          {"timezone", "auto"},
          {"forecast_days", std::to_string(request.forecast_days)},
          {"hourly", "temperature_2m"},
          {"hourly", "wind_speed_10m"},
          {"hourly", "weather_code"},
      });

  if (response.status_code != 200) {
    error_message_ = response.error.message.empty()
                         ? "API request failed with status code: " +
                               std::to_string(response.status_code)
                         : response.error.message;
    return false;
  }

  const nlohmann::json data = nlohmann::json::parse(response.text);

  if (data.empty()) {
    error_message_ = "No forecast data found";
    return false;
  }

  if (!data.contains("hourly")) {
    error_message_ = "Invalid forecast data format in API response";
    return false;
  }

  // TODO: move to parse function
  for (size_t i = 0; i < request.forecast_days; ++i) {
    forecast_data_.emplace_back();

    forecast_data_.back().date =
        static_cast<std::string>(data["hourly"]["time"][i * 24])
            .substr(0, sizeof("YYYY-MM-DD") - 1);

    const auto& temperatures = data["hourly"]["temperature_2m"];
    forecast_data_.back().morning.temperature = temperatures[i * 24 + 6];
    forecast_data_.back().afternoon.temperature = temperatures[i * 24 + 12];
    forecast_data_.back().evening.temperature = temperatures[i * 24 + 17];
    forecast_data_.back().night.temperature = temperatures[i * 24 + 23];

    const auto& wind_speeds = data["hourly"]["wind_speed_10m"];
    forecast_data_.back().morning.wind_speed = wind_speeds[i * 24 + 6];
    forecast_data_.back().afternoon.wind_speed = wind_speeds[i * 24 + 12];
    forecast_data_.back().evening.wind_speed = wind_speeds[i * 24 + 17];
    forecast_data_.back().night.wind_speed = wind_speeds[i * 24 + 23];

    const auto& weather_codes = data["hourly"]["weather_code"];
    forecast_data_.back().morning.weather_code = weather_codes[i * 24 + 6];
    forecast_data_.back().afternoon.weather_code = weather_codes[i * 24 + 12];
    forecast_data_.back().evening.weather_code = weather_codes[i * 24 + 17];
    forecast_data_.back().night.weather_code = weather_codes[i * 24 + 23];
  }

  is_ok_ = true;

  return IsOk();
}

ForecastData WeatherForecastProvider::GetForecastData(size_t day_offset) {
  return forecast_data_[day_offset];
}

std::string WeatherForecastProvider::GetErrorMessage() {
  return error_message_;
}

bool WeatherForecastProvider::IsOk() { return is_ok_; }
}  // namespace weather_forecast
