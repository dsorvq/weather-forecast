#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

#include "../include/weather_forecast/city_coordinates_provider.h"
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
    auto screen = ScreenInteractive::TerminalOutput();
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
          "Successfully fetched coordinates for " + state_.city_name;
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
      children.push_back(hbox(day_forecasts));
    }

    return vbox(children) | border | bgcolor(Color::Black);
  }
  Component ForecastContent(const weather_forecast::ForecastData& data) {
    auto morning = Renderer([&] {
      return vbox(
          {text("morning") | bold,
           text("Temp: " + std::to_string(data.morning.temperature) + "°C")});
    });

    auto afternoon = Renderer([&] {
      return vbox(
          {text("afternoon") | bold,
           text("Temp: " + std::to_string(data.afternoon.temperature) + "°C")});
    });

    auto evening = Renderer([&] {
      return vbox(
          {text("evening") | bold,
           text("Temp: " + std::to_string(data.evening.temperature) + "°C")});
    });

    auto night = Renderer([&] {
      return vbox(
          {text("night") | bold,
           text("Temp: " + std::to_string(data.night.temperature) + "°C")});
    });

    return Container::Vertical({morning, afternoon, evening, night});
  }

  Component ForecastWindow(const weather_forecast::ForecastData& data) {
    auto content = ForecastContent(data);
    return Renderer(content, [=] {
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
  /*
  weather_forecast::WeatherForecastProvider provider_(
      "https://api.open-meteo.com/v1/forecast", "");
  ││City: Moscow ││ Latitude: 55.755798 ││ Longitude: 37.617802 ││
  provider_.FetchForecastData({{55.755798, 37.617802}, 3});

  for (int i = 0; i < 3; ++i) {
    std::cout << provider_.GetForecastData(i).date << '\n';
    std::cout << "   morning: "
              << provider_.GetForecastData(i).morning.temperature << '\n';
    std::cout << "   afternoon: "
              << provider_.GetForecastData(i).afternoon.temperature << '\n';
    std::cout << "   evening: "
              << provider_.GetForecastData(i).evening.temperature << '\n';
    std::cout << "   night: "
              << provider_.GetForecastData(i).night.temperature << '\n';
  }
  */

  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <api_url> <api_key>\n";
    return 1;
  }

  const std::string api_url = argv[1];
  const std::string api_key = argv[2];

  WeatherApp app("https://api.open-meteo.com/v1/forecast", "", api_url,
                 api_key);
  app.Run();

  return 0;
}
