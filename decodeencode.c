//
// Created by seann on 13/03/2021.
//

#include <stdlib.h>
#include <stdint.h>
#include "decodeencode.h"

#define getImageByte(data, index, size) ((index) < (size) ? (data)[index] : 0)

void encode(Data data, unsigned char* image, unsigned long image_size)
{
    int bits = 1;
    int bit_mask = 1;
    while (data.size * 8 > image_size * bits)
    {
        bit_mask |= bit_mask << 1;
        bits++;
    }
    image[0] |= bit_mask;
    image[0] = ~(~image[0] | (bit_mask >> 1) + 1);

    unsigned long image_index = 0;
    int byte_index = 0;
    int bit_index = 0;
    while (image_index++ < image_size)
    {
        uint16_t data_temp = (uint16_t) getImageByte(data.data, byte_index, data.size)
                          | (uint16_t) getImageByte(data.data, byte_index + 1, data.size) << 8;
        data_temp &= bit_mask << bit_index;
        data_temp >>= bit_index;
        image[image_index] = ~(~image[image_index] | bit_mask) | data_temp;

        bit_index += bits;
        if(bit_index >= 8)
        {
            bit_index -= 8;
            byte_index++;
        }
    }
}

Data decode(const unsigned char* image, unsigned long image_size)
{
    int bits = 1;
    int mask = 1;
    while (image[0] & mask)
    {
        bits++;
        mask <<= 1;
    }

    unsigned long total_bits = bits * (image_size - 1);
    unsigned long total_bytes = total_bits / 8;
    unsigned char* out_data = calloc(total_bytes, 1);

    int data_mask = 0;
    do
    {
        data_mask |= mask;
        mask >>= 1;
    }
    while (mask > 0);

    unsigned long image_index = 1;
    int byte_index = 0;
    int bit_index = 0;
    while(byte_index < total_bytes)
    {
        unsigned char data = image[image_index] & (data_mask);
        data <<= bit_index;
        int overflow_mask = 0xFF00;
        unsigned char overflow_data = (data & overflow_mask) >> 8;
        data &= 0xFF;
        bit_index += bits;
        out_data[byte_index] |= data;
        if(bit_index >= 8)
        {
            bit_index -= 8;
            byte_index++;
        }
        if(overflow_data && byte_index < total_bytes)
            out_data[byte_index] |= overflow_data;

        image_index++;
    }

    Data out;
    out.data = out_data;
    out.size = total_bits / 8;
    return out;
}