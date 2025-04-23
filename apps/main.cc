#include <iostream>

#include "../include/weather_forecast/network/city_coordinates_provider.h"

int main(int argc, char** argv) {
  if (argc != 4) {
    std::cout << "Usage: ";
    std::cout << argv[0] << ' ';
    std::cout << "<api_url> <api_key> <city_name>" << std::endl;
    return 1;
  }

  std::string api_url{argv[1]};
  std::string api_key{argv[2]};
  std::string city_name{argv[3]};

  weather_forecast::CityCoordinatesProvider provider{api_url, api_key};
  provider.FetchCoordinates({city_name});

  if (!provider.IsOk()) {
    std::cout << provider.GetErrorMessage() << '\n';
    return 1;
  }

  auto location = provider.GetCityLocation();

  std::cout << "Coordinates of city " << location.city_name << '\n';
  std::cout << "  Latitude:" << location.coordinates.latitude << '\n';
  std::cout << "  Longitude:" << location.coordinates.longitude << '\n';

  return 0;
}
