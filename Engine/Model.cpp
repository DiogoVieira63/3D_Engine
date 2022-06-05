//
// Created by diogo on 31/03/22.
//

#include "Model.h"



Model::Model (const char *file) {
    filename = string(file);
    color = Color();
    parText= pair<bool,unsigned int>(false,-2);
}

void Model::addPonto(Ponto p) {
    pontos.push_back(p);
}

void Model::setSize(int n) {
    pontos.clear();
    pontos.reserve(n);
}

void Model::setTexFilename(const char *file){
    texFilename = string(file);
    parText= pair<bool,unsigned int>(true,-1);
}