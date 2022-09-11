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

#ifndef __OPENWEATHERAPI_H__
#define __OPENWEATHERAPI_H__

#include <QObject>

#include "FileDownloader.h"

typedef struct CurrentConditions {
  QString condition;
  int temperature;
  int low = INT_MAX;
  int high = INT_MIN;
} CurrentConditions;

class OpenWeatherAPI : public QObject {
Q_OBJECT
  public:
  OpenWeatherAPI();
  ~OpenWeatherAPI();

  public:
  void updateCurrentConditions(void);
  void updateCurrentForecast(void);
  CurrentConditions getCurrentConditions(void);
  CurrentConditions getCurrentForecast(void);

  signals:
  void currentConditionsUpdate(void);
  void currentForecastUpdate(void);

  private:
  FileDownloader* fDownloader;
  FileDownloader* fcastDownloader;
  CurrentConditions currentConditions;

  public slots:
  void parseCurrentConditions(void);
  void parseCurrentForecast(void);

};

#endif