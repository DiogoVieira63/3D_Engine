//
// Created by diogo on 12/03/22.
//
#include <iostream>
#include <fstream>
#include <list>
#include <iterator>
#include <cstring>
#include <vector>

using namespace std;

#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include "../Engine/Ponto.h"

void writeFile(vector<Ponto> vect, char *filename){
    ofstream file;
    file.open(filename);
    int nrVertices = vect.size();
    file << nrVertices << '\n';
    for (Ponto p : vect){
        file << p.x << ' ';
        file << p.y << ' ';
        file << p.z << ' ';
        file << '\n';
    }
}


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

void addTriangle(vector<Ponto> *vector,Ponto p0,Ponto p1,Ponto p2){
    vector->push_back(p0);
    vector->push_back(p1);
    vector->push_back(p2);

}


vector<Ponto> getPointsPlane (float length,int divisions){

    float inicial_posX = - length/2;
    float inicial_posZ = inicial_posX;
    float part = length/(float )divisions;

    vector<Ponto>vector;


    for (int i = 0;i < divisions;i++){
        float posX = inicial_posX + (float) i * part;
        for (int j = 0;j < divisions;j++){
            float posZ = inicial_posZ + (float) j * part;

            Ponto upNow = Ponto(posX, 0, posZ);
            Ponto upNext = Ponto(posX + part, 0, posZ);
            Ponto botNext = Ponto(posX + part, 0, posZ + part);
            Ponto botNow = Ponto(posX, 0, posZ + part);

            addTriangle(&vector,botNext,upNext,upNow);
            addTriangle(&vector,upNow,upNext,botNext);

            addTriangle(&vector,upNow,botNow,botNext);
            addTriangle(&vector,botNext,botNow,upNow);

        }
    }
    return vector;
}


vector<Ponto> getPointsCone(float r, float height, int slices,int stacks) {
    float anglePart = 2 * M_PI / slices;
    float heightPart = height/(float)stacks;
    float initialHeight =0;
    vector<Ponto> vector;

    for (int i = 0; i < slices;i++) {
        float angle = ((float) i) * anglePart;
        float nextAngle = angle + anglePart;
        for (int j = 0; j < stacks; j++) {

            float rDown =  r * (float )(stacks-j)/(float )stacks;
            float heightDown = initialHeight+heightPart*(float)j;
            float rUp = r * (float )(stacks-j-1)/(float )stacks;
            float heightUP = initialHeight + heightPart * (float)(j+1);


            Ponto upNext = Ponto(rUp* sin(nextAngle),heightUP,rUp* cos(nextAngle));
            Ponto upNow = Ponto(rUp* sin(angle),heightUP,rUp* cos(angle));
            Ponto downNow = Ponto(rDown* sin(angle),heightDown,rDown* cos(angle));
            Ponto downNext = Ponto(rDown* sin(nextAngle),heightDown,rDown* cos(nextAngle));

            addTriangle(&vector,upNext,upNow,downNow);

            addTriangle(&vector,downNow,downNext,upNext);

        }

        Ponto baseNext = Ponto(r * sin(nextAngle), initialHeight, r * cos(nextAngle));
        Ponto baseNow = Ponto(r * sin(angle), initialHeight, r * cos(angle));
        Ponto center = Ponto(0, initialHeight, 0);

        addTriangle(&vector,baseNext,baseNow,center);

    }
    return vector;
}


vector<Ponto> getPointsSphere(float radius, int slices,int stacks){
    float anglePart = 2 * M_PI / slices;
    float heightPart = M_PI/stacks;
    float initialAngle = -M_PI/2;
    //list<float> *list = new ::list<float>;
    vector<Ponto> vector;
    for (int i = 0; i < slices;i++) {
        float angle = ((float) i) * anglePart;
        float nextAngle = angle + anglePart;
        for (int j = 0; j < stacks; j++) {

            float angleUp =   initialAngle+ heightPart*(float)(j+1);
            float angleDown = initialAngle+ heightPart*(float)(j);


            if(angleUp>=M_PI/2)angleUp=M_PI/2-0.00001;
            if(angleDown<=-M_PI/2)angleDown=-M_PI/2+0.000001;

            Ponto upNow = Ponto(radius* cos(angleUp)*sin(angle),radius*sin(angleUp),radius* cos(angle)* cos(angleUp) );
            Ponto upNext = Ponto(radius* cos(angleUp)*sin(nextAngle),radius* sin(angleUp), radius * cos(nextAngle)* cos(angleUp));
            Ponto downNow = Ponto (radius* cos(angleDown)*sin(angle),radius*sin(angleDown),radius* cos(angle)* cos(angleDown));
            Ponto downNext = Ponto(radius* cos(angleDown)*sin(nextAngle),radius*sin(angleDown), radius * cos(nextAngle)* cos(angleDown));

            addTriangle(&vector,upNow,downNow,downNext);

            addTriangle(&vector,upNext,upNow,downNext);


        }
    }
    return vector;
}

vector<Ponto> getPointsBox (float length, int divisions){
    float part = length/(float)divisions;
    float halfAltura = length/2;
    float initX = - halfAltura;
    float initZ = initX;
    //list<float> *list = new ::list<float>;
    vector<Ponto> vector;

    // Y Constante
    for (int i = 0;i < divisions;i++){
        for (int j = 0;j < divisions;j++){
            float posX = initX + i * part;
            float posZ = initZ + j * part;
            float nextPosX = posX + part;
            float nextPosZ = posZ + part;

            Ponto nextXnextZ = Ponto(nextPosX,halfAltura,nextPosZ);
            Ponto XnextZ = Ponto(posX,halfAltura,nextPosZ);
            Ponto nextXZ = Ponto(nextPosX,halfAltura,posZ);
            Ponto XZ = Ponto(posX,halfAltura,posZ);

            addTriangle(&vector,nextXnextZ,nextXZ,XZ);
            addTriangle(&vector,XZ,XnextZ,nextXnextZ);

            Ponto OnextXnextZ = Ponto(nextPosX,-halfAltura,nextPosZ);
            Ponto OXnextZ = Ponto(posX,-halfAltura,nextPosZ);
            Ponto OnextXZ = Ponto(nextPosX,-halfAltura,posZ);
            Ponto OXZ = Ponto(posX,-halfAltura,posZ);

            addTriangle(&vector,OXZ,OnextXZ,OnextXnextZ);
            addTriangle(&vector,OnextXnextZ,OXnextZ,OXZ);

        }
    }
    // X constante
    for (int i = 0; i < divisions;i++){
        for (int j = 0; j < divisions;j++){
            float posY = halfAltura - i * part;
            float posZ = initZ + j * part;
            float nextPosZ = posZ + part;
            float nextPosY=posY-part;

            Ponto nextYnextZ = Ponto(initX, nextPosY, nextPosZ);
            Ponto YnextZ = Ponto(initX, posY, nextPosZ);
            Ponto nextYZ = Ponto(initX, nextPosY, posZ);
            Ponto YZ = Ponto(initX, posY, posZ);


            addTriangle(&vector,YnextZ,YZ,nextYZ);
            addTriangle(&vector,YnextZ,nextYZ,nextYnextZ);


            Ponto OnextYnextZ = Ponto(-initX, nextPosY, nextPosZ);
            Ponto OYnextZ = Ponto(-initX, posY, nextPosZ);
            Ponto OnextYZ = Ponto(-initX, nextPosY, posZ);
            Ponto OYZ = Ponto(-initX, posY, posZ);


            addTriangle(&vector,OnextYZ,OYZ,OYnextZ);
            addTriangle(&vector,OnextYnextZ,OnextYZ,OYnextZ);


        }
    }
    //Z constante
    for (int i = 0; i < divisions;i++){
        for (int j = 0; j < divisions;j++){
            float posY = halfAltura - i * part;
            float posX = initX + j * part;
            float nextPosY=posY-part;
            float nextPosX=posX+part;


            Ponto nextXnextY = Ponto(nextPosX,nextPosY, initZ);
            Ponto XnextY = Ponto(posX,nextPosY, initZ);
            Ponto nextXY = Ponto(nextPosX,posY, initZ);
            Ponto XY = Ponto(posX,posY, initZ);

            addTriangle(&vector,XnextY,XY,nextXY);
            addTriangle(&vector,nextXnextY,XnextY,nextXY);


            Ponto OnextXnextY = Ponto(nextPosX,nextPosY, -initZ);
            Ponto OXnextY = Ponto(posX,nextPosY, -initZ);
            Ponto OnextXY = Ponto(nextPosX,posY, -initZ);
            Ponto OXY = Ponto(posX,posY, -initZ);

            addTriangle(&vector,OnextXY,OXY,OXnextY);
            addTriangle(&vector,OnextXY,OXnextY,OnextXnextY);


        }
    }
    return vector;
}

vector<Ponto> getPointsCylinder(float r, float height, int slices, int stacks) {
    float anglePart = 2 * M_PI / slices;
    float halfHeight = height / 2;
    float heightPart = height/(float)stacks;


    vector<Ponto> vector;


    for (int i = 0; i < slices; i++) {


        float angle = ((float) i) * anglePart;
        float nextAngle = angle + anglePart;

        Ponto baseNow = Ponto(r * sin(angle), halfHeight, r * cos(angle));
        Ponto baseNext= Ponto(r * sin(nextAngle), halfHeight, r * cos(nextAngle));
        Ponto center = Ponto(0, halfHeight, 0);

        addTriangle(&vector,center,baseNow,baseNext);

        Ponto ObaseNow = Ponto(r * sin(angle), -halfHeight, r * cos(angle));
        Ponto ObaseNext= Ponto(r * sin(nextAngle), -halfHeight, r * cos(nextAngle));
        Ponto Ocenter = Ponto(0, -halfHeight, 0);

        addTriangle(&vector,ObaseNext,ObaseNow,Ocenter);

        for (int j = 0; j < stacks; j++) {
            float heightUp = -halfHeight + (heightPart * (float )(j+1));
            float heightDown = -halfHeight + (heightPart * (float )(j));


            Ponto upNow = Ponto(r * sin(angle), heightUp, r * cos(angle));
            Ponto upNext = Ponto(r * sin(nextAngle), heightUp, r * cos(nextAngle));
            Ponto downNow = Ponto(r * sin(angle), heightDown, r * cos(angle));
            Ponto downNext= Ponto(r * sin(nextAngle), heightDown, r * cos(nextAngle));

            addTriangle(&vector,upNow,downNow,downNext);
            addTriangle(&vector,upNext,upNow,downNext);

        }
    }
    return vector;
}

vector<Ponto> getPointsTorus (float raio_in, float raio_out, int slices, int stacks){
    float delta1= 2* M_PI / slices;
    float delta2= 2* M_PI / stacks;
    float phi = 0;
    float theta=0;

    vector<Ponto> vector;

    for (int i = 0; i < slices; ++i) {
        for (int j = 0; j < stacks; ++j) {

            float posX = (raio_in + raio_out  * cos(phi)) * cos(theta);
            float d1X =  (raio_in + raio_out  * cos(phi))* cos(theta + delta1);
            float d2X =  (raio_in + raio_out  * cos(phi + delta2)) * cos(theta);
            float d3X =  (raio_in + raio_out  * cos(phi + delta2)) * cos(theta+delta1);

            float posY = (raio_in + raio_out  * cos(phi))*sin(theta);
            float d1Y =  (raio_in + raio_out  * cos(phi))*sin(theta+delta1);
            float d2Y =  (raio_in + raio_out  * cos(phi+delta2))*sin(theta);
            float d3Y =  (raio_in + raio_out  * cos(phi+delta2))*sin(theta+delta1);

            float posZ =raio_out  * sin(phi);
            float d2Z = raio_out * sin(phi+delta2);

            Ponto p = Ponto(posX,posY,posZ);
            Ponto pd1 = Ponto(d1X,d1Y,posZ);
            Ponto pd2 = Ponto(d2X,d2Y,d2Z);
            Ponto pd3 = Ponto(d3X,d3Y,d2Z);

            addTriangle(&vector,p,pd1,pd3);
            addTriangle(&vector,pd3,pd2,p);

            phi = delta2 * (j+1);
        }
        theta = delta1 * (i+1);

    }
    return vector;

}



void printErro(const string& str){
    cout << "Erro:" <<str << "\n";
    cout <<"For info -> ./generator help\n";
}


void multMatrixVector(float m[4][4], float *v, float *res) {
    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j][k];
        }
    }

}


Ponto getBezierPoint(float t, Ponto p0,Ponto p1,Ponto p2,Ponto p3){
    //bezier matrix
    float m[4][4] = {	{-1,  3, -3,  1},
                         { 3, -6,  3, 0},
                         {-3,  3,  0,  0.0f},
                         { 1.0f,  0.0f,  0.0f,  0.0f}};


    float pos[3];


    for (int i = 0; i < 3;i++){
        float vector[4] ={p0.get(i),p1.get(i),p2.get(i),p3.get(i)};
        float a[4];
        multMatrixVector(m,vector,a);

        pos[i] = powf(t,3) * a[0] + powf(t,2) * a[1] + t * a[2] + a[3];

    }

    Ponto p(pos[0],pos[1],pos[2]);


    return p;


}



vector<Ponto> getPointsBezier(int tesselation,vector<vector<int>>patchVector, vector<Ponto> pontos){

    float part = 1.0f/(float)tesselation;
    vector <Ponto> vectorRes;
    for (vector<int> patch : patchVector) {
        vector<Ponto> points;
        for (int i = 0; i <= tesselation; i++) {
            float u = part * (float) i;

            vector<Ponto> curve;

            for (int ii = 0; ii < 4; ii++) {
                int index = ii * 4;
                Ponto p0 = pontos[(patch[index])];
                Ponto p1 = pontos[(patch[index + 1])];
                Ponto p2 = pontos[(patch[index + 2])];
                Ponto p3 = pontos[(patch[index + 3])];

                Ponto p = getBezierPoint(u, p0, p1, p2, p3);
                curve.push_back(p);
            }

            for (int j = 0; j <= tesselation; j++) {
                float v = part * (float) j;
                Ponto p0 = curve[0];
                Ponto p1 = curve[1];
                Ponto p2 = curve[2];
                Ponto p3 = curve[3];
                Ponto p = getBezierPoint(v, p0, p1, p2, p3);
                points.push_back(p);
            }
        }
        for (int u = 0; u < tesselation; u++) {
            int indexFIRST = u * (tesselation+1);
            int indexNEXT = (u + 1) * (tesselation+1);
            for (int i = 0; i < tesselation; i++) {
                Ponto upNow = points[indexFIRST + i];
                Ponto botNow = points[indexFIRST+ i + 1];
                Ponto upNext = points[indexNEXT + i];
                Ponto botNext = points[indexNEXT + i + 1];

                addTriangle(&vectorRes,upNext,upNow,botNow);
                addTriangle(&vectorRes,botNow,botNext,upNext);

            }
        }
    }
    return vectorRes;
}






vector<Ponto> readFilePatch (int tesselation ,string filename){
    ifstream file(filename);
    string str;
    getline(file,str);
    vector<vector<int>> patches;
    int nrPatches = stoi(str);
    for (int i = 0; i < nrPatches;i++) {
        vector<int> vector;
        getline(file, str);
        string token;
        stringstream ss (str);
        while (getline(ss,token,',')){
            int number = stoi(token);
            vector.push_back(number);
        }
        patches.push_back(vector);
    }

    getline(file,str);
    int nrPoints = stoi(str);
    vector<Ponto> vectorPontos;
    for (int i = 0; i < nrPoints;i++) {
        getline(file, str);
        string token;
        stringstream ss (str);
        vector<float> ponto;
        while (getline(ss,token,',')){
            float number = stof(token);
            ponto.push_back(number);
        }
        Ponto p(ponto[0],ponto[1],ponto[2]);
        vectorPontos.push_back(p);
    }
    printf("Vector pontos size %zu\n",vectorPontos.size());
    return getPointsBezier(tesselation,patches,vectorPontos);
}


//int main (int argc, char **argv){
//    readFilePatch(argv[1]);
//}


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
            printf("\nTorus (requires radius_in, radius_out, slices and stacks)\n");
            printf("Example ->./generator cylinder [radius_in] [radius_out] [slices] [stacks] [filename]\n");
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
            //list<float> *list = getPointsSphere(radius,slices,stacks);
            vector<Ponto> vector = getPointsSphere(radius,slices,stacks);

            char *filename = argv[5];
            writeFile(vector,filename);
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
            //list<float> *list = getPointsBox(length,divisions);
            vector<Ponto> vector = getPointsBox(length,divisions);

            char *filename = argv[4];
            writeFile(vector,filename);
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
            //list<float> *list = getPointsCone(radius,height,slices,stacks);
            vector<Ponto> vector = getPointsCone(radius,height,slices,stacks);
            char *filename = argv[6];
            writeFile(vector,filename);
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
            //list<float> *list = getPointsPlane(length,divisions);
            vector<Ponto> vector = getPointsPlane(length,divisions);
            char *filename = argv[4];
            writeFile(vector,filename);
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
            //list<float> *list = getPointsCylinder(radius,height,slices,stacks);
            vector<Ponto> vector = getPointsCylinder(radius,height,slices,stacks);
            char *filename = argv[6];
            writeFile(vector,filename);
            printf("%s gerado\n",filename);

        }
        catch (std::exception& ia){
            printErro("Argumentos inválidos");
        }
    }
    else if (!strcmp(primitive,"torus")){
        try {
            if (argc != 7){
                printErro("nº de argumentos inválido");
                return 0;
            }
            float radiusIn = std::stof(argv[2]);
            float radiusOut = std::stof(argv[3]);
            int slices= std::stoi(argv[4]);
            int stacks= std::stoi(argv[5]);
           // list<float> *list = getPointsTorus(radiusIn,radiusOut,slices,stacks);
            vector<Ponto> vector = getPointsTorus(radiusIn,radiusOut,slices,stacks);
            char *filename = argv[6];
            writeFile(vector,filename);
            printf("%s gerado\n",filename);

        }
        catch (std::exception& ia){
            printErro("Argumentos inválidos");
        }
    }
    else if (!strcmp(primitive,"bezier")){
        try {
            if (argc != 5){
                printErro("nº de argumentos inválido");
                return 0;
            }
            char* filePatch = argv[2];
            printf("%s\n",argv[4]);
            int tesselation = std::stoi(argv[3]);
            //list<float> *list = readFilePatch(tesselation,filePatch);
            vector<Ponto> vector= readFilePatch(tesselation,filePatch);
            char *filename = argv[4];
            writeFile(vector,filename);
            printf("%s gerado\n",filename);

        }
        catch (std::exception& ia){
            printErro("Argumentos inválidos");
        }
    }
    else printErro("Argumentos inválidos");
}