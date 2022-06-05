//
// Created by diogo on 31/03/22.
//

#ifndef CLASS4_PONTO_H
#define CLASS4_PONTO_H
#include "../Utils/utils.h"

class Ponto {
public:
    float x;
    float y;
    float z;

    Ponto(){
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Ponto(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float get(int i){
        if(i== 0)return x;
        if(i== 1)return y;
        if(i== 2)return z;
        return 0;
    }


    Ponto getNormal();


};

#endif //CLASS4_PONTO_H
