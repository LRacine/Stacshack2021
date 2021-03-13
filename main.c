//
// Created by seann on 13/03/2021.
//

#include <stdio.h>
#include "decodeencode.h"

int main()
{
    Data test;
    unsigned char testmsg[] = "test1234";
    test.data = testmsg;
    test.size = sizeof(testmsg);
    unsigned char testimg[] = "lkdfsjkfdllfdsfslkfdslkjflkdsfldskjfdslkj";
    unsigned char* image = testimg;
    unsigned long image_size = sizeof(testimg) - 1;
    encode(test, image, image_size);
    printf("%s\n", image);
    Data result = decode(image, image_size);
    printf("%s\n", result.data);
    return 0;
}