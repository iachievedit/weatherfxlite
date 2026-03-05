# WeatherFXLite — Claude Code Guide

## Project Overview

Qt5 desktop weather application that replicates the Bushnell WeatherFX hardware weather station. Fetches data from the Apple WeatherKit REST API. Primary deployment target: Raspberry Pi 4 with HyperPixel 4.0 Square display. Also runs on macOS and Linux desktops.

## Tech Stack

- **Language:** C++11/14
- **UI:** Qt 5 (QMake build system)
- **APIs:** Apple WeatherKit REST API v1
- **IPC:** ZeroMQ (optional GPS coordinate reception)
- **Crypto:** OpenSSL/libcrypto (JWT signing for WeatherKit auth)
- **Bundled headers:** jwt-cpp, picojson, nlohmann/json (all in `include/`)

## Project Structure

```
weatherfxLite.pro       # QMake project file — source of truth for build config
config.h                # NOT in git — API keys, location, unit prefs (see config.h.tmpl)
config.h.tmpl           # Template for config.h
build.sh                # Cross-platform build script (macOS/Linux)
build/                  # Build output directory

WeatherFXLite.h/.cpp    # Main window/UI controller
WeatherKitAPI.h/.cpp    # Apple WeatherKit API client + JWT auth
FileDownloader.h/.cpp   # Qt HTTP client (wraps QNetworkAccessManager)
WindArrow.h/.cpp        # Custom QWidget for wind direction arrow
ZmqListener.h/.cpp      # Background thread, subscribes to tcp://localhost:11205 for GPS
OpenWeatherAPI.h/.cpp   # Legacy OpenWeather integration (inactive)

weatherfxLite.ui        # Qt Designer UI definition
icons/                  # Source SVGs/PNGs + Makefile to generate icons.h
include/icons/icons.h   # Generated: all weather icons as embedded C arrays
```

## Building

**macOS (Homebrew):**
```bash
brew install qt5 openssl libzmq
export PATH="/opt/homebrew/opt/qt@5/bin:$PATH"
./build.sh
```

**Linux / Raspberry Pi OS:**
```bash
sudo apt-get install qtbase5-dev libssl-dev clang libzmq3-dev
./build.sh
```

`build.sh` validates `config.h` exists, runs `qmake` with the correct OpenSSL paths, then `make -j`.

Output:
- macOS: `build/weatherfxLite.app/Contents/MacOS/weatherfxLite`
- Linux: `build/weatherfxLite`

## Configuration (config.h)

`config.h` is gitignored. Copy from template and fill in:
```cpp
#define LATLNG "LAT/LNG"                   // e.g. "33.045230/-96.728030"
#define APPLE_DEVELOPER_TEAM_ID "..."
#define WEATHERKIT_KEY_ID "..."
#define WEATHERKIT_APP_ID "..."
#define WEATHERKIT_PUBKEY R"(...)"          // EC P-256 public key
#define WEATHERKIT_PRIVKEY R"(...)"         // EC P-256 private key

// Optional:
// #define METRIC                           // Celsius + km/h
// #define TWENTYFOUR_HOUR_FORMAT
// #define MINIMAL_UI
// #define DEBUG_EXTREMES                   // Test temperature color bands
```

## Key Architectural Patterns

- **Timer loop:** 1-second QTimer; weather data refreshed every 60 ticks
- **Signal/Slot:** All async communication (HTTP replies, ZMQ data, timer ticks)
- **JWT auth:** Generated per-request in WeatherKitAPI using jwt-cpp + OpenSSL
- **Icons:** Embedded as C arrays in `include/icons/icons.h` (built via `icons/Makefile`)
- **Temperature coloring:** Background shifts through purple→red across 10°F bands (0–90°F+)
- **Day/night icons:** Selected based on WeatherKit sunrise/sunset data

## Raspberry Pi Deployment

```bash
./pi4_setup.sh   # Installs systemd service for auto-start on boot
```

See [docs/RPi.md](docs/RPi.md) for full setup guide.

## Important Notes

- `config.h` must never be committed — it contains private API keys
- Regenerate icons header when adding new icons: run `make` in `icons/`
- The `develop` branch is the active development branch; `main` is for stable releases
- ZeroMQ port is `11205` (tcp://localhost:11205)
- Qt's moc/uic generated files (`moc_*.cpp`, `ui_*.h`) are build artifacts — don't edit them
