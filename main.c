//
// Created by seann on 13/03/2021.
//

#include <stdio.h>
#include "decodeencode.h"
#include "cbmp.h"

int main()
{
    BMP* bmp = bopen("./images/bond.bmp");

    Data test;
    unsigned char testmsg[] = "test1234";
    test.data = testmsg;
    test.size = sizeof(testmsg);
    unsigned char* image = (unsigned char *) bmp->pixels;
    unsigned long image_size = sizeof((unsigned char *)bmp->pixels);
    encode(test, image, image_size);
    Data result = decode(image, image_size);
    printf("%s\n", result.data);
    return 0;
}