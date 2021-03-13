//
// Created by seann on 13/03/2021.
//

#include <stdio.h>
#include "decodeencode.h"
#include "copybitmap.h"
#include "cbmp.h"


void cleanup(unsigned char* image, BMP* bmp) {
    destroy(image);
    bclose(bmp);
}


int main() {
    BMP* bmp = bopen("../images/bond.bmp");

    int image_size = bmp->height * bmp->width * 3;
    unsigned char* image = extract_rbg_only(bmp, image_size);

    Data test;
    unsigned char testmsg[] = "test1234";
    test.data = testmsg;
    test.size = sizeof(testmsg);

    encode(test, image, image_size);
    add_bytes_back(bmp, image);

    Data result = decode(image, image_size);
    printf("%s\n", result.data);

    bwrite(bmp, "../images/result1.bmp");

    bmp = bopen("../images/result1.bmp");
    image = extract_rbg_only(bmp, image_size);
    result = decode(image, image_size);
    printf("%s\n", result.data);

    cleanup(image, bmp);
    return 0;
}