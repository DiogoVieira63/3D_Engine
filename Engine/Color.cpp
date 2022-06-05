//
// Created by diogo on 02/06/22.
//

#include "Color.h"
#include <stdio.h>
void Color::setDiffuse(float r, float g, float b) {
    diffuse[0] = r/255;
    diffuse[1] = g/255;
    diffuse[2] = b/255;
}

void Color::setAmbient(float r, float g, float b) {
    ambient[0] = r/255;
    ambient[1] = g/255;
    ambient[2] = b/255;
}

void Color::setSpecular(float r, float g, float b) {
    specualar[0] = r/255;
    specualar[1] = g/255;
    specualar[2] = b/255;
}

void Color::setEmissive(float r, float g, float b) {
    emissive[0] = r/255;
    emissive[1] = g/255;
    emissive[2] = b/255;
}

void Color::doColor() {
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specualar);
    glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

}
