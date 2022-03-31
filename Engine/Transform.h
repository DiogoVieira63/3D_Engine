//
// Created by diogo on 31/03/22.
//

#ifndef CLASS4_TRANSFORM_H
#define CLASS4_TRANSFORM_H


class Transform {
public:
    float x;
    float y;
    float z;

    Transform(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    virtual void doAction() = 0;
};


#endif //CLASS4_TRANSFORM_H
