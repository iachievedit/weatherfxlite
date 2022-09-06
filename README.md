# weatherfxlite

## What is it?

The Bushnell WeatherFX is, in my opinion, one of the best "weather gadgets" on the market.  Well, _was_ on the market.  Several years ago Bushnell [announced](https://www.bushnell.com/bu-weatherfx-replacement.html) the end-of-life for the venerable device, but many, including mine, continued to work well into 2022.  Unfortunately it appears the last forecast areas are beginning to go dark.

<div align="center">
<img src="docs/byeWeatherFX.png">
</div>

It started one day where it was perpetually 80 degrees with rain and a thunderstorm (and humid!).  Then the zero degree days started, and now the high is forecasted to be `--`.  Those days are the worst. Okay, maybe not the _worst_.

<div align="center">
<img src="docs/theWorst.png">
</div>

Either way, I was mildly griefstricken when it dawned on me that the end had come to my faithful weather forecast companion.  So, being the engineer and tinkerer that I am I set about to "replace" the WeatherFX with a Raspberry Pi and HyperPixel display.  WeatherFXLite is the result for the time being.

For what it's worth, I'm not the only one traumatized by the demise of the WeatherFX.  On jillcataldo.com there are over 400 comments and [counting](https://jillcataldo.com/happy-new-year-is-your-weather-center-discontinuing-itself/) of those who are mourning their beloved WeatherFX.

## Hardware

If you're only interested in running the application on your desktop there's no additional hardware required.  WeatherFXLite is _not_ designed to work with your own sensors and requires no special hardware other than something with an Internet connection.

Now, if you're interested in a makeshift replica of the original WeatherFX, you can invest in:

* Raspberry Pi 4
* HyperPixel 4.0 display

The end result of your efforts will be something much more appealing!

<div align="center">
<img src="docs/weatherfxlite.jpeg">
</div>

NB:  This is a work in progress, hopefully by the time folks in the wild see this I'll have added the current conditions icon and temperature forecasts for the day.

## Configure and Build
### Configuration

WeatherFXLite uses the [OpenWeatherMap](https://openweathermap.org/api) REST API 2.5.  To build the application you'll need to provide an OpenWeatherMap API key and the latitude/longitude of the location.  The exact API endpoint is [current](https://openweathermap.org/current).

`config.h`:

```
#define OPENWEATHERMAP_APP_ID "<YOUR_OPENWEATHER_APP_ID>"
#define OPENWEATHERMAP_LATLNG "&lat=<YOUR_LAT>lon=<YOUR_LNG>"
```

### macOS

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

### Linux

To build on Linux, and preferably the Raspberry Pi with an appropriate external display:

```
sudo apt-get install qtbase5-dev
```

Then:

```
qmake
make
```
