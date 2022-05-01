//
// Created by diogo on 28/04/22.
//

#ifndef CLASS4_UTILS_H
#define CLASS4_UTILS_H

#include <cmath>
#include <stdio.h>
#include "../Engine/Ponto.h"


class utils {
public:


    static void multMatrixVector(float m[4][4], float *v, float *res);

    static void buildRotMatrix(float *x, float *y, float *z, float *m);


    static void cross(float *a, float *b, float *res);


    static void normalize(float *a);

    static void printPonto(Ponto p);

    static float toDegree(float angle);

    static float toRad(float angle);


};


#endif //CLASS4_UTILS_H
