/*
  conditiontest.cpp
  Cycles through all WeatherKit condition strings to verify font auto-sizing,
  icon display, and background colors across the full condition set.
*/

#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QFont>
#include <QFontMetrics>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPair>
#include <QString>
#include <map>

#include "ui_weatherfxLite.h"
#include <icons/icons.h>

typedef struct {
  unsigned char* data;
  unsigned int   len;
} WeatherIcon;

static std::map<QString, WeatherIcon> dayIcons = {
  {"Clear",                 {day_Clear_png,                day_Clear_png_len}},
  {"MostlyClear",           {day_Clear_png,                day_Clear_png_len}},
  {"MostlyCloudy",          {day_MostlyCloudy_png,         day_MostlyCloudy_png_len}},
  {"PartlyCloudy",          {day_PartlyCloudy_png,         day_PartlyCloudy_png_len}},
  {"Rain",                  {day_Rain_png,                 day_Rain_png_len}},
  {"HeavyRain",             {day_HeavyRain_png,            day_HeavyRain_png_len}},
  {"Thunderstorms",         {day_Thunderstorms_png,        day_Thunderstorms_png_len}},
  {"IsolatedThunderstorms", {day_IsolatedThunderstorms_png,day_IsolatedThunderstorms_png_len}},
  {"ScatteredThunderstorms",{day_Thunderstorms_png,        day_Thunderstorms_png_len}},
  {"Cloudy",                {day_Cloudy_png,               day_Cloudy_png_len}},
  {"Drizzle",               {day_Drizzle_png,              day_Drizzle_png_len}},
  {"Breezy",                {day_Breezy_png,               day_Breezy_png_len}},
  {"Windy",                 {day_Windy_png,                day_Windy_png_len}},
  {"Snow",                  {day_Snow_png,                 day_Snow_png_len}},
  {"HeavySnow",             {day_Snow_png,                 day_Snow_png_len}},
  {"Flurries",              {day_Flurries_png,             day_Flurries_png_len}},
  {"Haze",                  {day_Haze_png,                 day_Haze_png_len}},
};

static const QVector<QPair<QString, QString>> conditions = {
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

static const QString backgrounds[] = {
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

class ConditionTest : public QObject {
  Q_OBJECT
public:
  ConditionTest(Ui::Form* ui, QWidget* window)
    : ui(ui), window(window), index(0), temp(110), scene(nullptr), item(nullptr) {}

public slots:
  void next() {
    if (index >= conditions.size()) {
      index = 0;
    }

    const QString& code  = conditions[index].first;
    const QString& label = conditions[index].second;

    // Condition text with auto-shrink (same logic as WeatherFXLite.cpp)
    ui->currentCondition->setText(label);
    QFont f = ui->currentCondition->font();
    f.setPointSize(60);
    QFontMetrics fm(f);
    while (fm.horizontalAdvance(label) > ui->currentCondition->width() && f.pointSize() > 20) {
      f.setPointSize(f.pointSize() - 1);
      fm = QFontMetrics(f);
    }
    ui->currentCondition->setFont(f);

    // Icon
    if (scene == nullptr)
      scene = new QGraphicsScene();

    if (item) {
      delete item;
      item = nullptr;
    }

    if (dayIcons.count(code)) {
      WeatherIcon ico = dayIcons[code];
      QPixmap px;
      if (px.loadFromData(ico.data, ico.len, "png")) {
        item = new QGraphicsPixmapItem(px);
        scene->addItem(item);
      }
    }
    ui->graphicsView->setScene(scene);

    // Precipitation icon — use the same condition icon scaled down
    if (dayIcons.count(code)) {
      WeatherIcon ico = dayIcons[code];
      QPixmap px;
      if (px.loadFromData(ico.data, ico.len, "png"))
        ui->precipIcon->setPixmap(px.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // Count down from 110 to catch 3-digit clipping, with auto-shrink (same logic as main app)
    {
      QString tempText = QString::number(temp) + "°";
      ui->currentTemperature->setText(tempText);
      QFont tf = ui->currentTemperature->font();
      tf.setPointSize(114);
      QFontMetrics tfm(tf);
      while (tfm.horizontalAdvance(tempText) > ui->currentTemperature->width() && tf.pointSize() > 60) {
        tf.setPointSize(tf.pointSize() - 1);
        tfm = QFontMetrics(tf);
      }
      ui->currentTemperature->setFont(tf);
    }
    {
      QString speedText = (temp >= 100 ? "100" : "12");
      ui->windSpeed->setText(speedText);
      QFont sf = ui->windSpeed->font();
      sf.setPointSize(36);
      QFontMetrics sfm(sf);
      while (sfm.horizontalAdvance(speedText) > ui->windSpeed->width() && sf.pointSize() > 18) {
        sf.setPointSize(sf.pointSize() - 1);
        sfm = QFontMetrics(sf);
      }
      ui->windSpeed->setFont(sf);
    }
    ui->currentTime->setText("3:45 PM");
    ui->currentDate->setText("Mar 05");

    int hi = temp + 5;
    int lo = temp - 10;
    int precip = temp >= 100 ? 100 : 35;
    ui->hiTemp->setText(QString::number(hi) + "°");
    ui->loTemp->setText(QString::number(lo) + "°");
    ui->precipChance->setText(QString::number(precip) + "%");

    int bgIndex = qBound(0, temp / 10, 9);

    window->setStyleSheet("background-color:" + backgrounds[bgIndex] + ";");

    qDebug("[%2d/%-2d] %-30s → %dpt",
           index + 1, (int)conditions.size(),
           qPrintable(label), f.pointSize());

    index++;
    temp--;
    if (temp < 0) temp = 110;
  }

private:
  Ui::Form*            ui;
  QWidget*             window;
  int                  index;
  int                  temp;
  QGraphicsScene*      scene;
  QGraphicsPixmapItem* item;
};

#include "conditiontest.moc"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QWidget window;
  Ui::Form ui;
  ui.setupUi(&window);

  window.setWindowTitle("Condition Test");
  window.setStyleSheet("background-color:" + backgrounds[9] + ";");
  ui.gpsAvailable->hide();

  // Pin the condition label height (same as main app)
  {
    QFont f = ui.currentCondition->font();
    f.setPointSize(60);
    ui.currentCondition->setFixedHeight(QFontMetrics(f).height());
  }

  // Pin the temperature label to the widest 2-digit width so 3-digit values trigger auto-shrinking
  {
    QFont f = ui.currentTemperature->font();
    f.setPointSize(114);
    ui.currentTemperature->setFixedWidth(QFontMetrics(f).horizontalAdvance("99°"));
  }

  // Pin the wind speed label to the widest 2-digit width so 3-digit values trigger auto-shrinking
  {
    QFont f = ui.windSpeed->font();
    f.setPointSize(36);
    ui.windSpeed->setFixedWidth(QFontMetrics(f).horizontalAdvance("99"));
  }

  ConditionTest tester(&ui, &window);

#ifdef Q_OS_LINUX
  window.setWindowFlags(Qt::FramelessWindowHint);
  window.setWindowState(Qt::WindowFullScreen);
  QRect screenRect = QApplication::desktop()->screenGeometry(1);
  window.move(QPoint(screenRect.x(), screenRect.y()));
#endif

  window.show();

  tester.next();

  QTimer timer;
  QObject::connect(&timer, &QTimer::timeout, &tester, &ConditionTest::next);
  timer.start(1000);

  return app.exec();
}
