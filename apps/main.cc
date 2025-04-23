#include <iostream>

#include "../include/weather_forecast/network/city.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: ";
    std::cout << argv[0] << ' ' << "<city_name>" << std::endl;
    return 1;
  }

  std::string city_name{argv[1]};
  auto coordinates = weather_forecast::GetCityCoordinates(city_name);

  if (!coordinates.has_value()) {
    std::cout << "City not found" << '\n';
    return 1;
  }

  std::cout << "Coordinates of city " << city_name << '\n';
  std::cout << "  Latitude:" << coordinates->latitude << '\n';
  std::cout << "  Longitude:" << coordinates->longitude << '\n';

  return 0;
}
