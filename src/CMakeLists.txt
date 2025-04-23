add_library(weather_forecast_library network/city.cc)

target_include_directories(weather_forecast_library PUBLIC ../include)

target_compile_features(weather_forecast_library PUBLIC cxx_std_20)
