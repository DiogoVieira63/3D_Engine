//
// Created by diogo on 30/04/22.
//

#ifndef CLASS4_XMLPARSER_H
#define CLASS4_XMLPARSER_H

#include <GL/glew.h>
#include <GL/glut.h>

#include "../tinyxml2.h"
#include <map>
#include <iostream>
#include <utility>
#include <fstream>




using namespace tinyxml2;
using namespace std;


#include "Group.h"
#include "Camera.h"
#include "../Utils/utils.h"
#include <IL/il.h>

class xmlParser {
public:
    Camera *cam;


    xmlParser(Camera *cam,GLuint *textures){
        this->cam = cam;
        this->textures = textures;
    }

    Group* readXml(const char *filename);

private:
    GLuint *textures;
    map<string, pair<unsigned int *,unsigned int>> mapArraysModel;
    map<string ,pair<bool,unsigned int>> mapTextures;
    void readFile(Model *m);
    Group * readGroup(XMLElement *pElement);
    void readModels(Group *g);

    vector<float> readVerticesFileText(ifstream *pIfstream);

    void readColor(XMLElement *pElement, Model *m);


    void setupTexture(Model *m);

    void loadTexture(Model *m, GLuint num);
};


#endif //CLASS4_XMLPARSER_H
