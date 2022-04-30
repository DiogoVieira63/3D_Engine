//
// Created by diogo on 12/03/22.
//

#include <GL/glew.h>
#include <GL/glut.h>

#include "../tinyxml2.h"
#include "Ponto.h"
#include "Translation.h"
#include "Scale.h"
#include "Rotation.h"
#include "Color.h"
#include "Model.h"
#include "Group.h"
#include <iostream>



#define _USE_MATH_DEFINES

#include <math.h>
#include <cstdio>
#include <fstream>
#include <vector>
#include <map>
#include <chrono>
#include <ctime>

auto start = chrono::system_clock::now();

float yaw = -90, pitch;

float direction[3];

using namespace std;
using namespace tinyxml2;

float cameraPos[3]={0.0f, 0.0f,  3.0f};
float cameraFront[3]={0.0f, 0.0f, -1.0f};
float cameraUp[3]={0.0f, 1.0f,  0.0f};

static float dX, dY, dZ;
static float posX , posY, posZ;
static float lookAtX, lookAtY, lookAtZ;
static float upX = 0, upY = 1, upZ = 0;
static float fov = 60, near = 1, far = 1000;

float sensibility = 1;



static int drawMode = 0, eixos =0 ;

double timebase,tim = 0;

static float radius, alpha, beta,radiusMax;

map<string, pair<unsigned int,unsigned int>> mapArraysModel;

map<string, vector<Ponto>> mapFilesPontos;
Group *groupMain;


void drawGroup(Group *g) {
    glPushMatrix();
    if (g->cor.R != -1) glColor3f(g->cor.R, g->cor.G, g->cor.B);

    if(!(*g).isRandom) {
        for (auto t: (*g).transform) {
            t->doAction(tim);
        }
        for (auto m: (*g).models) {
            //glBegin(GL_TRIANGLES);


            __glewBindBuffer(GL_ARRAY_BUFFER, m->parArray.first);
            glVertexPointer(3, GL_FLOAT, 0, 0);
            glDrawArrays(GL_TRIANGLES, 0, m->parArray.second);

            //for (auto p: (*m).pontos) {
            //    glVertex3f(p.x, p.y, p.z);
            //}
            //glEnd();

        }
    }

    for (auto subg: (*g).subGroups) {
        drawGroup(subg);
    }
    glPopMatrix();
}

void normalize(float *a) {

    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}

double chekkFPS(){
    double time = glutGet(GLUT_ELAPSED_TIME);

    double elapsed = (time - timebase)/1000;
    int elpasedFloor = floor(elapsed);

    return  elapsed;
}



void cross(float *a, float *b, float *res) {

    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

float k=1;

void renderScene() {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    if (eixos) {
        glBegin(GL_LINES);
        // X axis in red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(radiusMax, 0.0f, 0.0f);
        // Y Axis in Green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, radiusMax, 0.0f);
        // Z Axis in Blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, radiusMax);

        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
    }

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
/*

    posX = radius * cos(beta) * sin(alpha);
    posY = radius * sin(beta);
    posZ = radius * cos(beta) * cos(alpha);
    gluLookAt(posX, posY, posZ,
              lookAtX, lookAtY, lookAtZ,
              upX, upY, upZ);

*/





    gluLookAt(posX, posY, posZ,
              posX + dX, posY + dY, posZ + dZ,
              upX, upY, upZ);




    if (drawMode == 0) glPolygonMode(GL_FRONT, GL_FILL);
    if (drawMode == 1) glPolygonMode(GL_FRONT, GL_LINE);

    drawGroup(groupMain);


    tim = chekkFPS();

    glutSwapBuffers();
    glutPostRedisplay();
}


#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if ((a_eResult) != XML_SUCCESS) { return a_eResult; }
#endif


float randomMax(float max) {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX / max);
}


void queryAttrib(XMLElement *element, string query, float *res) {
    float num = -1;
    element->QueryFloatAttribute(query.c_str(), &num);
    query += 'R';
    float rand;
    XMLError error = element->QueryFloatAttribute(query.c_str(), &rand);

    if (error == XML_SUCCESS) {
        *res = num + randomMax(rand);
    }
    else
        *res = num;
}

void printPonto(Ponto p){
    printf("%f | %f | %f\n",p.x,p.y,p.z);
}


void readTransform(XMLElement *pElement, Group *grupo) {
    XMLElement *pElement2;
    if (pElement != nullptr) {
        pElement2 = pElement->FirstChildElement();
        while (pElement2 != nullptr) {
            const char *transform = pElement2->Name();
            if (!strcmp(transform, "rotate")) {
                float x, y, z, angle = 0;
                queryAttrib(pElement2, "x", &x);
                queryAttrib(pElement2, "y", &y);
                queryAttrib(pElement2, "z", &z);

                float time = -1;
                queryAttrib(pElement2,"time",&time);

                queryAttrib(pElement2, "angle", &angle);
                Rotation *rotate = new Rotation(angle, x, y, z,time);
                (*grupo).addRotation(rotate);
            }

            if (!strcmp(transform, "translate")) {
                float x, y, z;
                queryAttrib(pElement2, "x", &x);
                queryAttrib(pElement2, "y", &y);
                queryAttrib(pElement2, "z", &z);




                float num;
                XMLError error = pElement2->QueryFloatAttribute("time", &num);
                if (error != XML_SUCCESS) num = -1;

                const char *alignStr;
                bool align = false;
                if ((alignStr = pElement2->Attribute("align"))){
                    if (!strcmp(alignStr,"True")) align = true;
                }

                XMLElement *pElement3 = pElement2->FirstChildElement("point");
                vector<Ponto> vector;
                while (pElement3 != nullptr){
                    float x1, y1, z1;
                    queryAttrib(pElement3, "x", &x1);
                    queryAttrib(pElement3, "y", &y1);
                    queryAttrib(pElement3, "z", &z1);
                    Ponto p = Ponto(x1,y1,z1);
                    vector.push_back(p);
                    pElement3 = pElement3->NextSiblingElement();
                }

                Translation *translate = new Translation(x, y, z,vector,num,align);
                (*grupo).addTranslation(translate);
            }
            if (!strcmp(transform, "scale")) {
                float x, y, z;
                queryAttrib(pElement2, "x", &x);
                queryAttrib(pElement2, "y", &y);
                queryAttrib(pElement2, "z", &z);
                Scale *scale = new Scale(x, y, z);
                (*grupo).addScale(scale);
            }
            pElement2 = pElement2->NextSiblingElement();
        }
    }


}


Group *readGroup(XMLElement *pElement) {
    Group *group = new Group();
    XMLElement *pElement2;
    pElement2 = pElement->FirstChildElement("models");
    XMLElement *pElement3;
    if (pElement2 != nullptr) {
        pElement3 = pElement2->FirstChildElement("model");
        const char *file;
        while (pElement3 != nullptr) {
            file = pElement3->Attribute("file");
            Model *m = new Model(file);
            (*group).addModel(m);
            if (mapFilesPontos.find(file) == mapFilesPontos.end()) {
                vector<Ponto> pontos;
                mapFilesPontos.insert(pair<string, vector<Ponto>>(file, pontos));
            }

            if (mapArraysModel.find(file) == mapArraysModel.end()) {
                unsigned  int size = mapArraysModel.size();
                mapArraysModel.insert(pair<string,pair<unsigned int,unsigned int>>(file,pair<unsigned int,unsigned int>(size,-1)));
            }

            pElement3 = pElement3->NextSiblingElement("model");
        }
    }
    pElement2 = pElement->FirstChildElement("transform");
    if (pElement2!= nullptr){
        readTransform(pElement2,group);
    }

    int units;
    XMLError error = pElement->QueryIntAttribute("units", &units);



    if (error == XML_SUCCESS) {
        pElement2 = pElement->FirstChildElement("transform");
        (*group).isRandom = true;
        for(int i = 0; i < units;i++) {
            auto grupoAdd = new Group();
            (*grupoAdd).models = (*group).models;
            (*grupoAdd).cor = (*group).cor;
            readTransform(pElement2, grupoAdd);
            (*group).addSubGroup(grupoAdd);
        }

    }

    pElement2 = pElement->FirstChildElement("color");
    if (pElement2 != nullptr) {
        float r, g, b;
        pElement2->QueryFloatAttribute("r", &r);
        pElement2->QueryFloatAttribute("g", &g);
        pElement2->QueryFloatAttribute("b", &b);
        Color cor(r, g, b);

        (*group).cor = cor;
    }

    pElement2 = pElement->FirstChildElement("group");
    while (pElement2 != nullptr) {
        Group *subGroup = readGroup(pElement2);
        (*group).addSubGroup(subGroup);
        pElement2 = pElement2->NextSiblingElement("group");
    }

    return group;
}


void convertToSpherical() {
    if (posX == 0)posX = 0.0000001;
    if (posY == 0)posY = 0.0000001;
    if (posZ == 0)posZ = 0.0000001;
    alpha = atan(posX / posZ);
    beta = tan((posY * sin(alpha)) / posX);
    if (beta >= M_PI / 2)beta -= M_PI;
    if (beta <= -M_PI / 2)beta += M_PI;
    radius = posY / sin(beta);

    posX = radius * cos(beta) * sin(alpha);
    posY = radius * sin(beta);
    posZ = radius * cos(beta) * cos(alpha);

    float d[3];
    d[0] = lookAtX - posX;
    d[1] = lookAtY - posY;
    d[2] = lookAtZ - posZ;

    beta *= -1;
    alpha+=M_PI;

    normalize(d);
    dX=d[0];
    dY=d[1];
    dZ=d[2];


}


int readCamera(XMLElement *pElement) {
    XMLError error;
    XMLElement *pElement2 = pElement->FirstChildElement("position");

    if (pElement2 == nullptr) {
        throw ("Grupo camara não contém grupo obrigatório: position");
    }
    error = pElement2->QueryFloatAttribute("x", &posX);
    XMLCheckResult(error);
    error = pElement2->QueryFloatAttribute("y", &posY);
    XMLCheckResult(error);
    error = pElement2->QueryFloatAttribute("z", &posZ);
    XMLCheckResult(error);
    error = pElement2->QueryFloatAttribute("rMax", &radiusMax);

    if (error != XML_SUCCESS)
        radiusMax = 1000000;
    convertToSpherical();



    pElement2 = pElement->FirstChildElement("lookAt");
    if (pElement2 == nullptr) {
        throw string ("Grupo camara não contém grupo obrigatório: lookAt");
    }
    error = pElement2->QueryFloatAttribute("x", &lookAtX);
    XMLCheckResult(error);
    error = pElement2->QueryFloatAttribute("y", &lookAtY);
    XMLCheckResult(error);
    error = pElement2->QueryFloatAttribute("z", &lookAtZ);
    XMLCheckResult(error);



    pElement2 = pElement->FirstChildElement("up");
    if (pElement2 != nullptr) {
        pElement2->QueryFloatAttribute("x", &upX);
        pElement2->QueryFloatAttribute("y", &upY);
        pElement2->QueryFloatAttribute("z", &upZ);
    }

    pElement2 = pElement->FirstChildElement("projection");
    if (pElement2 != nullptr) {
        pElement2->QueryFloatAttribute("fov", &fov);
        pElement2->QueryFloatAttribute("near", &near);
        pElement2->QueryFloatAttribute("far", &far);
    }



    return 0;
}

int readXml(const char *filename) {
    XMLDocument xmlDoc;
    XMLError erro = xmlDoc.LoadFile(filename);
    if (erro != XML_SUCCESS){
        string s = "Ficheiro '" + string (filename) + "': não existe ou erro ao ler.";
        throw string (s);
    }
    XMLNode *pRoot = xmlDoc.FirstChild();

    XMLElement *pElement = pRoot->FirstChildElement("camera");
    readCamera(pElement);

    pElement = pRoot->FirstChildElement("group");
    groupMain = readGroup(pElement);

    return 0;
}


int readFile(Model *m) {
    string filename = (*m).filename;
    auto par = mapFilesPontos.find(filename);
    auto parArray = mapArraysModel.find(filename);
    if (parArray->second.second != -1){
        (*m).parArray = parArray->second;
        return 0;
    }

    if (!par->second.empty()) {
        (*m).pontos = par->second;
        return 0;
    }
    ifstream file(filename);
    if (file.fail()){
        string s = "Ficheiro model '" + filename + "' não existe.";
        throw (s);
    }
    int nrVertices;
    file >> nrVertices;
    par->second.reserve(nrVertices);
    vector<float> vectorPoints;
    for (int i = 0; i < nrVertices; i++) {
        float x, y, z;
        file >> x;
        file >> y;
        file >> z;
        Ponto p(x, y, z);
        par->second.push_back(p);

        vectorPoints.push_back(x);
        vectorPoints.push_back(y);
        vectorPoints.push_back(z);
    }

    glGenBuffers(1, &parArray->second.first);

    glBindBuffer(GL_ARRAY_BUFFER, parArray->second.first);
    glBufferData(
            GL_ARRAY_BUFFER, // tipo do buffer, só é relevante na altura do desenho
            sizeof(float) * vectorPoints.size(), // tamanho do vector em bytes
            vectorPoints.data(), // os dados do array associado ao vector
            GL_STATIC_DRAW); // indicativo da utilização (estático e para desenho)
    unsigned int verticeCount = vectorPoints.size()/3;

    parArray->second.second = verticeCount;
    (*m).parArray = parArray->second;
    (*m).pontos = par->second;
    return 0;
}

void readModels(Group *g) {

    for (auto m: (*g).models)
        readFile(m);

    for (auto g1: g->subGroups)
        readModels(g1);

}

void processKeys(unsigned char c, int xx, int yy) {

    if (c == 'p' || c == 'P')drawMode = 1 - drawMode;
    if (c == 'e' || c == 'E')eixos = 1 - eixos;

    if (c =='1' && sensibility >= 0.2)sensibility-=0.1;
    if (c =='2'&& sensibility <= 1.8)sensibility+=0.1;


    /*

    if (c == '1')radius += 2;
    if (c == '2')radius -= 2;

    if (c == '3')k=1;
    if (c == '4')k=-1;
    if (c == '3' || c == '4') {
        posX += (dX * k);
        posY += (dY * k);
        posZ += (dZ * k);
    }
    if (radius <= 0) radius = 1;

    if(radius >= radiusMax) radius = radiusMax;
     */

    glutPostRedisplay();
}



void processSpecialKeys(int key, int xx, int yy) {



    if (key == GLUT_KEY_UP) beta += 0.1;
    if (key == GLUT_KEY_DOWN) beta -= 0.1;
    if (key == GLUT_KEY_LEFT) alpha += 0.1;
    if (key == GLUT_KEY_RIGHT) alpha -= 0.1;

    if (beta >= 1.5)beta = 1.5;
    if (beta <= -1.5)beta = -1.5;

    dX = radius * cos(beta) * sin(alpha);
    dY = radius * sin(beta);
    dZ = radius * cos(beta) * cos(alpha);

    float d[3]={dX,dY,dZ};
    normalize(d);

    dX=d[0];
    dY=d[1];
    dZ=d[2];
    //lookAtX += dX;
    //lookAtY += dY;
    //lookAtZ += dZ;

    //posX+=dX*k;
    //posY+=dY*k;
    //posZ+=dZ*k;



    glutPostRedisplay();
}


int startX, startY, tracking = 0;

float lastX = 400, lastY = 300;
float initialPX,initialPY,initialPZ;


void processMouseButtons(int button, int state, int xx, int yy)
{
    if (state == GLUT_DOWN)  {
        startX = xx;
        startY = yy;
        initialPX = posX;
        initialPY = posY;
        initialPZ = posZ;


        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else
            tracking = 0;
    }
    else if (state == GLUT_UP) {
        if (tracking == 1) {

            float alphaAux = alpha *180/M_PI + sensibility * (xx - startX);
            alpha = alphaAux *M_PI/180;

            float betaAux = beta*180/M_PI + sensibility * (yy - startY);
            beta = betaAux *M_PI/180;

            //alpha += (xx - startX);
            //beta += (yy - startY);
        }
        else if (tracking == 2) {

            //k -= (yy - startY)*sensibility;

            //r -= yy - startY;
            if (k < -2) k= -2;
            if (k > 2) k = 2;
        }
        tracking = 0;
    }
}


void processMouseMotion(int xx, int yy)
{
    float deltaX, deltaY;
    float alphaAux, betaAux;
    int rAux;

    if (!tracking)
        return;

    deltaX = sensibility * (xx - startX);
    deltaY = sensibility * (yy - startY);


    //deltaX *= 0.5;
    //deltaY *= 0.5;

    if (tracking == 1) {

        alphaAux = alpha*180/M_PI + deltaX;
        betaAux = beta*180/M_PI +   deltaY;

        if (betaAux > 85.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;

        dX = radius * cos(betaAux* 3.14 / 180.0) * sin(alphaAux* 3.14 / 180.0);
        dY = radius * sin(betaAux* 3.14 / 180.0);
        dZ = radius * cos(betaAux* 3.14 / 180.0) * cos(alphaAux* 3.14 / 180.0);


        float d[3]={dX,dY,dZ};
        normalize(d);

        dX=d[0];
        dY=d[1];
        dZ=d[2];

    }
    else if (tracking == 2){

        int kAux;
        kAux = sensibility * (k - deltaY);




        float posXaux = initialPX,posYaux = initialPY,posZaux = initialPZ;
        posXaux += (dX * kAux);
        posYaux += (dY * kAux);
        posZaux += (dZ * kAux);






        posX = posXaux < radiusMax ? posXaux : posX;
        posY = posYaux < radiusMax ? posYaux : posY;
        posZ = posZaux < radiusMax ? posZaux : posZ;



        if (rAux < 3)
            rAux = 3;
    }


    //camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    //camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    //camY = rAux *							    sin(betaAux * 3.14 / 180.0);
}




void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(fov, ratio, near, far);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}



int main(int argc, char **argv) {

    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG@DI-UM");

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    // Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);
    glewInit();
    glEnableClientState(GL_VERTEX_ARRAY);



    if (argc != 2) {
        printf("nº de argumentos inválido\n");
        return 0;
    }

    char *filename = argv[1];

    try {
        srand(63);
        readXml(filename);
        readModels(groupMain);

    }
    catch (string s){
        printf("%s\n",s.c_str());
        return 1;
    }



    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    timebase = glutGet(GLUT_ELAPSED_TIME);

    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}