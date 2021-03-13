#ifndef LOADER
#define LOADER

typedef struct Image {
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned char* pixels; 
} Image;

void loader(char* path, Image* image);
void destroy(Image* image);
#endif