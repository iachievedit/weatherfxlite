/*
    weatherfxlite
    A lite replacement for the venerable Bushnell WeatherFX station. RIP.
    Copyright (C) 2022 iAchieved.it LLC

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "WeatherKitAPI.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <jwt-cpp/jwt.h>

#include <icons/icons.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
using std::cout;
using std::endl;

#define WEATHERKIT_API_URL "https://weatherkit.apple.com/api/v1/weather/"

#include "config.h"

static std::map<QString, QString> conditionCodeMap = {
  {"Clear",                  "Clear"},
  {"Cloudy",                 "Cloudy"},
  {"Dust",                   "Dust"},
  {"Fog",                    "Fog"},
  {"Haze",                   "Haze"},
  {"MostlyClear",            "Mostly Clear"},
  {"MostlyCloudy",           "Mostly Cloudy"},
  {"PartlyCloudy",           "Partly Cloudy"},
  {"ScatteredThunderstorms", "Scattered Thunderstorms"},
  {"Smoke",                  "Smoke"},
  {"Breezy",                 "Breezy"},
  {"Windy",                  "Windy"},
  {"Drizzle",                "Drizzle"},
  {"HeavyRain",              "Heavy Rain"},
  {"Rain",                   "Rain"},
  {"Showers",                "Showers"},
  {"Flurries",               "Flurries"},
  {"HeavySnow",              "Heavy Snow"},
  {"MixedRainAndSleet",      "Mixed Rain and Sleet"},
  {"MixedRainAndSnow",       "Mixed Rain and Snow"},
  {"MixedRainfall",          "Mixed Rainfall"},
  {"MixedSnowAndSleet",      "Mixed Snow and Sleet"},
  {"ScatteredShowers",       "Scattered Showers"},
  {"ScatteredSnowShowers",   "Scattered Snow Showers"},
  {"Sleet",                  "Sleet"},
  {"Snow",                   "Snow"},
  {"SnowShowers",            "Snow Showers"},
  {"Blizzard",               "Blizzard"},
  {"BlowingSnow",            "Blowing Snow"},
  {"FreezingDrizzle",        "Freezing Drizzle"},
  {"FreezingRain",           "Freezing Rain"},
  {"Frigid",                 "Frigid"},
  {"Hail",                   "Hail"},
  {"Hot",                    "Hot"},
  {"Hurricane",              "Hurricane"},
  {"IsolatedThunderstorms",  "Isolated Thunderstorms"},
  {"SevereThunderstorm",     "Severe Thunderstorm"},
  {"Thunderstorms",          "Thunderstorms"},
  {"Tornado",                "Tornado"},
  {"TropicalStorm",          "Tropical Storm"},
};

static std::map<QString, WeatherIcon> nightIcons = {
  {"Clear",       {night_Clear_png,       night_Clear_png_len}},
  {"MostlyClear", {night_MostlyClear_png, night_MostlyClear_png_len}},
  {"Snow",        {night_Snow_png,        night_Snow_png_len}},
  {"Cloudy",      {night_Cloudy_png,      night_Cloudy_png_len}},
  {"MostlyCloudy",{night_MostlyCloudy_png, night_MostlyCloudy_png_len}},
  {"PartlyCloudy", {night_PartlyCloudy_png, night_PartlyCloudy_png_len}},
  {"Rain",         {night_Rain_png, night_Rain_png_len}},
  {"Thunderstorms",         {night_Thunderstorms_png, night_Thunderstorms_png_len}},
  {"Breezy",        {night_Breezy_png, night_Breezy_png_len}},
  {"Drizzle",       {night_Drizzle_png, night_Drizzle_png_len}},
  {"Windy",         {night_Windy_png, night_Windy_png_len}}
};

static std::map<QString, WeatherIcon> dayIcons = {
  {"Clear",         {day_Clear_png, day_Clear_png_len}},
  {"MostlyClear",   {day_Clear_png, day_Clear_png_len}},
  {"MostlyCloudy",  {day_MostlyCloudy_png, day_MostlyCloudy_png_len}},
  {"PartlyCloudy",  {day_PartlyCloudy_png, day_PartlyCloudy_png_len}},
  {"Rain",          {day_Rain_png, day_Rain_png_len}},
  {"HeavyRain",          {day_HeavyRain_png, day_HeavyRain_png_len}},
  {"Thunderstorms", {day_Thunderstorms_png, day_Thunderstorms_png_len}},
  {"Cloudy",        {day_Cloudy_png, day_Cloudy_png_len}},
  {"Drizzle",       {day_Drizzle_png, day_Drizzle_png_len}},
  {"Breezy",        {day_Breezy_png, day_Breezy_png_len}},
  {"Windy",         {day_Windy_png, day_Windy_png_len}},
  {"Snow",          {day_Snow_png, day_Snow_png_len}},
  {"HeavySnow",     {day_Snow_png, day_Snow_png_len}},
  {"Flurries",      {day_Flurries_png, day_Flurries_png_len}}
};

// Constructor
WeatherKitAPI::WeatherKitAPI() {
}

// Destructor
WeatherKitAPI::~WeatherKitAPI() {
}

void WeatherKitAPI::updateCurrentConditions(void) {

  QString url = WEATHERKIT_API_URL;
  url += "en_US/";
  url += LATLNG;
  url += "?dataSets=currentWeather";

  QString bearer(makeJWT().c_str());

  QUrl current(url);

  RequestHeaders headers;
  headers.append(qMakePair(QByteArray("Authorization"), QByteArray("Bearer " + bearer.toUtf8())));

  fDownloader = new FileDownloader(current, this, &headers);

  connect(fDownloader, SIGNAL(downloaded()), this, SLOT(parseCurrentConditions()));
}

void WeatherKitAPI::updateCurrentForecast(void) {

  qDebug() << "Update current forecast";
  
  QString url = WEATHERKIT_API_URL;
  url += "en_US/";
  url += LATLNG;
  url += "?dataSets=forecastDaily";

  QString bearer(makeJWT().c_str());

  QUrl current(url);

  RequestHeaders headers;
  headers.append(qMakePair(QByteArray("Authorization"), QByteArray("Bearer " + bearer.toUtf8())));

  fcastDownloader = new FileDownloader(current, this, &headers);

  connect(fcastDownloader, SIGNAL(downloaded()), this, SLOT(parseCurrentForecast()));

}

void WeatherKitAPI::parseCurrentConditions(void) {
  QString current(fDownloader->downloadedData());

  qDebug() << "Parse current conditions";
  qDebug().noquote() << current;

  QJsonDocument doc = QJsonDocument::fromJson(current.toUtf8());
  QJsonObject json = doc.object();

  if (json.contains("currentWeather")) {
    QJsonObject w = json["currentWeather"].toObject();
    QJsonValue t = w["temperature"];

    currentConditions.temperature = floor((t.toDouble() * 9.0) / 5.0 + 32); // Celsius to Fahrenheit

    QJsonValue c = w["conditionCode"];

    currentConditions.condition = conditionCodeMap[c.toString()];

    QJsonValue d = w["daylight"];

    if (d.toBool()) {
      currentConditions.icon = dayIcons[c.toString()];
    } else {
      currentConditions.icon = nightIcons[c.toString()];
    }

  }

  fDownloader->deleteLater();

  emit currentConditionsUpdate();

}

void WeatherKitAPI::parseCurrentForecast(void) {

  QDateTime now = QDateTime().currentDateTime();

  QString forecast(fcastDownloader->downloadedData());

  qDebug() << "Parse current forecast"; 
  qDebug().noquote() << forecast;

  QJsonDocument doc = QJsonDocument::fromJson(forecast.toUtf8());
  QJsonObject json = doc.object();

  if (json.contains("forecastDaily")) {

    QJsonObject forecastDaily = json["forecastDaily"].toObject();

    if (forecastDaily.contains("days")) {

      QJsonArray forecasts = forecastDaily["days"].toArray(); // Daily forecasts

      for (auto i = forecasts.begin(); i != forecasts.end(); i++) {

        //qDebug() << (*i);

        QJsonObject f = QJsonValue(*i).toObject();

        if (f.contains("forecastStart")) {

          QJsonValue dt = f["forecastStart"];
          QDateTime qdt = QDateTime::fromString(dt.toString(), "yyyy-MM-ddTHH:mm:ssZ");

          // Is the forecast for today?
          if (qdt.date() == now.date()) {

            double hi = floor((f["temperatureMax"].toDouble()  * 9.0) / 5.0 + 32);
            double lo = floor((f["temperatureMin"].toDouble() * 9.0) / 5.0 + 32);

            currentConditions.low = lo;
            currentConditions.high = hi;

          } 
        }
      }
    }

    fcastDownloader->deleteLater();
    emit currentForecastUpdate();
  }
}


CurrentConditions WeatherKitAPI::getCurrentConditions(void) {
  return currentConditions;
}

CurrentConditions WeatherKitAPI::getCurrentForecast(void) {
  return currentConditions;
}

std::string WeatherKitAPI::makeJWT(void) {

  // Compile key directly into binary
  const char* priv_key = WEATHERKIT_PRIVKEY;
  const char* pub_key = WEATHERKIT_PUBKEY;

  // Load key from file
#if 0
  std::ifstream t("weatherkit/priv.key");
  std::stringstream buffer;
  buffer << t.rdbuf();

  std::string priv_key = buffer.str();

  t.open("weatherkit/pub.key");
  buffer << t.rdbuf();
  std::string pub_key = buffer.str();
#endif

  auto token = jwt::create()
                   .set_issuer(APPLE_DEVELOPER_TEAM_ID)
                   .set_type("JWT")
                   .set_key_id(WEATHERKIT_KEY_ID)
                   .set_header_claim("id", jwt::claim(std::string(WEATHERKIT_APP_ID)))
                   .set_subject(WEATHERKIT_APP)
                   .set_issued_at(std::chrono::system_clock::now())
                   .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{120})
                   .sign(jwt::algorithm::es256(pub_key, priv_key));

  return token;
}
