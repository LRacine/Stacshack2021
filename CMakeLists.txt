cmake_minimum_required(VERSION 3.17)
project(Stacshack2021 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -Wall -Wextra -lm")

add_executable(Stacshack2021
        main.c decodeencode.h decodeencode.c cbmp.c cbmp.h copybitmap.c copybitmap.h stb_image.h stb_image_resize.h stb_image_write.h)
