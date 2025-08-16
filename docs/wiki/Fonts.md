# 🔤 Fonts

Character availability varies a lot, but fortunately it's easy to add new characters.

## 👁️ Braille

Ultra-compact 2×3 pixel font, ideal for minimal space constraints.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define FONT_BRAILLE true // boolean
```

> Enabled by default.

## 🔬 Micro

Extremely small 3×3 pixel font, offering basic legibility in tight layouts.

## 🤏 Mini

Balanced 4×5 pixel font providing improved readability without taking up much space.

## ⬇️ Small

Small yet clear font suitable for most general-purpose display needs.

> Enabled by default.

## ➡️ Medium

Clean and versatile font with a slightly larger footprint, great for numeric and text content.

See also [Medium bold](#🅱%EF%B8%8F-medium-bold).

## 🅱️ Medium bold

Bold and legible style based on the medium font, suited for headings and emphasis.

See also [Medium](#%EF%B8%8F-medium).

## ⬆️ Large

High-impact display font with oversized characters for standout visibility.
