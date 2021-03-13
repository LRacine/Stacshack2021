#include "load.h"
#include "LoadBMP/loadbmp.h"
#include <stdlib.h>
#include <stdio.h>

void loader(char* path, Image* image) {
    unsigned int err = loadbmp_decode_file(path, &(image->pixels), &(image->width),
                                            &(image->height), LOADBMP_RGBA);
    if (err) {
        printf("Could not load file due to %d\n", err);
    }
    image->size = image->width * image->height;
}

void destroy(Image* image) {
    free(image->pixels);
    free(image);
}

int main(int argc, char const *argv[])
{
    Image* image = malloc(sizeof(Image));
        if (image == NULL) {
            printf("Ran out of mem");
        return 1;
    }
    loader("Samples/1.bmp", image);
    printf("size of array is %d\n", image->size);
    destroy(image);
    /* code */
    return 0;
}


