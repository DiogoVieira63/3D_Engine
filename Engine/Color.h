//
// Created by diogo on 31/03/22.
//

#ifndef CLASS4_COLOR_H
#define CLASS4_COLOR_H

#include <GL/gl.h>


class Color {
public:
    float diffuse[4] = {0.784,0.784,0.784,1};
    float ambient[4] = {0.196,0.196,0.196,1};
    float specualar[4] = {0,0,0,1};
    float emissive[4] = {0,0,0,1};
    float shininess = 0;


    void setDiffuse(float r,float g,float b);
    void setAmbient(float r,float g,float b);
    void setSpecular(float r,float g,float b);
    void setEmissive(float r,float g,float b);

    void doColor();


};


#endif //CLASS4_COLOR_H
