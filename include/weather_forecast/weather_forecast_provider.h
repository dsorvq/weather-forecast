#pragma once
#include <string>
#include <vector>

#include "city_coordinates_provider.h"

namespace weather_forecast {
struct ForecastRequest {
  std::string city_name;
  int forecast_days;
};

struct ForecastPoint {
  int temperature;
};

struct ForecastData {
  std::string date;
  ForecastPoint morning;
  ForecastPoint afternoon;
  ForecastPoint evening;
  ForecastPoint night;
};

class WeatherForecastProvider {
 public:
  WeatherForecastProvider(std::string weather_api_url,
                          std::string weather_api_key,
                          std::string city_api_url, std::string city_api_key);

  bool FetchForecastData(const ForecastRequest request);

  ForecastData GetForecastData(size_t day_offset);

  std::string GetErrorMessage();

  bool IsOk();

 private:
  const std::string api_url_;
  const std::string api_key_;
  bool is_ok_{false};
  std::vector<ForecastData> forecast_data_;
  std::string error_message_;
  CityCoordinatesProvider city_coordinates_provider_;
};
}  // namespace weather_forecast
