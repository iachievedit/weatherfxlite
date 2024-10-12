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

#include <QPainter>
#include <QDebug>
#include "WindArrow.h"

#define ARROW_WIDTH  72
#define ARROW_HEIGHT 72

WindArrow::WindArrow(QWidget* parent)
  : QWidget(parent) {
    arrowPixmap = drawArrowPixmap(ARROW_WIDTH, ARROW_HEIGHT);

    qDebug() << "ArrowWidget created with wind direction: " << windDirection;
}

void WindArrow::setDirection(double direction) {
  windDirection = direction;
  arrowPixmap = drawArrowPixmap(ARROW_WIDTH, ARROW_HEIGHT);  
  update();  // Trigger a repaint of the widget
  qDebug() << "ArrowWidget set wind direction: " << windDirection;
  }

void WindArrow::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Calculate the center point of the widget
  QPoint center(width() / 2, height() / 2);

  // Apply rotation transformation based on wind direction
  painter.translate(center);          // Move origin to center of the widget
  painter.rotate(windDirection + 180);     // Rotate around the center
  painter.translate(-center);         // Move origin back to top-left corner

  // Draw the rotated arrow pixmap at the center of the widget
  painter.drawPixmap(center.x() - arrowPixmap.width() / 2, 
                      center.y() - arrowPixmap.height() / 2, 
                      arrowPixmap);
}

QPixmap WindArrow::drawArrowPixmap(int width, int height) {
  QPixmap pixmap(width, height);
  pixmap.fill(Qt::transparent);  // Fill with transparency

  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setBrush(Qt::black);    
  painter.setPen(Qt::NoPen);    

  painter.setBrush(Qt::NoBrush);                    // No fill for the shape
  painter.setPen(QPen(Qt::black, 3));               // Black pen with medium thickness (3px)


QPolygon arrow;
arrow << QPoint(width / 2, 0)               // Top center (arrow tip)
      << QPoint(width / 4, height)          // Left base
      << QPoint(width / 2, height * 0.9)    // Middle of base (dent up a bit)
      << QPoint(3 * width / 4, height);     // Right base




  painter.drawPolygon(arrow);
  return pixmap;
}
