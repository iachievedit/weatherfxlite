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

#ifndef __WEATHERFXLITE_H__
#define __WEATHERFXLITE_H__

#include <QWidget>
#include <QTimer>
#include <OpenWeatherAPI.h>

#ifdef Q_OS_LINUX
  #include <QDesktopWidget>
#endif

#include "ui_weatherfxLite.h"

class WeatherFXLite : public QObject {
Q_OBJECT
  public:
  WeatherFXLite();

  private:
  QWidget* window;
  QTimer* timer;
  Ui::Form ui;
  OpenWeatherAPI* openWeather;

  private:
  std::string backgroundForTemperature(short temp);

  private:
  bool boot {true};
  int currentConditionTicks { 0 };

  private slots:
  void updateWeatherDisplay();
  void timerTick();

};

#endif
