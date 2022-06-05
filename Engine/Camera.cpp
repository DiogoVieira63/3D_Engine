//
// Created by diogo on 30/04/22.
//

#include "Camera.h"


void Camera::setPos(float posX, float posY, float posZ) {
    this->pos[0] = posX;
    this->pos[1] = posY;
    this->pos[2] = posZ;

}

void Camera::setAlphaBeta(float alpha1, float beta1) {
    this->alpha = alpha1;
    this->beta = beta1;

}

void Camera::setUp(float upX, float upY, float upZ) {
    this->up[0] = upX;
    this->up[1] = upY;
    this->up[2] = upZ;

}

void Camera::setD(float dX, float dY, float dZ) {
    this->d[0] = dX;
    this->d[1] = dY;
    this->d[2] = dZ;
}

void Camera::changeDir(float deltaX, float deltaY) {
    float alphaAux = utils::toDegree(alpha) + deltaX;
    float betaAux =  utils::toDegree(beta) +  deltaY;

    if (betaAux > 89.0)
        betaAux = 89.0;
    else if (betaAux < -89.0)
        betaAux = -89.0;

    d[0] = cos(utils::toRad(betaAux)) * sin(utils::toRad(alphaAux));
    d[1] = sin(utils::toRad(betaAux));
    d[2] = cos(utils::toRad(betaAux)) * cos(utils::toRad(alphaAux));


    utils::normalize(d);

}

void Camera::moveCamera(float deltaY,float pX,float pY,float pZ) {
    int kAux = sensibility * (1 - deltaY);

    pX += (d[0] * kAux);
    pY += (d[1] * kAux);
    pZ += (d[2] * kAux);


    pos[0] = abs(pX) < radiusMax ? pX : pos[0];
    pos[1] = abs(pY) < radiusMax ? pY : pos[1];
    pos[2] = abs(pZ) < radiusMax ? pZ : pos[2];
}

void Camera::updateDirAngles(int xx, int yy,int startX,int startY) {
    float alphaAux = utils::toDegree(alpha) + sensibility * (xx - startX);
    alpha = utils::toRad(alphaAux) ;


    float betaAux = utils::toDegree(beta) + sensibility * (yy - startY);
    beta = utils::toRad(betaAux);

}

void Camera::setCamera() {
    gluLookAt(pos[0], pos[1],pos[2],
              pos[0] + d[0], pos[1] + d[1], pos[2] + d[2],
              up[0], up[1], up[2]);
    for(auto l: lights)
        l->doAction();


}

void Camera::setRadiusMax(float rMax) {
    this->radiusMax = rMax;
}
