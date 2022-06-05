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
#include "../Utils/utils.h"

void writePontos(ofstream *file, vector<Ponto> vect){
    int nrVertices = vect.size();
    *file << nrVertices << '\n';
    for (Ponto p : vect){
        *file << p.x << ' ';
        *file << p.y << ' ';
        *file << p.z << ' ';
        *file << '\n';
    }
}

void writeFile(vector<Ponto> vect,vector<Ponto> normal,vector<Ponto> textures, char *filename){
    ofstream file;
    //some
    file.open(filename);
    writePontos(&file,vect);
    writePontos(&file,normal);
    writePontos(&file,textures);
    file.close();

}

void writeFile(vector<Ponto> vect, char *filename){
    ofstream file;
    //some
    file.open(filename);
    writePontos(&file,vect);

    file.close();

}


void addTriangle(vector<Ponto> *vector,Ponto p0,Ponto p1,Ponto p2){
    vector->push_back(p0);
    vector->push_back(p1);
    vector->push_back(p2);

}


void getPointsPlane (float length,int divisions,char* filename){

    float inicial_posX = - length/2;
    float inicial_posZ = inicial_posX;
    float part = length/(float )divisions;

    float partText = 1.0f/(float )divisions;

    vector<Ponto> vector, textures, normal;


    for (int i = 0;i < divisions;i++){
        float posX = inicial_posX + (float) i * part;
        for (int j = 0;j < divisions;j++){
            float posZ = inicial_posZ + (float) j * part;

            Ponto upNow = Ponto(posX, 0, posZ);
            Ponto upNext = Ponto(posX + part, 0, posZ);
            Ponto botNext = Ponto(posX + part, 0, posZ + part);
            Ponto botNow = Ponto(posX, 0, posZ + part);

            Ponto up = Ponto(0,1,0);

            Ponto down= Ponto(0,-1,0);

            //Ponto TupNow = Ponto(  (float)i * partText,(float)j * partText, 0);
            //Ponto TupNext = Ponto( (float)(i+1) * (partText),(float) j * partText, 0);
            //Ponto TbotNext = Ponto((float)(i+1) * (partText),(float)  (j+1) * (partText), 0);
            //Ponto TbotNow = Ponto( (float)i * partText,(float)(j+1) * (partText), 0);

            Ponto TupNow = Ponto(  0,0, 0);
            Ponto TupNext = Ponto( 1,0, 0);
            Ponto TbotNext = Ponto(1,1, 0);
            Ponto TbotNow = Ponto( 0,1, 0);


            addTriangle(&vector,botNext,upNext,upNow);
            addTriangle(&normal,up,up,up);
            addTriangle(&textures,TbotNext,TupNext,TupNow);


            addTriangle(&vector,upNow,upNext,botNext);
            addTriangle(&normal,down,down,down);
            addTriangle(&textures,TupNow,TupNext,TbotNext);


            addTriangle(&vector,upNow,botNow,botNext);
            addTriangle(&normal,up,up,up);
            addTriangle(&textures,TupNow,TbotNow,TbotNext);


            addTriangle(&vector,botNext,botNow,upNow);
            addTriangle(&normal,down,down,down);
            addTriangle(&textures,TbotNext,TbotNow,TupNow);


        }
    }
    writeFile(vector,normal,textures,filename);

}


void getPointsCone(float r, float height, int slices,int stacks,char *filename) {
    float anglePart = 2 * M_PI / slices;
    float heightPart = height/(float)stacks;
    float initialHeight =0;
    vector<Ponto> vector, textures, normal;

    float textV = 1.0f/(float)stacks;
    float textH = 1.0f/(float)slices;


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

            Ponto NupNext =    upNext.getNormal();
            Ponto NupNow =      upNow.getNormal();
            Ponto NdownNow =    downNow.getNormal();
            Ponto NdownNext =   downNext.getNormal();

            Ponto TupNow =      Ponto(i*textH,(j+1) * textV,0);
            Ponto TupNext =     Ponto((i+1)*textH,(j+1) * textV,0);
            Ponto TdownNow =    Ponto(i*textH,j * textV,0);
            Ponto TdownNext =   Ponto((i+1)*textH,j * textV,0);




            addTriangle(&vector,upNext,upNow,downNow);
            addTriangle(&normal,NupNext,NupNow,NdownNow);
            addTriangle(&textures,TupNext,TupNow,TdownNow);



            addTriangle(&vector,downNow,downNext,upNext);
            addTriangle(&normal,NdownNow,NdownNext,NupNext);
            addTriangle(&textures,TdownNow,TdownNext,TupNext);


        }

        Ponto baseNext = Ponto(r * sin(nextAngle), initialHeight, r * cos(nextAngle));
        Ponto baseNow = Ponto(r * sin(angle), initialHeight, r * cos(angle));
        Ponto center = Ponto(0, initialHeight, 0);

        Ponto down = Ponto(0,-1,0);



        Ponto TbaseNext = Ponto((i+1)*textH,0,0);
        Ponto TbaseNow = Ponto(i*textH,0,0);
        Ponto Tcenter = Ponto((i+0.5)*textH,1,0);

        addTriangle(&vector,baseNext,baseNow,center);
        addTriangle(&normal,down,down,down);
        addTriangle(&textures,TbaseNext,TbaseNow,Tcenter);

    }

    writeFile(vector, normal,textures,filename);
}


void getPointsSphere(float radius, int slices,int stacks,char *filename){
    float anglePart = 2 * M_PI / slices;
    float heightPart = M_PI/stacks;
    float initialAngle = -M_PI/2;
    //list<float> *list = new ::list<float>;
    vector<Ponto> vector, textures, normal;

    float textV = 1.0f/(float)stacks;
    float textH = 1.0f/(float)slices;
    printf("%f | %f\n",textV,textH);


    for (int i = 0; i < slices;i++) {
        float angle = ((float) i) * anglePart;
        float nextAngle = angle + anglePart;

        float hNow = textH * (float)(i);
        float hNext = textH * (float)(i+1);

        for (int j = 0; j < stacks; j++) {

            float angleUp =   initialAngle+ heightPart*(float)(j+1);
            float angleDown = initialAngle+ heightPart*(float)(j);


            if(angleUp>=M_PI/2)angleUp=M_PI/2-0.00001;
            if(angleDown<=-M_PI/2)angleDown=-M_PI/2+0.000001;

            Ponto upNow = Ponto(radius* cos(angleUp)*sin(angle),radius*sin(angleUp),radius* cos(angle)* cos(angleUp) );
            Ponto upNext = Ponto(radius* cos(angleUp)*sin(nextAngle),radius* sin(angleUp), radius * cos(nextAngle)* cos(angleUp));
            Ponto downNow = Ponto (radius* cos(angleDown)*sin(angle),radius*sin(angleDown),radius* cos(angle)* cos(angleDown));
            Ponto downNext = Ponto(radius* cos(angleDown)*sin(nextAngle),radius*sin(angleDown), radius * cos(nextAngle)* cos(angleDown));


            Ponto NupNow = upNow.getNormal();
            Ponto NupNext = upNext.getNormal();
            Ponto NdownNow = downNow.getNormal();
            Ponto NdownNext = downNext.getNormal();



            float vUp = 1 -textV * (float)(j+1);
            float vDown = 1 - textV * (float)(j);



            Ponto TupNow = Ponto(hNow,vUp,0);
            Ponto TupNext = Ponto(hNext,vUp,0);
            Ponto TdownNow = Ponto(hNow,vDown,0);
            Ponto TdownNext = Ponto(hNext,vDown,0);



            addTriangle(&vector,upNow,downNow,downNext);
            addTriangle(&normal,NupNow,NdownNow,NdownNext);
            addTriangle(&textures,TupNow,TdownNow,TdownNext);


            addTriangle(&vector,upNext,upNow,downNext);
            addTriangle(&normal,NupNext,NupNow,NdownNext);
            addTriangle(&textures,TupNext,TupNow,TdownNext);

        }
    }
    writeFile(vector,normal,textures,filename);
}

void getPointsBox (float length, int divisions,char *filename){
    float part = length/(float)divisions;
    float halfAltura = length/2;
    float initX = - halfAltura;
    float initZ = initX;
    float text = 1.0f/(float )divisions;
    //list<float> *list = new ::list<float>;
    vector<Ponto> vector, normal, texture;

    // Y Constante
    for (int i = 0;i < divisions;i++){
        for (int j = 0;j < divisions;j++){
            float posX = initX + (float )i * part;
            float posZ = initZ + (float )j * part;
            float nextPosX = posX + part;
            float nextPosZ = posZ + part;

            Ponto nextXnextZ = Ponto(nextPosX,halfAltura,nextPosZ);
            Ponto XnextZ = Ponto(posX,halfAltura,nextPosZ);
            Ponto nextXZ = Ponto(nextPosX,halfAltura,posZ);
            Ponto XZ = Ponto(posX,halfAltura,posZ);

            Ponto up = Ponto(0,1,0);
            Ponto down = Ponto(0,-1,0);

            Ponto TnextXnextZ = Ponto(1,1,0);
            Ponto TXnextZ = Ponto(0,1,0);
            Ponto TnextXZ = Ponto(1,0,0);
            Ponto TXZ = Ponto(0,0,0);


            addTriangle(&vector,nextXnextZ,nextXZ,XZ);
            addTriangle(&normal,up,up,up);
            addTriangle(&texture,TnextXnextZ,TnextXZ,TXZ);



            addTriangle(&vector,XZ,XnextZ,nextXnextZ);
            addTriangle(&normal,up,up,up);
            addTriangle(&texture,TXZ,TXnextZ,TnextXnextZ);


            Ponto OnextXnextZ = Ponto(nextPosX,-halfAltura,nextPosZ);
            Ponto OXnextZ = Ponto(posX,-halfAltura,nextPosZ);
            Ponto OnextXZ = Ponto(nextPosX,-halfAltura,posZ);
            Ponto OXZ = Ponto(posX,-halfAltura,posZ);


            addTriangle(&vector,OXZ,OnextXZ,OnextXnextZ);
            addTriangle(&normal,down,down,down);
            addTriangle(&texture,TXZ,TnextXZ,TnextXnextZ);



            addTriangle(&vector,OnextXnextZ,OXnextZ,OXZ);
            addTriangle(&normal,down,down,down);
            addTriangle(&texture,TnextXnextZ,TXnextZ,TXZ);


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



            Ponto TnextYnextZ = Ponto(1,1, 0);
            Ponto TYnextZ =     Ponto(1,0, 0);
            Ponto TnextYZ =     Ponto(0,1, 0);
            Ponto TYZ =         Ponto(0,0, 0);

            Ponto up = Ponto(-1,0,0);
            Ponto down = Ponto(1,0,0);

            addTriangle(&vector,YnextZ,YZ,nextYZ);
            addTriangle(&normal,up,up,up);
            addTriangle(&texture,TYnextZ,TYZ,TnextYZ);


            addTriangle(&vector,YnextZ,nextYZ,nextYnextZ);
            addTriangle(&normal,up,up,up);
            addTriangle(&texture,TYnextZ,TnextYZ,TnextYnextZ);



            Ponto OnextYnextZ = Ponto(-initX, nextPosY, nextPosZ);
            Ponto OYnextZ = Ponto(-initX, posY, nextPosZ);
            Ponto OnextYZ = Ponto(-initX, nextPosY, posZ);
            Ponto OYZ = Ponto(-initX, posY, posZ);


            addTriangle(&vector,OnextYZ,OYZ,OYnextZ);
            addTriangle(&normal,down,down,down);
            addTriangle(&texture,TnextYZ,TYZ,TYnextZ);


            addTriangle(&vector,OnextYnextZ,OnextYZ,OYnextZ);
            addTriangle(&normal,down,down,down);
            addTriangle(&texture,TnextYnextZ,TnextYZ,TYnextZ);


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

            Ponto up =  Ponto(0,0,-1);
            Ponto down =Ponto(0,0,1);

            Ponto TXY =         Ponto(0,0 ,0);
            Ponto TnextXY =     Ponto(1,0 , 0);
            Ponto TXnextY =     Ponto(0,1, 0);
            Ponto TnextXnextY = Ponto(1,1, 0);



            addTriangle(&vector,XnextY,XY,nextXY);
            addTriangle(&normal,up,up,up);
            addTriangle(&texture,TXnextY,TXY,TnextXY);



            addTriangle(&vector,nextXnextY,XnextY,nextXY);
            addTriangle(&normal,up,up,up);
            addTriangle(&texture,TnextXnextY,TXnextY,TnextXY);


            Ponto OnextXnextY = Ponto(nextPosX,nextPosY, -initZ);
            Ponto OXnextY = Ponto(posX,nextPosY, -initZ);
            Ponto OnextXY = Ponto(nextPosX,posY, -initZ);
            Ponto OXY = Ponto(posX,posY, -initZ);

            addTriangle(&vector,OnextXY,OXY,OXnextY);
            addTriangle(&normal,down,down,down);
            addTriangle(&texture,TnextXY,TXY,TXnextY);



            addTriangle(&vector,OnextXY,OXnextY,OnextXnextY);
            addTriangle(&normal,down,down,down);
            addTriangle(&texture,TnextXY,TXnextY,TnextXnextY);



        }
    }
    writeFile(vector,normal,texture,filename);
}

void getPointsCylinder(float r, float height, int slices, int stacks,char *filename) {
    float anglePart = 2 * M_PI / slices;
    float halfHeight = height / 2;
    float heightPart = height/(float)stacks;

    float textH = 1.0f/(float) slices;
    float textV = 1.0f/(float) stacks;

    vector<Ponto> vector, normal, texture;


    for (int i = 0; i < slices; i++) {


        float angle = ((float) i) * anglePart;
        float nextAngle = angle + anglePart;


        Ponto baseNow = Ponto(r * sin(angle), halfHeight, r * cos(angle));
        Ponto baseNext= Ponto(r * sin(nextAngle), halfHeight, r * cos(nextAngle));
        Ponto center = Ponto(0, halfHeight, 0);

        Ponto up = Ponto(0,1,0);
        Ponto down = Ponto(0,-1,0);

        Ponto TbaseNext = Ponto((i+1)*textH,0,0);
        Ponto TbaseNow = Ponto(i*textH,0,0);
        Ponto Tcenter = Ponto((i+0.5)*textH,1,0);



        addTriangle(&vector,center,baseNow,baseNext);
        addTriangle(&normal,up,up,up);
        addTriangle(&texture,Tcenter,TbaseNow,TbaseNext);


        Ponto ObaseNow = Ponto(r * sin(angle), -halfHeight, r * cos(angle));
        Ponto ObaseNext= Ponto(r * sin(nextAngle), -halfHeight, r * cos(nextAngle));
        Ponto Ocenter = Ponto(0, -halfHeight, 0);

        addTriangle(&vector,ObaseNext,ObaseNow,Ocenter);
        addTriangle(&normal,down,down,down);
        addTriangle(&texture,TbaseNext,TbaseNow,Tcenter);



        for (int j = 0; j < stacks; j++) {
            float heightUp = -halfHeight + (heightPart * (float )(j+1));
            float heightDown = -halfHeight + (heightPart * (float )(j));


            Ponto upNow = Ponto(r * sin(angle), heightUp, r * cos(angle));
            Ponto upNext = Ponto(r * sin(nextAngle), heightUp, r * cos(nextAngle));
            Ponto downNow = Ponto(r * sin(angle), heightDown, r * cos(angle));
            Ponto downNext= Ponto(r * sin(nextAngle), heightDown, r * cos(nextAngle));

            Ponto NNow = Ponto(sin(angle), cos(angle),0);
            Ponto NNext = Ponto(sin(nextAngle), cos(nextAngle),0);


            Ponto TupNow =      Ponto(i*textH,(j+1) * textV,0);
            Ponto TupNext =     Ponto((i+1)*textH,(j+1) * textV,0);
            Ponto TdownNow =    Ponto(i*textH,j * textV,0);
            Ponto TdownNext =   Ponto((i+1)*textH,j * textV,0);


            addTriangle(&vector,upNow,downNow,downNext);
            addTriangle(&normal,NNow,NNow,NNext);
            addTriangle(&texture,TupNow,TdownNow,TdownNext);


            addTriangle(&vector,upNext,upNow,downNext);
            addTriangle(&normal,NNext,NNow,NNext);
            addTriangle(&texture,TupNext,TupNow,TdownNext);

        }
    }
    writeFile(vector, normal,texture,filename);
}

void getPointsTorus (float raio_in, float raio_out, int slices, int stacks,char * filename){
    float delta1= 2* M_PI / slices;
    float delta2= 2* M_PI / stacks;
    float phi = 0;
    float theta=0;

    vector<Ponto> vector, textures, normal;

    float partV = 1.0f/(float )stacks;
    float partH = 1.0f/(float )slices;


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


            Ponto up = Ponto(0,0,-1);
            Ponto down = Ponto(0,0,1);

            Ponto TupNow    = Ponto(0,1,0);
            Ponto TupNext   = Ponto(1,1,0);
            Ponto TdownNow  = Ponto(0,0,0);
            Ponto TdownNext = Ponto(1,0,0);



            addTriangle(&vector,p,pd1,pd3);
            addTriangle(&normal,up,up,up);
            addTriangle(&textures,TdownNow,TdownNext,TupNext);

            addTriangle(&vector,pd3,pd2,p);
            addTriangle(&normal,up,up,up);
            addTriangle(&textures,TupNext,TupNow,TdownNow);



            phi = delta2 * (j+1);
        }
        theta = delta1 * (i+1);
    }
    writeFile(vector,normal,textures,filename);
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
        utils::multMatrixVector(m,vector,a);

        pos[i] = powf(t,3) * a[0] + powf(t,2) * a[1] + t * a[2] + a[3];

    }

    Ponto p(pos[0],pos[1],pos[2]);


    return p;


}



void getPointsBezier(int tesselation,vector<vector<int>>patchVector, vector<Ponto> pontos,char *filename){
    float part = 1.0f/(float)tesselation;
    vector <Ponto> vectorRes, normal, texture;
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

                Ponto NupNow = upNow.getNormal();
                Ponto NbotNow =botNow.getNormal();
                Ponto NupNext = upNext.getNormal();
                Ponto NbotNext = botNext.getNormal();



                Ponto TupNow =  Ponto((u)*part,  (i)*part,0);
                Ponto TbotNow = Ponto((u)*part,  (i+1)*part,0);
                Ponto TupNext = Ponto((u+1)*part,(i)*part,0);
                Ponto TbotNext =Ponto((u+1)*part,(i+1)*part,0);



                addTriangle(&vectorRes,botNow,upNow,upNext);
                addTriangle(&normal,NbotNow,NupNow,NupNext);
                addTriangle(&texture,TbotNow,TupNow,TupNext);


                addTriangle(&vectorRes,upNext,botNext,botNow);
                addTriangle(&normal,NupNext,NbotNext,NbotNow);
                addTriangle(&texture,TupNext,TbotNext,TbotNow);



            }
        }
    }

    writeFile(vectorRes,normal,texture,filename);
}


void printErro(const string& str){
    cout << "Erro:" <<str << "\n";
    cout <<"For info -> ./generator help\n";
}



void readFilePatch (int tesselation ,string filePatch, char *filename){
    ifstream file(filePatch);
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
    getPointsBezier(tesselation,patches,vectorPontos,filename);
}





int main(int argc, char **argv){
    if (argc <=2) {
        if(!strcmp(argv[1],"help")){
            printf("Graphical primitives available:\n\n");
            printf("Plane (a square in the XZ plane, centred in the origin, subdivided in both X and Z directions\n");
            printf("Example ->./generator plane [length] [division] [filename]\n");
            printf("\nBox (requires dimension, and the lightNumber of divisions per edge)\n");
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
            char *filename = argv[5];
            getPointsSphere(radius,slices,stacks,filename);
            //writeFile(vector,filename);

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
            char *filename = argv[4];
            getPointsBox(length,divisions,filename);

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
            char *filename = argv[6];
            getPointsCone(radius,height,slices,stacks,filename);
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
            char *filename = argv[4];
            getPointsPlane(length,divisions,filename);
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
            char *filename = argv[6];
            getPointsCylinder(radius,height,slices,stacks,filename);
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
            char *filename = argv[6];
            getPointsTorus(radiusIn,radiusOut,slices,stacks,filename);
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
            char *filename = argv[4];
            readFilePatch(tesselation,filePatch,filename);
            printf("%s gerado\n",filename);

        }
        catch (std::exception& ia){
            printErro("Argumentos inválidos");
        }
    }
    else printErro("Argumentos inválidos");
}