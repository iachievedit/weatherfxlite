# Building on Raspberry Pi

These instructions are for Raspberry Pi OS based upon Debian
Bookworm.

## Imaging

The [Raspberry Pi Imager](https://www.raspberrypi.com/software/) software
is the easiest way to get started.  

<div align="center">
<img src="rpi_imager.png">
</div>

I recommend using the **OS Customization** option to configure your
hostname, Wi-Fi, timezone, and SSH keys.


## Obtaining

SSH into your newly imaged Pi, and

```bash
git clone https://github.com/iachievedit/weatherfxlite
```

## Building

You can run `build.sh` to build everything on the Raspberry Pi.

```bash
cd weatherfxlite
./build.sh
```

## Installing

To turn your Pi 4 into a WeatherFXLite display, run `pi4_setup.sh`.

This script runs a number of commands to:

* Initialize the Hyperpixel 4.0 display
* Prevent the display from going to sleep
* Hide the mouse cursor under Wayland

## Tweaks

The latest Raspberry Pi OS based upon Bookworm requires little in the way
of configuring the Hyperpixel 4.0 display.

Edit `/boot/firmware/config.txt` and add the following line:

```
dtoverlay=vc4-kms-dpi-hyperpixel4sq
```

Reboot your Pi.  If necessary, you can rotate the display by
changing the above to:

```
dtoverlay=vc4-kms-dpi-hyperpixel4sq,rotate=180
```