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
    : ui(ui), window(window), index(0), bgIndex(5), scene(nullptr), item(nullptr) {}

public slots:
  void next() {
    if (index >= conditions.size()) {
      index = 0;
      bgIndex = (bgIndex + 1) % 10;
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

    // Other fields
    int temp = bgIndex * 10 + 5;
    ui->currentTemperature->setText(QString::number(temp) + "°");
    ui->windSpeed->setText("12");
    ui->currentTime->setText("3:45 PM");
    ui->currentDate->setText("Mar 05");
    ui->hiTemp->setText("72°");
    ui->loTemp->setText("48°");

    window->setStyleSheet("background-color:" + backgrounds[bgIndex] + ";");

    qDebug("[%2d/%-2d] %-30s → %dpt",
           index + 1, (int)conditions.size(),
           qPrintable(label), f.pointSize());

    index++;
  }

private:
  Ui::Form*            ui;
  QWidget*             window;
  int                  index;
  int                  bgIndex;
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
  window.setStyleSheet("background-color:" + backgrounds[5] + ";");
  ui.gpsAvailable->hide();

  // Pin the condition label height (same as main app)
  {
    QFont f = ui.currentCondition->font();
    f.setPointSize(60);
    ui.currentCondition->setFixedHeight(QFontMetrics(f).height());
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
