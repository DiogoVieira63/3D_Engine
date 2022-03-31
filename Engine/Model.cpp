//
// Created by diogo on 31/03/22.
//

#include "Model.h"



Model::Model (const char *file) {
    filename = string(file);
}

void Model::addPonto(Ponto p) {
    pontos.push_back(p);
}

void Model::setSize(int n) {
    pontos.clear();
    pontos.reserve(n);
}
