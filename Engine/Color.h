//
// Created by diogo on 31/03/22.
//

#ifndef CLASS4_COLOR_H
#define CLASS4_COLOR_H


class Color {
public:
    float R;
    float G;
    float B;

    Color() {
        R = -1;
        G = 1;
        B = 1;
    }

    Color(float r, float g, float b) {
        R = r / 255;
        G = g / 255;
        B = b / 255;
    }
};


#endif //CLASS4_COLOR_H
