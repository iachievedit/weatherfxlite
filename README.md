# WeatherFXLite

## What is it?

The Bushnell WeatherFX is, in my opinion, one of the best "weather gadgets" on the market.  Well, _was_ on the market.  Several years ago Bushnell [announced](https://www.bushnell.com/bu-weatherfx-replacement.html) the end-of-life for the venerable device, but many, including mine, continued to work well into 2022.  Unfortunately it appears the last forecast areas are beginning to go dark.

<div align="center">
<img src="docs/byeWeatherFX.png">
</div>

It started one day where it was perpetually 80 degrees with rain and a thunderstorm (and humid!).  Then the zero degree days started, and now the high is forecasted to be `--`.  Those days are the worst. Okay, maybe not the _worst_.

<div align="center">
<img src="docs/theWorst.png">
<img src="docs/theWorstHeat.png">
</div>

Either way, I was mildly griefstricken when it dawned on me that the end had come to my faithful weather forecast companion.  So, being the engineer and tinkerer that I am I set about to "replace" the WeatherFX with a Raspberry Pi and HyperPixel display.  WeatherFXLite is the result for the time being.

For what it's worth, I'm not the only one traumatized by the demise of the WeatherFX.  On jillcataldo.com there are over 400 comments and [counting](https://jillcataldo.com/happy-new-year-is-your-weather-center-discontinuing-itself/) of those who are mourning their beloved WeatherFX.

**2022-10-08 Update**

Around September 7, 2022, individuals across the country began reporting their WeatherFX units had come back to life.  Indeed, mine did as well.  Occasionally the clock will be behind an hour and the current temperature will be wildly incorrect, so I think we'll keep working on this project!

**2024-10-12 Update**
My Bushnell WeatherFX continues to update, though it occasionally goes "offline" for a day or two.

## Hardware

If you're only interested in running the application on your desktop there's no additional hardware required.  WeatherFXLite is _not_ designed to work with your own sensors and requires no special hardware other than something with an Internet connection.

Now, if you're interested in a makeshift replica of the original WeatherFX, you can invest in:

* Raspberry Pi 4
* [HyperPixel 4.0 Square display](https://shop.pimoroni.com/products/hyperpixel-4-square?variant=30138251444307)

The end result of your efforts will be something much more appealing!

<div align="center">
<img src="docs/weatherfxlite.jpeg">
</div>

NB:  This is a work in progress, hopefully by the time folks in the wild see this I'll have added the current conditions icon and temperature forecasts for the day.

**2022-09-25 Update**

Additional icons and tightening up the display!

<div align="center">
<img src="docs/weatherfxlite2.jpg">
</div>

## Configure and Build
### Configuration

WeatherFXLite uses the REST API for Apple's [WeatherKit](https://developer.apple.com/weatherkit/).  To use WeatherKit and build this project you'll need an Apple Developer Account.  See our [tutorial](https://dev.iachieved.it/iachievedit/weatherkit-rest-api/) for details on obtaining a private and public key.  For what it's worth,  I started developing WeatherFXLite with the free version of [OpenWeatherMap](https://openweathermap.org/api) REST API 2.5 but the current conditions for my area (North Texas) were frequently incorrect.  Rather than paying for yet another service (like AccuWeather), I went with utilizing the WeatherKit credits (500,000 API calls per month) in my Apple Developer account.

Once you have your key and Apple Developer account information, create `config.h`:

```
// WeatherKit
// LATLNG is LAT/LNG for your location
// #define METRIC
#define LATLNG "32.7767/-96.7970" // Dallas, TX

// Your Apple Developer Information
#define APPLE_DEVELOPER_TEAM_ID "5367BG94QP"
#define WEATHERKIT_KEY_ID "S3J684C78A"
#define WEATHERKIT_APP_ID "5367BG94QP.it.iachieved.weatherfx"
#define WEATHERKIT_APP "it.iachieved.weatherfx"
```

**NOTE**:  Your Apple Developer team ID, key ID, etc. will be unique for you.  The values should not be copy/pasted blindly!

Now, for the WeatherKit keys, in `config.h` define `WEATHERKIT_PUBKEY` and `WEATHERKIT_PRIVKEY` as C++ raw strings:

```
#define WEATHERKIT_PUBKEY R"(-----BEGIN PUBLIC KEY-----
-----END PUBLIC KEY-----)"

#define WEATHERKIT_PRIVKEY R"(-----BEGIN PRIVATE KEY-----
-----END PRIVATE KEY-----)"
```

> [!NOTE]
> Add `#define METRIC` to `config.h` if you prefer Celsius and kilometers.

# Building

* [macOS](macOS.md)
* [Raspberry Pi](RPi.md)

# Installing

* [Raspberry Pi](RPi.md)

# Legal

This code is licensed under [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html).  Why?  Because we are using the open source license of [Qt](https://www.qt.io/licensing/).

Additional dependencies and their licenses:

* [jwt-cpp](https://github.com/Thalhammer/jwt-cpp) - [MIT License](https://choosealicense.com/licenses/mit/)
* [picojson](https://github.com/kazuho/picojson) - [2-Clause BSD](https://choosealicense.com/licenses/bsd-2-clause/)
* [json.hpp](https://github.com/nlohmann/json) - [MIT License](https://choosealicense.com/licenses/mit/)
