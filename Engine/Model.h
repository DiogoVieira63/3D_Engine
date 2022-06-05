//
// Created by diogo on 31/03/22.
//

#ifndef CLASS4_MODEL_H
#define CLASS4_MODEL_H

#include <vector>
#include <string>
#include "Ponto.h"
#include "Color.h"

using namespace std;

class Model {
public:
    vector<Ponto> pontos;
    string filename;
    pair<unsigned int*,unsigned int> parArray;
    Color color;
    string texFilename;
    pair<bool,unsigned int> parText;

    Model(const char *file);

    void addPonto(Ponto p);

    void setSize(int n);

    void setTexFilename(const char *file);
};


#endif //CLASS4_MODEL_H
