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

#include "WeatherFXLite.h"

#define CURRENT_CONDITION_TICKS 60
#define CURRENT_FORECAST_TICKS 60

WeatherFXLite::WeatherFXLite() {

  window = new QWidget();
  ui.setupUi(window);  

  window->setStyleSheet("background-color:black;");

  #ifdef Q_OS_LINUX

  window->setWindowFlags(Qt::FramelessWindowHint);
  window->setWindowState(Qt::WindowFullScreen);
  QRect screenRect = QApplication::desktop()->screenGeometry(1);
  window->move(QPoint(screenRect.x(), screenRect.y()));

  #endif


  window->show();

  // Use WeatherKit
  weatherAPI = new WeatherKitAPI();  
  
  timer = new QTimer(this);

  connect(weatherAPI, SIGNAL(currentConditionsUpdate()), this, SLOT(updateWeatherDisplay()));
  connect(weatherAPI, SIGNAL(currentForecastUpdate()), this, SLOT(updateForecastDisplay()));
  connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));

  timer->start(1000);

}

void WeatherFXLite::updateWeatherDisplay(void) {
  CurrentConditions current = weatherAPI->getCurrentConditions();
  ui.currentCondition->setText(current.condition);
  ui.currentTemperature->setText(QString::number(current.temperature) + QString("°"));
  std::string background = "background-color:" + backgroundForTemperature(current.temperature) + ";";
  window->setStyleSheet(background.c_str());

  if (scene == NULL) {
    scene = new QGraphicsScene();
  }

  if (item) {
    delete item;
  }

  QPixmap icon;
  if (icon.loadFromData(current.icon.data, current.icon.len, "png")) {
    item = new QGraphicsPixmapItem(icon);
    scene->addItem(item);
    ui.graphicsView->setScene(scene);
  } else {
    qDebug() << "Unable to load icon data";
  }

}

void WeatherFXLite::updateForecastDisplay(void) {
  CurrentConditions current = weatherAPI->getCurrentConditions();
  ui.hiTemp->setText(QString::number(current.high) + QString("°"));
  ui.loTemp->setText(QString::number(current.low) +  QString("°"));
}

void WeatherFXLite::timerTick(void) {

  currentConditionTicks++; currentForecastTicks++;

  QDateTime now = QDateTime().currentDateTime();
  QString timeNow = now.toString("hh:mm A");
  QString dateNow = now.toString("MMMM dd, yyyy");
  ui.currentTime->setText(timeNow);
  ui.currentDate->setText(dateNow);

  if (currentForecastTicks % CURRENT_FORECAST_TICKS == 0 || boot) {
    weatherAPI->updateCurrentForecast();
    currentForecastTicks = 0;
  }

  if (currentConditionTicks % CURRENT_CONDITION_TICKS == 0 || boot) {
    weatherAPI->updateCurrentConditions();
    currentConditionTicks = 0;
    boot = false;
  }

}

const std::string backgrounds[] = {
  "#ABA5C2", // 0s
  "#ABA5C2", // 10s
  "#ABA5C2", // 20s
  "#0089C6", // 30s
  "#262A62", // 40s
  "#35713D", // 50s
  "#74AB46", // 60s
  "#F3D13C", // 70s
  "#D37733", // 80s
  "#BB352B"  // 90+
};

/// @brief Look up background color for temperature
/// @param temp 
/// @return 
std::string WeatherFXLite::backgroundForTemperature(short temp) {
  int tIndex = temp/10;
  if (tIndex <= 0) tIndex = 0;
  if (tIndex >= 9) tIndex = 9;
  return backgrounds[tIndex];
}
