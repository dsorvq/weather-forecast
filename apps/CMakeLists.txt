add_executable(app main.cc)

target_compile_features(app PRIVATE cxx_std_20)

target_link_libraries(app PRIVATE weather_forecast_library)
target_link_libraries(app
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
