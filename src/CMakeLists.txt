add_library(weather_forecast_library 
  city_coordinates_provider.cc
  weather_forecast_provider.cc
)

target_include_directories(weather_forecast_library PUBLIC ../include)

target_compile_features(weather_forecast_library PUBLIC cxx_std_20)

target_link_libraries(weather_forecast_library PRIVATE cpr::cpr)
target_link_libraries(weather_forecast_library PUBLIC nlohmann_json::nlohmann_json)
