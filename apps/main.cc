#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

#include "../include/weather_forecast/weather_forecast_provider.h"

using namespace ftxui;

class WeatherApp {
 public:
  WeatherApp(const std::string& weather_api_url,
             const std::string& weather_api_key,
             const std::string& city_api_url, const std::string& city_api_key)
      : provider_(weather_api_url, weather_api_key, city_api_url,
                  city_api_key) {
    input_city_ =
        Input(&state_.city_name, "Enter city name", InputOption::Spacious());

    button_search_ = Button(
        " Search ", [this] { OnSearchClicked(); }, ButtonOption::Animated());

    layout_ = Container::Horizontal({
        input_city_,
        button_search_,
    });

    renderer_ = Renderer(layout_, [this] { return RenderUI(); });
  }

  void Run() {
    auto screen = ScreenInteractive::Fullscreen();
    screen.Loop(renderer_);
  }

 private:
  struct AppState {
    std::string city_name;
    std::string status_message = "Enter a city name and press Search";
    bool show_results = false;
  };

  void OnSearchClicked() {
    if (state_.city_name.empty()) {
      state_.status_message = "Please enter a city name";
      state_.show_results = false;
      return;
    }

    provider_.FetchForecastData({state_.city_name, 3});

    if (!provider_.IsOk()) {
      state_.status_message = "Error: " + provider_.GetErrorMessage();
      state_.show_results = false;
    } else {
      state_.status_message =
          "Successfully fetched weather forecast for " + state_.city_name;
      state_.show_results = true;
    }
  }

  Element RenderUI() {
    std::vector<Element> children;

    children.push_back(hbox({input_city_->Render() | flex,
                             button_search_->Render() | color(Color::Green)}) |
                       border);

    children.push_back(text(state_.status_message) |
                       color(state_.show_results ? Color::Green : Color::Red));

    if (state_.show_results) {
      std::vector<Element> day_forecasts;
      for (int day = 0; day < 3; ++day) {
        day_forecasts.push_back(
            ForecastWindow(provider_.GetForecastData(day))->Render());
      }
      children.push_back(vbox(day_forecasts));
    }

    return vbox(children) | border | bgcolor(Color::Black);
  }

  Component ForecastContent(const weather_forecast::ForecastData& data) {
    auto make_section = [&](const std::string& name, const auto& day_data) {
      return Renderer([=] {
        return vbox({
                   text(name) | bold | center,
                   text("Temp: " + std::to_string(day_data.temperature) + "°C"),
                   text("Wind Speed: " + std::to_string(day_data.wind_speed) +
                        "m/s"),
                   text("Humidity: " + std::to_string(day_data.humidity) + "%"),
               }) |
               flex | border;
      });
    };

    return Container::Horizontal(
               {make_section("Morning", data.morning) | flex,
                make_section("Afternoon", data.afternoon) | flex,
                make_section("Evening", data.evening) | flex,
                make_section("Night", data.night) | flex}) |
           flex;
  }

  Component ForecastWindow(const weather_forecast::ForecastData& data) {
    auto content = ForecastContent(data);
    return Renderer(content, [=] {
      // TODO: center just doesn't work
      return window(text(data.date), content->Render() | frame);
    });
  }

  AppState state_;
  weather_forecast::WeatherForecastProvider provider_;
  Component input_city_;
  Component button_search_;
  Component layout_;
  Component renderer_;
};

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <api-ninjas.com KEY>\n";
    return 1;
  }

  const std::string forecast_url{"https://api.open-meteo.com/v1/forecast"};
  const std::string forecast_key{""};
  const std::string city_url{"https://api.api-ninjas.com/v1/city"};
  const std::string city_key{argv[1]};

  WeatherApp app(forecast_url, forecast_key, city_url, city_key);
  app.Run();

  return 0;
}
