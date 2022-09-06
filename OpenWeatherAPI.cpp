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

#include "OpenWeatherAPI.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <iostream>
#include <cmath>
using std::cout;
using std::endl;

#define OPENWEATHERMAP_CURRENT_COND "https://api.openweathermap.org/data/2.5/weather"

#include "config.h"

OpenWeatherAPI::OpenWeatherAPI() {
}

OpenWeatherAPI::~OpenWeatherAPI() {
}

void OpenWeatherAPI::updateCurrentConditions(void) {

  QString url = OPENWEATHERMAP_CURRENT_COND;
  url += "?appId=";
  url += OPENWEATHERMAP_APPID;
  url += OPENWEATHERMAP_LATLNG;
  url += "&units=imperial";

  QUrl current(url);

  fDownloader = new FileDownloader(current, this);

  connect(fDownloader, SIGNAL(downloaded()), this, SLOT(parseCurrentConditions()));

}

void OpenWeatherAPI::parseCurrentConditions(void) {
  QString current(fDownloader->downloadedData());

  //cout << current.toStdString() << endl;

  QJsonDocument doc = QJsonDocument::fromJson(current.toUtf8());
  QJsonObject  json = doc.object();

  // "main":{"temp":90.61,"feels_like":97.25,"temp_min":87.03,"temp_max":93.06,"pressure":1014,"humidity":53}
  if (json.contains("main")) {
    QJsonObject m = json["main"].toObject();
    QJsonValue  t = m["temp"];
    currentConditions.temperature = floor(t.toDouble());
  }

  // "weather":[{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}]
  if (json.contains("weather")) {
    QJsonArray w = json["weather"].toArray();
    QJsonValue a = w[0];
    QJsonValue c = a["main"];
    
    currentConditions.condition = c.toString();
  } 
  
  emit currentConditionsUpdate();

}

void OpenWeatherAPI::updateCurrentForecast(void) {

}

void OpenWeatherAPI::parseCurrentForecast(void) {

}

CurrentConditions OpenWeatherAPI::getCurrentConditions(void) {
  return currentConditions;
}
