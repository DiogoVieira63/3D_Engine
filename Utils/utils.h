//
// Created by diogo on 28/04/22.
//

#ifndef CLASS4_UTILS_H
#define CLASS4_UTILS_H

#include <cmath>



class utils {
public:


    static void multMatrixVector(float m[4][4], float *v, float *res);

    static void buildRotMatrix(float *x, float *y, float *z, float *m);


    static void cross(float *a, float *b, float *res);


    static void normalize(float *a);
};


#endif //CLASS4_UTILS_H
