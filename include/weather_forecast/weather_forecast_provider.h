#pragma once
#include <string>

namespace weather_forecast {
struct ForecastRequest {
  std::string city;
  std::string country_code;          // ISO 3166 country code
  std::optional<std::string> state;  // for countries with states/regions
  int days_to_forecast;  // number of days to forecast (1-16 typically)
  TemperatureUnit unit;
  WeatherProvider provider;
  bool include_alerts;
  bool hourly_breakdown;
};

class WeatherForecastProvider {
 public:
  explicit WeatherForecastProvider(std::string api_url, std::string api_key);

  bool Fetch(const);
};
}  // namespace weather_forecast
