//
// Created by seann on 13/03/2021.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decodeencode.h"
#include "copybitmap.h"
#include "cbmp.h"

static BMP* bmp;
static unsigned char* image;
static double SCALING_FACTOR = 1.5;

void cleanup() {
    destroy(image);
    bclose(bmp);
}

unsigned long setup() {
    bmp = bopen("../images/bond.bmp");
    unsigned long image_size = (unsigned long) bmp->height * (unsigned long) bmp->width * 3;
    image = extract_rbg_only(bmp, image_size);
    return image_size;
}

Data readInput() {
    Data data;
    unsigned char* message = malloc(sizeof(char) * 20);
    int index = 4;
    char character;
    int capacity = 20;
    while (scanf("%c", &character) != EOF) {
        message[index++] = character;
        if (index == capacity) {
            char* temp = realloc(message, sizeof(char) * capacity * SCALING_FACTOR);
            if (temp == NULL) {
                perror("no more memory!!!\n");
                exit(1);
            }
        }
    }

    *((int*) message) = index;
    data.data = message;
    data.size = index;
    return data;
}

int main(int argc, char const *argv[]) {

    if (argc < 3 || argc > 4 || (argc == 2 && !strcmp("decode", argv[1])) ||
        (argc == 3 && !strcmp("encode", argv[1]))) {
        printf("   usage %s encode <in file> <out file>\n or usage %s decode <in file>\n", argv[0], argv[0]);
        return 1;
    }

    unsigned long image_size = setup();
    if (!strcmp("encode", argv[1])) {
        Data message = readInput();
        encode(message, image, image_size);
        add_bytes_back(bmp, image);
        bwrite(bmp, argv[3]);
    }
    else {
        Data result = decode(image, image_size);
        int size = *((int*) result.data);
        printf("%*s\n", size, result.data + 4);
    }

    cleanup();
    return 0;
}