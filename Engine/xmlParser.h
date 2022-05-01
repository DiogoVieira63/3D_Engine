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


class xmlParser {
public:
    map<string, pair<unsigned int,unsigned int>> mapArraysModel;
    Camera *cam;


    xmlParser(map<string, pair<unsigned int,unsigned int>> mapArraysModel,Camera *cam){
        this->mapArraysModel = move(mapArraysModel);
        this->cam = cam;
    }

    Group* readXml(const char *filename);

private:
    int readFile(Model *m);
    Group * readGroup(XMLElement *pElement);
    void readModels(Group *g);

};


#endif //CLASS4_XMLPARSER_H
