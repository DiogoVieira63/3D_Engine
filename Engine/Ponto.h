//
// Created by diogo on 31/03/22.
//

#ifndef CLASS4_PONTO_H
#define CLASS4_PONTO_H


class Ponto {
public:
    float x;
    float y;
    float z;

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


};

#endif //CLASS4_PONTO_H
