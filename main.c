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
#include "rsa.h"

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

Data readInput(FILE* fp) {
    Data data;
    unsigned char* message = malloc(sizeof(char) * 20);
    int index = 4;
    char character;
    int capacity = 20;
    while (fscanf(fp, "%c", &character) != EOF) {
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

void store_key(key_class input, const char* keypath) {
    FILE* fp = fopen(keypath, "w");
    fprintf(fp, "%lld %lld", input.modulus, input.exponent);
    fclose(fp);
}

key_class get_key(const char* keypath) {
    key_class result;
    FILE* fp = fopen(keypath, "r");
    fscanf(fp, "%lld", &result.modulus);
    fscanf(fp, " ");
    fscanf(fp, "%lld", &result.exponent);
    fclose(fp);
    return result;
}

void create_keys() {
    key_class public;
    key_class private;
    rsa_gen_keys(&public, &private, "primes.txt");
    store_key(public, "key.pub");
    store_key(private, "key");
}

void encrypt(Data* message, const char* keypath) {
    int new_size = 4 + ((int)message->size - 4) * 8;
    unsigned char* new_message = malloc(new_size);
    key_class key = get_key(keypath);
    unsigned char* encrypted_message = (unsigned char *) rsa_encrypt((const char *) (message->data + 4), message->size - 4, &key);
    memcpy(new_message + 4, encrypted_message, new_size - 4);
    message->size = new_size;
    *((int*) new_message) = (int)new_size - 4;
    free(message->data);
    message->data = new_message;
    free(encrypted_message);
}

void decrypt(Data* message, const char* keypath) {
    int new_size = 4 + (*((int*) message->data)) / 8;
    unsigned char* new_message = malloc(new_size);
    key_class key = get_key(keypath);
    unsigned char* decrypt_message = (unsigned char *) rsa_decrypt((const long long *) (message->data + 4), (*((int*) message->data)), &key);
    memcpy(new_message + 4, decrypt_message, new_size - 4);
    message->size = new_size;
    *((int*) new_message) = (int)new_size - 4;
    free(message->data);
    message->data = new_message;
    free(decrypt_message);
}

int main(int argc, char const *argv[]) {

    if (argc == 2 && !strcmp("key-gen", argv[1])) {
        create_keys();
        return 0;
    }

    if (argc < 4 || argc > 5 || (argc == 3 && !strcmp("decode", argv[1])) ||
        (argc == 4 && !strcmp("encode", argv[1]))) {
        printf("   usage %s encode <in file> <out file> <key>\nor usage %s decode <in file> <key>\nor usage %s key-gen\n", argv[0], argv[0], argv[0]);
        return 1;
    }
    int width, height, channels;
    unsigned char* image = stbi_load(argv[2], &width, &height, &channels, 0);
    unsigned long image_size = (unsigned long)width * height * channels;
    if (!strcmp("encode", argv[1])) {
        Data message = readInput(stdin);
        encrypt(&message, argv[4]);
        encode(message, image, image_size);
        switch (argv[3][strlen(argv[3]) - 1])
        {
            case 'p':
                stbi_write_bmp(argv[3], width, height, channels, image);
                break;
            case 'g':
                printf("%d\n", stbi_write_png(argv[3], width, height, channels, image, width * channels));
                break;
        }
    }
    else {
        Data result = decode(image, image_size);
        decrypt(&result, argv[3]);
        int size = *((int*) result.data);
        printf("%.*s\n", size, result.data + sizeof(int));
    }

    stbi_image_free(image);
    return 0;
}