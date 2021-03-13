//
// Created by seann on 13/03/2021.
//

#ifndef STACSHACK2021_DECODEENCODE_H
#define STACSHACK2021_DECODEENCODE_H

typedef struct Data
{
    unsigned long size;
    unsigned char* data;
} Data;

void encode(Data data, unsigned char* image, unsigned long image_size);
Data decode(const unsigned char* image, unsigned long image_size);

#endif //STACSHACK2021_DECODEENCODE_H
