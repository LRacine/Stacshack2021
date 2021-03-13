#ifndef COPY_BITMAP
#define COPY_BITMAP

#include "cbmp.h"

unsigned char* extract_rbg_only(BMP* bmp, unsigned long size);
void add_bytes_back(BMP* bmp, unsigned char* pixels);
void destroy(unsigned char* pixels);

#endif