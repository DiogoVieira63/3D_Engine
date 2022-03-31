//
// Created by diogo on 31/03/22.
//

#include "Group.h"



void Group::addModel(Model *m) {
    models.push_back(m);
}

void Group::addRotation(Rotation* r){
    transform.push_back(r);
}

void Group::addTranslation(Translation *t){
    transform.push_back(t);
}

void Group::addScale(Scale* s){
    transform.push_back(s);
}

void Group::addSubGroup(Group *g){
    subGroups.push_back(g);
}