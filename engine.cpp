//
// Created by diogo on 12/03/22.
//
#include <list>
#include "tinyxml2.h"
#include <iostream>

#include <GL/glut.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdio>
#include <fstream>


using namespace std;
using namespace tinyxml2;


static float posX, posY,posZ;
static float lookAtX,lookAtY,lookAtZ;
static float upX=0, upY=1,upZ=0;
static float fov = 60,near=1, far=1000;

static float radius, alpha, beta;


list<char*>files;


list<list<float>>listaPonto;



void drawList (){
    glColor3f(1,1,1);
    glBegin(GL_TRIANGLES);
    list<list<float>>::iterator itr;
    for (itr=listaPonto.begin(); itr != listaPonto.end(); itr++)
    {
        list<float>tl=*itr;
        list<float>::iterator it;
        for (it=tl.begin(); it != tl.end(); it++)
        {
            float x = *it;
            ++it;
            float y = *it;
            ++it;
            float z = *it;
            glVertex3f(x,y,z);
        }
    }
    glEnd();
}



void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(radius*cos(beta)* sin(alpha),radius*sin(beta), radius*cos(beta)* cos(alpha),
              lookAtX,lookAtY,lookAtZ,
              upX,upY,upZ);


    //gluLookAt(2,5,1,0,0,0,0.0f,1.0f,0.0f);


    glPolygonMode(GL_FRONT, GL_LINE);

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




    drawList();

    glutSwapBuffers();
}


#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if ((a_eResult) != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

void convertToSpherical(){
    if (posX==0)posX=0.0000001;
    if (posY==0)posY=0.0000001;
    if (posZ==0)posZ=0.0000001;
    alpha = atan(posX/posZ) ;
    beta = tan((posY*sin(alpha))/posX);
    radius = posY/sin(beta);
}

int readXml(const char* filename){
    XMLDocument xmlDoc;
    xmlDoc.LoadFile(filename);
    XMLNode * pRoot = xmlDoc.FirstChild();
    XMLError error;

    XMLElement * pElement = pRoot->FirstChildElement("camera");
    XMLElement *pElement2 = pElement->FirstChildElement("position");

    if (pElement2 == nullptr) return XML_ERROR_PARSING_ELEMENT;
    error =  pElement2->QueryFloatAttribute("x",&posX);
    XMLCheckResult(error);
    error= pElement2->QueryFloatAttribute("y",&posY);
    XMLCheckResult(error);
    error= pElement2->QueryFloatAttribute("z",&posZ);
    XMLCheckResult(error);

    convertToSpherical();

    pElement2 = pElement->FirstChildElement("lookAt");
    if (pElement2 == nullptr) return XML_ERROR_PARSING_ELEMENT;

    error= pElement2->QueryFloatAttribute("x",&lookAtX);
    XMLCheckResult(error);
    error= pElement2->QueryFloatAttribute("y",&lookAtY);
    XMLCheckResult(error);
    error= pElement2->QueryFloatAttribute("z",&lookAtZ);
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

    pElement = pRoot->FirstChildElement("group");
    if (pElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
    pElement2 = pElement->FirstChildElement("models");
    if (pElement2 == nullptr) return XML_ERROR_PARSING_ELEMENT;
    XMLElement *pElement3 = pElement2->FirstChildElement("model");
    if (pElement3 == nullptr) return XML_ERROR_PARSING_ELEMENT;
    while (pElement3 != nullptr){
        const char* file = pElement3->Attribute("file");
        if(file == nullptr)return 0;
        files.push_back(strdup(file));
        printf("file -> %s\n",file);
        pElement3 = pElement3->NextSiblingElement("model");
    }
    return 1;

    //XMLCheckResult(eResult);
}
void addVerticeToList(list<float>&list, float x,float y, float z){
    list.push_back(x);
    list.push_back(y);
    list.push_back(z);

}

list<float> readFile(char* filename){
    ifstream file(filename);
    list<float> list;
    int nrVertices;
    file>>nrVertices;
    for (int i = 0; i < nrVertices;i++){
        // Output the text from the file
        float x,y,z;
        file >> x;
        file >> y;
        file >> z;
        addVerticeToList(list,x,y,z);
    }
    return list;
}

void processKeys(unsigned char c, int xx, int yy) {

    if (c =='1')radius+=0.5;
    if (c =='2')radius-=0.5;


    glutPostRedisplay();

}


void processSpecialKeys(int key, int xx, int yy) {

    if (key == GLUT_KEY_UP) beta+=0.1;
    if (key == GLUT_KEY_DOWN) beta-=0.1;
    if (key == GLUT_KEY_LEFT) alpha+=0.1;
    if (key == GLUT_KEY_RIGHT) alpha-=0.1;

    if (beta>=1.5)beta=1.5;
    if (beta<=-1.5)beta=-1.5;

    glutPostRedisplay();


}
void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
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
    gluPerspective(fov ,ratio, near ,far);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {


    if (argc != 2){
        printf("nº de argumentos inválido\n");
        return 0;
    }

    char* filename = argv[1];

    if(readXml(filename) == XML_ERROR_PARSING_ELEMENT){
        printf("Erro ao ler %s.\n",filename);
    }


    list<char*>::iterator it;
    for (it=files.begin(); it != files.end(); it++)
    {
        char* file = *it;
        list<float>list = readFile(file);
        listaPonto.push_back(list);
    }




    // init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
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