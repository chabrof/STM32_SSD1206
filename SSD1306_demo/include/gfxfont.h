// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.

#ifndef _GFXFONT_H_
#define _GFXFONT_H_

typedef struct { // Data stored PER GLYPH
	unsigned int bitmapOffset;     // Pointer into GFXfont->bitmap
	unsigned int  width, height;    // Bitmap dimensions in pixels
	unsigned int  xAdvance;         // Distance to advance cursor (x axis)
	int   xOffset, yOffset; // Dist from cursor pos to UL corner
} GFXglyph;

typedef struct { // Data stored for FONT AS A WHOLE:
	unsigned int  *bitmap;      // Glyph bitmaps, concatenated
	GFXglyph *glyph;       // Glyph array
	unsigned int   first, last; // ASCII extents
	unsigned int   yAdvance;    // Newline distance (y axis)
} GFXfont;

#endif // _GFXFONT_H_
