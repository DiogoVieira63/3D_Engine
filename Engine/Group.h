//
// Created by diogo on 31/03/22.
//

#ifndef CLASS4_GROUP_H
#define CLASS4_GROUP_H


#include "Transform.h"
#include "Model.h"
#include "Color.h"
#include "Rotation.h"
#include "Translation.h"
#include "Scale.h"

class Group {
public:
    vector<Transform *> transform;
    vector<Model *> models;
    vector<Group *> subGroups;
    Color cor;

    void addModel(Model *m);

    void addRotation(Rotation* r);

    void addTranslation(Translation *t);

    void addScale(Scale* s);

    void addSubGroup(Group *g);

};


#endif //CLASS4_GROUP_H
