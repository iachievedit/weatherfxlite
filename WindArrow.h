/*
    weatherfxlite
    A lite replacement for the venerable Bushnell WeatherFX station. RIP.
    Copyright (C) 2024 iAchieved.it LLC

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

#ifndef __WINDARROW_H__
#define __WINDARROW_H__

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>

class WindArrow : public QWidget {
public:
  WindArrow(QWidget* parent = nullptr);
  void setDirection(double direction);
  
protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap drawArrowPixmap(int width, int height);

    QPixmap arrowPixmap;
    double windDirection;
};
#endif