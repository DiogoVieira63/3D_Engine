//
// Created by diogo on 12/03/22.
//
#include <list>
#include "../tinyxml2.h"
#include "Ponto.h"
#include "Transform.h"
#include "Translation.h"
#include "Scale.h"
#include "Rotation.h"
#include "Color.h"
#include "Model.h"
#include "Group.h"
#include <iostream>

#include <GL/glut.h>

#define _USE_MATH_DEFINES

#include <math.h>
#include <cstdio>
#include <fstream>
#include <vector>
#include <map>


using namespace std;
using namespace tinyxml2;


static float posX, posY, posZ;
static float lookAtX, lookAtY, lookAtZ;
static float upX = 0, upY = 1, upZ = 0;
static float fov = 60, near = 1, far = 1000;

static int drawMode = 0;


static float radius, alpha, beta,radiusMax;


map<string, vector<Ponto>> mapFilesPontos;


Group *groupMain;


void drawGroup(Group *g) {
    glPushMatrix();
    if (g->cor.R != -1) glColor3f(g->cor.R, g->cor.G, g->cor.B);

    for (auto t: (*g).transform) {
        t->doAction();
    }
    if(!(*g).isRandom) {
        for (auto m: (*g).models) {
            glBegin(GL_TRIANGLES);
            for (auto p: (*m).pontos) {
                glVertex3f(p.x, p.y, p.z);
            }
            glEnd();

        }
    }

    if (!(*g).subGroups.empty())printf("SIZE - %zu\n", (*g).subGroups.size());
    for (auto subg: (*g).subGroups) {
        drawGroup(subg);
    }
    glPopMatrix();

}


void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(radius * cos(beta) * sin(alpha), radius * sin(beta), radius * cos(beta) * cos(alpha),
              lookAtX, lookAtY, lookAtZ,
              upX, upY, upZ);


    if (drawMode == 0) glPolygonMode(GL_FRONT, GL_FILL);
    if (drawMode == 1) glPolygonMode(GL_FRONT, GL_LINE);

/*
    glBegin(GL_LINES);
    // X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f( 100.0f, 0.0f, 0.0f);
    // Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    // Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
*/


    glPushMatrix();
    drawGroup(groupMain);
    glPopMatrix();
    glutSwapBuffers();
}


#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if ((a_eResult) != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif


float randomMax(float max) {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX / max);
}


void queryAttrib(XMLElement *element, string query, float *res) {
    XMLError error = element->QueryFloatAttribute(query.c_str(), res);
    if (error != XML_SUCCESS) {
        query += 'R';
        element->QueryFloatAttribute(query.c_str(), res);
        *res = randomMax(*res);
    }
}


void readTransform(XMLElement *pElement, Group *grupo) {
    XMLElement *pElement3;
    if (pElement != nullptr) {
        pElement3 = pElement->FirstChildElement();
        while (pElement3 != nullptr) {
            const char *transform = pElement3->Name();
            if (!strcmp(transform, "rotate")) {
                float x, y, z, angle;
                queryAttrib(pElement3, "x", &x);
                queryAttrib(pElement3, "y", &y);
                queryAttrib(pElement3, "z", &z);
                queryAttrib(pElement3, "angle", &angle);

                Rotation *rotate = new Rotation(angle, x, y, z);
                (*grupo).addRotation(rotate);
            }

            if (!strcmp(transform, "translate")) {
                float x, y, z;
                queryAttrib(pElement3, "x", &x);
                queryAttrib(pElement3, "y", &y);
                queryAttrib(pElement3, "z", &z);
                Translation *translate = new Translation(x, y, z);
                (*grupo).addTranslation(translate);
            }
            if (!strcmp(transform, "scale")) {
                float x, y, z;
                queryAttrib(pElement3, "x", &x);
                queryAttrib(pElement3, "y", &y);
                queryAttrib(pElement3, "z", &z);
                Scale *scale = new Scale(x, y, z);
                (*grupo).addScale(scale);
            }
            pElement3 = pElement3->NextSiblingElement();
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

            pElement3 = pElement3->NextSiblingElement("model");
        }
    }
/*
    pElement2 = pElement->FirstChildElement("random");
    int units = 1;
    if (pElement2 != nullptr) {
        pElement2->QueryIntAttribute("units", &units);
        pElement2 = pElement2->FirstChildElement("transform");
    } else
        pElement2 = pElement->FirstChildElement("transform");

 */
    pElement2 = pElement->FirstChildElement("transform");
    if (pElement2!= nullptr){
        readTransform(pElement2,group);
    }

    pElement2 = pElement->FirstChildElement("random");
    if (pElement2 != nullptr) {
        int units;
        pElement2->QueryIntAttribute("units", &units);
        pElement2 = pElement2->FirstChildElement("transform");

        (*group).isRandom = true;
        for(int i = 0; i < units;i++) {
            auto grupoAdd = new Group();
            (*grupoAdd).models = (*group).models;
            (*grupoAdd).cor = (*group).cor;
            readTransform(pElement2, grupoAdd);
            (*group).addSubGroup(grupoAdd);
        }

    }

    /*
    if (pElement2 != nullptr) {
        for (int i = 0; i < units; i++) {
            auto *grupoAdd = group;
            if (units > 1) {
                grupoAdd = new Group();
                (*grupoAdd).models = (*group).models;
                (*grupoAdd).cor = (*group).cor;
            }

            pElement3 = pElement2->FirstChildElement();
            while (pElement3 != nullptr) {
                const char *transform = pElement3->Name();
                if (!strcmp(transform, "rotate")) {
                    float x, y, z, angle;
                    queryAttrib(pElement3, "x", &x);
                    queryAttrib(pElement3, "y", &y);
                    queryAttrib(pElement3, "z", &z);
                    queryAttrib(pElement3, "angle", &angle);

                    Rotation *rotate = new Rotation(angle, x, y, z);
                    (*grupoAdd).addRotation(rotate);
                }

                if (!strcmp(transform, "translate")) {
                    float x, y, z;
                    queryAttrib(pElement3, "x", &x);
                    queryAttrib(pElement3, "y", &y);
                    queryAttrib(pElement3, "z", &z);
                    Translation *translate = new Translation(x, y, z);
                    (*grupoAdd).addTranslation(translate);
                }

                if (!strcmp(transform, "scale")) {
                    float x, y, z;
                    queryAttrib(pElement3, "x", &x);
                    queryAttrib(pElement3, "y", &y);
                    queryAttrib(pElement3, "z", &z);
                    Scale *scale = new Scale(x, y, z);
                    (*grupoAdd).addScale(scale);
                }
                pElement3 = pElement3->NextSiblingElement();
            }
            if (units > 1)
                (*group).addSubGroup(grupoAdd);
        }
    }
     */

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
    printf("BETA %f\n", beta);
    if (beta >= M_PI / 2)beta -= M_PI;
    if (beta <= -M_PI / 2)beta += M_PI;
    radius = posY / sin(beta);
}


int readCamera(XMLElement *pElement) {
    XMLError error;
    XMLElement *pElement2 = pElement->FirstChildElement("position");

    if (pElement2 == nullptr) return XML_ERROR_PARSING_ELEMENT;
    error = pElement2->QueryFloatAttribute("x", &posX);
    XMLCheckResult(error);
    error = pElement2->QueryFloatAttribute("y", &posY);
    XMLCheckResult(error);
    error = pElement2->QueryFloatAttribute("z", &posZ);
    XMLCheckResult(error);
    error = pElement2->QueryFloatAttribute("rMax", &radiusMax);


    if(error != XML_SUCCESS){
        radiusMax= 500;
    }

    convertToSpherical();

    pElement2 = pElement->FirstChildElement("lookAt");
    if (pElement2 == nullptr) return XML_ERROR_PARSING_ELEMENT;

    error = pElement2->QueryFloatAttribute("x", &lookAtX);
    XMLCheckResult(error);
    error = pElement2->QueryFloatAttribute("y", &lookAtY);
    XMLCheckResult(error);
    error = pElement2->QueryFloatAttribute("z", &lookAtZ);
    XMLCheckResult(error);


    pElement2 = pElement->FirstChildElement("up");
    if (pElement2 != nullptr) {
        error = pElement2->QueryFloatAttribute("x", &upX);
        XMLCheckResult(error);
        error = pElement2->QueryFloatAttribute("y", &upY);
        XMLCheckResult(error);
        error = pElement2->QueryFloatAttribute("z", &upZ);
        XMLCheckResult(error);
    }

    pElement2 = pElement->FirstChildElement("projection");
    if (pElement2 != nullptr) {
        error = pElement2->QueryFloatAttribute("fov", &fov);
        XMLCheckResult(error);
        error = pElement2->QueryFloatAttribute("near", &near);
        XMLCheckResult(error);
        error = pElement2->QueryFloatAttribute("far", &far);
        XMLCheckResult(error);
    }
    return 1;
}

int readXml(const char *filename) {
    XMLDocument xmlDoc;
    xmlDoc.LoadFile(filename);
    XMLNode *pRoot = xmlDoc.FirstChild();
    XMLError error;

    XMLElement *pElement = pRoot->FirstChildElement("camera");

    readCamera(pElement);

    pElement = pRoot->FirstChildElement("group");

    groupMain = readGroup(pElement);

    return 1;

}


void readFile(Model *m) {
    string filename = (*m).filename;
    auto par = mapFilesPontos.find(filename);
    if (!par->second.empty()) {
        (*m).pontos = par->second;
        return;
    }
    ifstream file(filename);
    int nrVertices;
    file >> nrVertices;
    par->second.reserve(nrVertices);
    for (int i = 0; i < nrVertices; i++) {
        float x, y, z;
        file >> x;
        file >> y;
        file >> z;
        Ponto p(x, y, z);
        par->second.push_back(p);
    }
    (*m).pontos = par->second;
}

void readModels(Group *g) {

    for (auto m: (*g).models)
        readFile(m);

    for (auto g1: g->subGroups)
        readModels(g1);

}

void processKeys(unsigned char c, int xx, int yy) {

    if (c == 'p' || c == 'P')drawMode = 1 - drawMode;
    if (c == '1')radius += 2;
    if (c == '2')radius -= 2;

    if (radius <= 0) {
        radius = 1;
    }
    if(radius >= radiusMax) radius = radiusMax;


    glutPostRedisplay();

}


void processSpecialKeys(int key, int xx, int yy) {

    if (key == GLUT_KEY_UP) beta += 0.1;
    if (key == GLUT_KEY_DOWN) beta -= 0.1;
    if (key == GLUT_KEY_LEFT) alpha += 0.1;
    if (key == GLUT_KEY_RIGHT) alpha -= 0.1;

    if (beta >= 1.5)beta = 1.5;
    if (beta <= -1.5)beta = -1.5;

    glutPostRedisplay();


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


    if (argc != 2) {
        printf("nº de argumentos inválido\n");
        return 0;
    }

    char *filename = argv[1];

    readXml(filename);

    printf("Size is %lu\n", mapFilesPontos.size());

    //listaModels.resize(mapFilesInt.size());
    srand(63);
    readModels(groupMain);


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

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // enter GLUT's main cycle
    glutMainLoop();

    return 1;
}