#pragma once
#include <string>
#include <vector>

#include "location.h"

namespace weather_forecast {
struct ForecastRequest {
  Coordinates coordinates;
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
  WeatherForecastProvider(std::string api_url, std::string api_key);

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
};
}  // namespace weather_forecast
