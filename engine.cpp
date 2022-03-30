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
#include <vector>

#include <stack>

using namespace std;
using namespace tinyxml2;


static float posX, posY,posZ;
static float lookAtX,lookAtY,lookAtZ;
static float upX=0, upY=1,upZ=0;
static float fov = 60,near=1, far=1000;


static float radius, alpha, beta;

class Ponto{
    public:
    float x;
    float y;
    float z;
    Ponto(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
};


class Transform{
    public:
    float x;
    float y;
    float z;
    Transform(float x, float y,float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    virtual void doAction() = 0;
};


class Translation : public Transform{
public:
    Translation(float x, float y, float z) : Transform(x,y,z){}

    void doAction() {
        glTranslatef(x,y,z);
        printf("T,%f|%f|%f\n",x,y,z);
    }
};

class Scale : public Transform{
public:
    Scale(float x, float y, float z) : Transform(x,y,z){}
    void doAction() {
        glScalef(x,y,z);
        printf("S,%f|%f|%f\n",x,y,z);
    }
};

class Rotation: public Transform{
public:
    float angle;
    Rotation(float angle, float x,float y, float z) : Transform(x,y,z){
        this->angle= angle;
    }

    void doAction() {
        glRotatef(angle,x,y,z);
        printf("R,%f|%f|%f|%f\n",angle,x,y,z);

    }
};

class Color{
public:
    float R;
    float G;
    float B;
    Color(){
        R=1;
        G=1;
        B=1;
    }
    Color(float r,float g,float b){
        R = r/255;
        G = g/255;
        B = b/255;
    }
};


class Model{
    public:
    vector<Ponto> pontos;
    string filename;


    Model(const char*file ){
        filename = string(file);
    }

    void addPonto(Ponto p){
        pontos.push_back(p);
    }

    void setSize(int n){
        pontos.clear();
        pontos.reserve(n);
    }
};

class Group{
    public:
    vector<Transform*> transform;
    vector<Model*> models;
    vector<Group*> subGroups;
    Color cor;

    void addModel(Model *m){
        models.push_back(m);
    }



};


list<char*>files;


vector<Model>listaModels;

Group *groupMain;



void drawGroup (Group *g){
    glColor3f(g->cor.R,g->cor.G,g->cor.B);
    printf("COLOR:%f|%f|%f",g->cor.R,g->cor.G,g->cor.B);

    for(auto t : (*g).transform){
        t->doAction();
    }
    printf("S %zu\n",g->models.size());
    for (auto m : (*g).models)
    {
        glBegin(GL_TRIANGLES);
        for (auto p : (*m).pontos)
        {
            glVertex3f(p.x,p.y,p.z);
        }
        glEnd();

    }
    for (auto subg : (*g).subGroups){
        printf("SUB\n");
        glPushMatrix();
        drawGroup(subg);
        glPopMatrix();
    }
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


    glPolygonMode(GL_FRONT, GL_FILL);
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

void convertToSpherical(){
    if (posX==0)posX=0.0000001;
    if (posY==0)posY=0.0000001;
    if (posZ==0)posZ=0.0000001;
    alpha = atan(posX/posZ) ;
    beta = tan((posY*sin(alpha))/posX);
    radius = posY/sin(beta);
}


Group* readGroup(XMLElement *pElement){


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
            pElement3 = pElement3->NextSiblingElement("model");

            printf("%s\n",file);
        }
    }

    pElement2 = pElement->FirstChildElement("color");
    if (pElement2 != nullptr){
        float r, g, b;
        pElement2->QueryFloatAttribute("r", &r);
        pElement2->QueryFloatAttribute("g", &g);
        pElement2->QueryFloatAttribute("b", &b);
        Color cor(r,g,b);

        (*group).cor = cor;
    }


    pElement2 = pElement->FirstChildElement("transform");
    if (pElement2 != nullptr) {
        pElement3 = pElement2->FirstChildElement();
        while(pElement3 != nullptr) {
            const char *transform = pElement3->Name();
            printf("%s\n",transform);
            if (!strcmp(transform, "rotate")) {
                float x, y, z, angle;
                pElement3->QueryFloatAttribute("x", &x);
                pElement3->QueryFloatAttribute("y", &y);
                pElement3->QueryFloatAttribute("z", &z);
                pElement3->QueryFloatAttribute("angle", &angle);
                Rotation *rotate = new Rotation(angle, x, y, z);
                (*group).transform.push_back(rotate);
            }


            if (!strcmp(transform, "translate")) {
                float x, y, z;
                pElement3->QueryFloatAttribute("x", &x);
                pElement3->QueryFloatAttribute("y", &y);
                pElement3->QueryFloatAttribute("z", &z);
                printf("%f|%f|%f\n",x,y,z);

                Translation *translate = new Translation(x, y, z);
                (*group).transform.push_back(translate);
            }

            if (!strcmp(transform, "scale")) {
                float x, y, z;
                pElement3->QueryFloatAttribute("x", &x);
                pElement3->QueryFloatAttribute("y", &y);
                pElement3->QueryFloatAttribute("z", &z);
                Scale *scale = new Scale(x, y, z);
                (*group).transform.push_back(scale);
            }
            pElement3 = pElement3->NextSiblingElement();
        }
    }


    pElement2 = pElement->FirstChildElement("group");

    while (pElement2 != nullptr){
        printf("GROUP\n");
        Group* subGroup = readGroup(pElement2);
        (*group).subGroups.push_back(subGroup);
        pElement2 = pElement2->NextSiblingElement("group");
    }


    return group;

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

    groupMain = readGroup(pElement);

    return 1;

    //XMLCheckResult(eResult);
}
void addVerticeToList(list<float>&list, float x,float y, float z){
    list.push_back(x);
    list.push_back(y);
    list.push_back(z);

}

void readFile(Model *m){
    string filename = (*m).filename;
    ifstream file(filename);
    int nrVertices;
    file>>nrVertices;
    (*m).setSize(nrVertices);
    printf("->%d %s\n",nrVertices,filename.c_str());
    for (int i = 0; i < nrVertices;i++){
        // Output the text from the file
        float x,y,z;
        file >> x;
        file >> y;
        file >> z;
        Ponto p (x,y,z);
        (*m).addPonto(p);
        //addVerticeToList(list,x,y,z);
    }
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

void readModels(Group *g){
    printf("Msize is %zu\n", (*g).models.size());

    for (auto m : (*g).models) {
        printf("HERE\n");
        //printf("SIZE %zu | %s \n",(*m).pontos.size(),(*m).filename.c_str());
        readFile(m);
    }

    printf("size is %zu\n", (*g).subGroups.size());


    for(auto g1 : g->subGroups){
        printf("DONE\n");
        readModels(g1);
    }
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



    readModels(groupMain);
/*
        for (auto i = g.models.cbegin(); i != g.models.cend(); i++) {
            printf("HERE\n");
            Model *m = *i;
            //printf("SIZE %zu | %s \n",(*m).pontos.size(),(*m).filename.c_str());
            readFile(m);
        }


*/

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