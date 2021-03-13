#include "cbmp.h"
#include <stdlib.h>


unsigned char* extract_rbg_only(BMP* bmp, unsigned long size) {
    unsigned char* pixels = malloc(sizeof(unsigned char) * size);

    unsigned int x, y;
    unsigned char r, g, b;
    unsigned long index = 0;
    for (x = 0; x < bmp->width; x++) {
        for (y = 0; y < bmp->height; y++) {
            // Gets pixel rgb values at point (x, y)
            get_pixel_rgb(bmp, x, y, &r, &g, &b);
            pixels[index++] = r;
            pixels[index++] = g;
            pixels[index++] = b;
        }
    }

    return pixels;
}

void destroy(unsigned char* pixels) {
    free(pixels);
}

void add_bytes_back(BMP* bmp, unsigned char* pixels) {
    unsigned int x, y;
    unsigned char r, g, b;
    unsigned long index = 0;
    for (x = 0; x < bmp->width; x++) {
        for (y = 0; y < bmp->height; y++) {
            r = pixels[index++];
            g = pixels[index++];
            b = pixels[index++];
            set_pixel_rgb(bmp, x, y, r, g, b);
        }
    }
}
