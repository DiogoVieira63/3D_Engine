//
// Created by diogo on 31/03/22.
//

#ifndef CLASS4_MODEL_H
#define CLASS4_MODEL_H

#include <vector>
#include <string>
#include "Ponto.h"

using namespace std;

class Model {
public:
    vector<Ponto> pontos;
    string filename;
    pair<unsigned int,unsigned int> parArray;

    Model(const char *file);

    void addPonto(Ponto p);

    void setSize(int n);
};


#endif //CLASS4_MODEL_H
