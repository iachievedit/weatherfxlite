# Icons

Icons are from Erik Flowers' awesome [Weather Icons](https://erikflowers.github.io/weather-icons/).

Rather than have the icons installed as "asset files" they are compiled directly into the binary.

The first step to do so is convert the delivered SVG files into PNGs with ImageMagick `convert`:

```
convert -background none -size 256x256 filename.svg filename.png
```

Once all the files have been converted into PNGs, use the supplied Makefile to create `icons.h`.  The Makefile uses [`xxd`](https://linux.die.net/man/1/xxd) to take the PNG and generate C code.

Here's an example of the icon used for clear conditions during the day:

```
unsigned char day_Clear_png[] = {
0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
...
};
unsigned int day_Clear_png_len = 4354;
```



