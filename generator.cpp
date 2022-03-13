//
// Created by diogo on 12/03/22.
//
#include <iostream>
#include <fstream>
#include <list>
#include <iterator>
#include <cstring>

using namespace std;

#define _USE_MATH_DEFINES
#include <math.h>



void writeFile(list<float> list,char* filename){
    ofstream file;
    file.open(filename);
    ::list<float>::iterator it;
    int nrVertices = list.size()/3;
    file << nrVertices << '\n';
    for (it = list.begin(); it != list.end(); ++it) {
        file << *it << ' ';
        ++it;
        file << *it << ' ';
        ++it;
        file << *it << ' ';
        file << '\n';
    }
}

void addVerticeToList(list<float>&list, float x,float y, float z){
    list.push_back(x);
    list.push_back(y);
    list.push_back(z);
}




list<float>* getPointsPlane (float length,int divisions){

    float inicial_posX = - length/2;
    float inicial_posZ = inicial_posX;
    float part = length/(float )divisions;

    list<float> *list = new ::list<float>;

    for (int i = 0;i < divisions;i++){
        for (int j = 0;j < divisions;j++){
            float posX = inicial_posX + i * part;
            float posZ = inicial_posZ + j * part;

            addVerticeToList(*list, posX + part, 0, posZ + part);
            addVerticeToList(*list, posX + part, 0, posZ);
            addVerticeToList(*list, posX, 0, posZ);


            if (i ==0 || j == divisions-1) {
                addVerticeToList(*list,posX,0,posZ);
                addVerticeToList(*list,posX,0,posZ+part);
                addVerticeToList(*list,posX+part,0,posZ+part);
            }
        }
    }
    return list;
}


list<float>* getPointsCone(float r, float height, int slices,int stacks) {
    float anglePart = 2 * M_PI / slices;
    float heightPart = height/(float)stacks;
    float initialHeight =0;
    list<float> *list = new ::list<float>;

    for (int i = 0; i < slices;i++) {
        float angle = ((float) i) * anglePart;
        float nextAngle = angle + anglePart;
        for (int j = 0; j < stacks; j++) {

            float rDown =  r * (float )(stacks-j)/(float )stacks;
            float heightDown = initialHeight+heightPart*(float)j;
            float rUp = r * (float )(stacks-j-1)/(float )stacks;
            float heightUP = initialHeight + heightPart * (float)(j+1);


            addVerticeToList(*list,rUp* sin(nextAngle),heightUP,rUp* cos(nextAngle));
            addVerticeToList(*list,rUp* sin(angle),heightUP,rUp* cos(angle));
            addVerticeToList(*list,rDown * sin(angle), heightDown, rDown * cos(angle));

            addVerticeToList(*list,rDown* sin(angle), heightDown, rDown * cos(angle));
            addVerticeToList(*list,rDown* sin(nextAngle),heightDown,rDown* cos(nextAngle));
            addVerticeToList(*list,rUp* sin(nextAngle), heightUP, rUp* cos(nextAngle));

        }

        addVerticeToList(*list,r * sin(nextAngle), initialHeight, r * cos(nextAngle));
        addVerticeToList(*list,r * sin(angle), initialHeight, r * cos(angle));
        addVerticeToList(*list,0, initialHeight, 0);

    }
    return list;
}


list<float>* getPointsSphere(float radius, int slices,int stacks){
    float anglePart = 2 * M_PI / slices;
    float heightPart = M_PI/stacks;
    float initialAngle = -M_PI/2;
    list<float> *list = new ::list<float>;
    for (int i = 0; i < slices;i++) {
        float angle = ((float) i) * anglePart;
        float nextAngle = angle + anglePart;
        for (int j = 0; j < stacks; j++) {

            float angleUp =   initialAngle+ heightPart*(float)(j+1);
            float angleDown = initialAngle+ heightPart*(float)(j);


            if(angleUp>=M_PI/2)angleUp=M_PI/2-0.00001;
            if(angleDown<=-M_PI/2)angleDown=-M_PI/2+0.000001;


            addVerticeToList(*list,radius* cos(angleUp)*sin(angle),radius*sin(angleUp),radius* cos(angle)* cos(angleUp) ) ;
            addVerticeToList(*list,radius* cos(angleDown)*sin(angle),radius*sin(angleDown),radius* cos(angle)* cos(angleDown));
            addVerticeToList(*list,radius* cos(angleDown)*sin(nextAngle),radius*sin(angleDown), radius * cos(nextAngle)* cos(angleDown));

            addVerticeToList(*list,radius* cos(angleUp)*sin(nextAngle),radius* sin(angleUp), radius * cos(nextAngle)* cos(angleUp));
            addVerticeToList(*list,radius* cos(angleUp)*sin(angle),radius* sin(angleUp),radius* cos(angle)* cos(angleUp));
            addVerticeToList(*list,radius* cos(angleDown)*sin(nextAngle), radius* sin(angleDown), radius* cos(nextAngle)* cos(angleDown));

        }
    }
    return list;
}

list<float>* getPointsBox (float length, int divisions){
    float part = length/(float)divisions;
    float halfAltura = length/2;
    float initX = - halfAltura;
    float initZ = initX;
    list<float> *list = new ::list<float>;

    // Y Constante
    for (int i = 0;i < divisions;i++){
        for (int j = 0;j < divisions;j++){
            float posX = initX + i * part;
            float posZ = initZ + j * part;
            float nextPosX = posX + part;
            float nextPosZ = posZ + part;


            addVerticeToList(*list,nextPosX,halfAltura,nextPosZ);
            addVerticeToList(*list,nextPosX,halfAltura,posZ);
            addVerticeToList(*list,posX,halfAltura,posZ);


            addVerticeToList(*list, posX, halfAltura, posZ);
            addVerticeToList(*list, posX, halfAltura, nextPosZ);
            addVerticeToList(*list, nextPosX, halfAltura, nextPosZ);


            addVerticeToList(*list,posX,-halfAltura,posZ);
            addVerticeToList(*list,nextPosX,-halfAltura,posZ);
            addVerticeToList(*list,nextPosX,-halfAltura,nextPosZ);


            addVerticeToList(*list, nextPosX, -halfAltura, nextPosZ);
            addVerticeToList(*list, posX, -halfAltura, nextPosZ);
            addVerticeToList(*list, posX, -halfAltura, posZ);

        }
    }
    // X constante
    for (int i = 0; i < divisions;i++){
        for (int j = 0; j < divisions;j++){
            float posY = halfAltura - i * part;
            float posZ = initZ + j * part;
            float netxPosZ = posZ + part;
            float nextPosY=posY-part;
            addVerticeToList(*list,initX,posY, netxPosZ);
            addVerticeToList(*list,initX,posY, posZ);
            addVerticeToList(*list,initX,nextPosY, posZ);


            addVerticeToList(*list, initX, posY, posZ + part);
            addVerticeToList(*list, initX, nextPosY, posZ);
            addVerticeToList(*list, initX, nextPosY, netxPosZ);

            addVerticeToList(*list,-initX,nextPosY, posZ);
            addVerticeToList(*list,-initX,posY, posZ);
            addVerticeToList(*list,-initX,posY, netxPosZ);

            addVerticeToList(*list, -initX, nextPosY, netxPosZ);
            addVerticeToList(*list, -initX, nextPosY, posZ);
            addVerticeToList(*list, -initX, posY, netxPosZ);

        }
    }
    //Z constante
    for (int i = 0; i < divisions;i++){
        for (int j = 0; j < divisions;j++){
            float posY = halfAltura - i * part;
            float posX = initX + j * part;
            float nextPosY=posY-part;
            float nextPosX=posX+part;


            addVerticeToList(*list,posX,nextPosY, initZ);
            addVerticeToList(*list,posX,posY, initZ);
            addVerticeToList(*list,nextPosX,posY, initZ);

            addVerticeToList(*list, nextPosX, nextPosY, initZ);
            addVerticeToList(*list, posX, nextPosY, initZ);
            addVerticeToList(*list, nextPosX, posY, initZ);


            addVerticeToList(*list,posX + part,posY, -initZ);
            addVerticeToList(*list,posX,posY, -initZ);
            addVerticeToList(*list,posX,nextPosY, -initZ);

            addVerticeToList(*list, nextPosX, posY, -initZ);
            addVerticeToList(*list, posX, nextPosY, -initZ);
            addVerticeToList(*list, nextPosX, nextPosY, -initZ);

        }
    }
    return list;
}

list<float>* getPointsCylinder(float r, float height, int slices, int stacks) {
    float anglePart = 2 * M_PI / slices;
    float halfHeight = height / 2;
    float heightPart = height/(float)stacks;

    list<float> *list = new ::list<float>;



    for (int i = 0; i < slices; i++) {


        float angle = ((float) i) * anglePart;
        float nextAngle = angle + anglePart;

        addVerticeToList(*list, 0, halfHeight, 0);
        addVerticeToList(*list, r * sin(angle), halfHeight, r * cos(angle));
        addVerticeToList(*list, r * sin(nextAngle), halfHeight, r * cos(nextAngle));

        addVerticeToList(*list, r * sin(nextAngle), -halfHeight, r * cos(nextAngle));
        addVerticeToList(*list, r * sin(angle), -halfHeight, r * cos(angle));
        addVerticeToList(*list, 0, -halfHeight, 0);

        for (int j = 0; j < stacks; j++) {
            float heightUp = -halfHeight + (heightPart * (float )(j+1));
            float heightDown = -halfHeight + (heightPart * (float )(j));


            addVerticeToList(*list, r * sin(angle), heightUp, r * cos(angle));
            addVerticeToList(*list, r * sin(angle), heightDown, r * cos(angle));
            addVerticeToList(*list, r * sin(nextAngle), heightDown, r * cos(nextAngle));

            addVerticeToList(*list, r * sin(nextAngle), heightUp, r * cos(nextAngle));
            addVerticeToList(*list, r * sin(angle), heightUp, r * cos(angle));
            addVerticeToList(*list, r * sin(nextAngle), heightDown, r * cos(nextAngle));

        }
    }
    return list;
}


void printErro(const string& str){
    cout << "Erro:" <<str << "\n";
    cout <<"For info -> ./generator help\n";
}



int main(int argc, char **argv){
    if (argc <=2) {
        if(!strcmp(argv[1],"help")){
            printf("Graphical primitives available:\n\n");
            printf("Plane (a square in the XZ plane, centred in the origin, subdivided in both X and Z directions\n");
            printf("Example ->./generator plane [length] [division] [filename]\n");
            printf("\nBox (requires dimension, and the number of divisions per edge)\n");
            printf("Example ->./generator box [length] [division] [filename]\n");
            printf("\nSphere (requires radius, slices and stacks)\n");
            printf("Example ->./generator sphere [radius] [slices] [stacks] [filename]\n");
            printf("\nCone (requires bottom radius, height, slices and stacks)\n");
            printf("Example ->./generator cone [radius] [height] [slices] [stacks] [filename]\n");
            printf("\nCylinder (requires radius, height, slices and stacks)\n");
            printf("Example ->./generator cylinder [radius] [height] [slices] [stacks] [filename]\n");
        }
        else printErro("nº de argumentos inválido");
        return 0;
    }

    char *primitive = argv[1];
    if (!strcmp(primitive,"sphere")){
        if (argc != 6){
            printErro("nº de argumentos inválido");
            return 0;
        }
        try {
            float radius = std::stof(argv[2]);
            int slices = std::stoi(argv[3]);
            int stacks = std::stoi(argv[4]);
            list<float> *list = getPointsSphere(radius,slices,stacks);
            char *filename = argv[5];
            writeFile(*list,filename);
            printf("%s gerado\n",filename);


        }
        catch (std::exception& ia){
            printErro("Argumentos inválidos");
        }
    }
    else if (!strcmp(primitive,"box")){
        if (argc != 5){
            printErro("nº de argumentos inválido");
            return 0;
        }
        try {
            float length = std::stof(argv[2]);
            int divisions = std::stoi(argv[3]);
            list<float> *list = getPointsBox(length,divisions);
            char *filename = argv[4];
            writeFile(*list,filename);
            printf("%s gerado\n",filename);

        }
        catch (std::exception& ia){
            printErro("Argumentos inválidos");
        }
    }
    else if (!strcmp(primitive,"cone")){
        if (argc != 7){
            printErro("nº de argumentos inválido");
            return 0;
        }
        try {
            float radius = std::stof(argv[2]);
            float height = std::stof(argv[3]);
            int slices = std::stoi(argv[4]);
            int stacks = std::stoi(argv[5]);
            list<float> *list = getPointsCone(radius,height,slices,stacks);
            char *filename = argv[6];
            writeFile(*list,filename);
            printf("%s gerado\n",filename);

        }
        catch (std::exception& ia){
            printErro("Argumentos inválidos");
        }

    }
    else if (!strcmp(primitive,"plane")){
        if (argc != 5){
            printErro("nº de argumentos inválido");
            return 0;
        }
        try {
            float length = std::stof(argv[2]);
            int divisions = std::stoi(argv[3]);
            list<float> *list = getPointsPlane(length,divisions);
            char *filename = argv[4];
            writeFile(*list,filename);
            printf("%s gerado\n",filename);
        }
        catch (std::exception& ia){
            printErro("Argumentos inválidos");
        }
    }
    else if (!strcmp(primitive,"cylinder")){
        try {
            if (argc != 7){
                printErro("nº de argumentos inválido");
                return 0;
            }
            float radius = std::stof(argv[2]);
            float height = std::stof(argv[3]);
            int slices= std::stoi(argv[4]);
            int stacks= std::stoi(argv[5]);
            list<float> *list = getPointsCylinder(radius,height,slices,stacks);
            char *filename = argv[6];
            writeFile(*list,filename);
            printf("%s gerado\n",filename);

        }
        catch (std::exception& ia){
            printErro("Argumentos inválidos");
        }
    }
    else printErro("Argumentos inválidos");
}