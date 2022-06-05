//
// Created by diogo on 28/04/22.
//

#ifndef CLASS4_UTILS_H
#define CLASS4_UTILS_H

#include <cmath>
#include <stdio.h>


class utils {
public:

    static bool rotate;

    static void multMatrixVector(float m[4][4], float *v, float *res);

    static void buildRotMatrix(float *x, float *y, float *z, float *m);


    static void cross(float *a, float *b, float *res);


    static void normalize(float *a);


    static float toDegree(float angle);

    static float toRad(float angle);


};


#endif //CLASS4_UTILS_H
