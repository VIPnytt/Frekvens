# 🔤 Fonts

Character availability varies a lot, but fortunately it's easy to add new characters.

## 👁️ Braille

World's smallest font with just 2x3 pixels.

Included by default, but can be excluded by setting `FONT_BRAILLE` to `false`.

[secrets.h](../tree/main/firmware/include/config/secrets.h) example:

```h
#define FONT_BRAILLE true // boolean
```

## 🔬 Micro

Microscopic but "readable" font. Most characters are 3x3 pixels.

## 🤏 Mini

A good middleground, most characters are about 4x5 pixels.

## ⬇️ Small

Included by default, but can be excluded by setting `FONT_SMALL` to `false`.

[secrets.h](../tree/main/firmware/include/config/secrets.h) example:

```h
#define FONT_SMALL true // boolean
```

## ➡️ Medium

Used by some clocks, currently only digits are supported.

See also [Medium bold](#🅱%EF%B8%8F-medium-bold).

## 🅱️ Medium bold

Similar to the "regular" medium font, digits only.

See also [Medium](#%EF%B8%8F-medium).

## ⬆️ Large

Stylish, but so large that it almost isn't practical.
