# weatherfxlite

## Configuration

WeatherFXLite uses the [OpenWeatherMap](https://openweathermap.org/api) REST API 2.5.  To build the application you'll need to provide an OpenWeatherMap API key and the latitude/longitude of the location.  The exact API endpoint is [current](https://openweathermap.org/current).

`config.h`:

```
#define OPENWEATHERMAP_APP_ID "<YOUR_OPENWEATHER_APP_ID>"
#define OPENWEATHERMAP_LATLNG "&lat=<YOUR_LAT>lon=<YOUR_LNG>"
```

## macOS

To build on macOS:

```
brew install qt5
```

You may need to update your `PATH`:

```
export PATH="/opt/homebrew/opt/qt@5/bin:$PATH"
```

Then:
```
qmake
make
```

To run the application from the command line:

```
% build/weatherfxLite.app/Contents/MacOS/weatherfxLite
```

## Linux