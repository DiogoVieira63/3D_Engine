//
// Created by diogo on 02/06/22.
//

#include "Ponto.h"


Ponto Ponto::getNormal(){
    float v[3] ={x,y,z};
    utils::normalize(v);
    return {v[0],v[1],v[2]};
}