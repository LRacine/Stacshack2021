//
// Created by seann on 13/03/2021.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decodeencode.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "aes.h"

static int SCALING_FACTOR = 2;

double pow(double x, double exp)
{
    double temp;
    int y = (int)exp;
    if(y == 0)
        return 1;
    temp = pow(x, y/2);
    if (y % 2 == 0)
        return temp*temp;
    else
    {
        if(y > 0)
            return x*temp*temp;
        else
            return (temp*temp)/x;
    }
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
            capacity *= SCALING_FACTOR;
            message = realloc(message, sizeof(unsigned char) * capacity);
            if (message == NULL) {
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

unsigned char* encrypt(unsigned char* message, unsigned char* key) {
    int key_size;
    int* key_schedule;
    unsigned char* output;
    aes_key_setup(key, key_schedule, &key_size);
    aes_encrypt(message, output, key_schedule, key_size);
    return message;
}

int main(int argc, char const *argv[]) {

    if (argc < 4 || argc > 5 || (argc == 3 && !strcmp("decode", argv[1])) ||
        (argc == 4 && !strcmp("encode", argv[1]))) {
        printf("   usage %s encode <in file> <out file> <key>\nor usage %s decode <in file>\n", argv[0], argv[0]);
        return 1;
    }
    int width, height, channels;
    unsigned char* image = stbi_load(argv[2], &width, &height, &channels, 0);
    unsigned long image_size = (unsigned long)width * height * channels;
    if (!strcmp("encode", argv[1])) {
        Data message = readInput();
        encode(message, image, image_size);
        switch (argv[3][strlen(argv[3]) - 1])
        {
            case 'p':
                stbi_write_bmp(argv[3], width, height, channels, image);
                break;
            case 'g':
                stbi_write_png(argv[3], width, height, channels, image, width * channels);
                break;
        }
    }
    else {
        Data result = decode(image, image_size);
        int size = *((int*) result.data);
        printf("%.*s\n", size, result.data + sizeof(int));
    }

    stbi_image_free(image);
    return 0;
}