cmake_minimum_required(VERSION 3.15)

project(
  weather_forecast 
  VERSION 0.1
  DESCRIPTION "weather forecast"
  LANGUAGES CXX
  )


set(CMAKE_CXX_EXTENSIONS OFF)

# include(FetchContent)

add_subdirectory(src)

add_subdirectory(apps)
