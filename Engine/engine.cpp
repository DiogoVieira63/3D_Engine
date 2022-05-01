//
// Created by diogo on 12/03/22.
//

#include <GL/glew.h>
#include <GL/glut.h>

#include "../tinyxml2.h"
#include "Model.h"
#include "Group.h"
#include "xmlParser.h"
#include "Camera.h"

#include <cstdio>
#include <vector>
#include <map>

using namespace std;

static int drawMode = 0, eixos =0 ;
double timebase,tim = 0;

map<string, pair<unsigned int,unsigned int>> mapArraysModel;
Group *groupMain;
Camera cam;

void drawGroup(Group *g) {
    glPushMatrix();
    if (g->cor.R != -1) glColor3f(g->cor.R, g->cor.G, g->cor.B);
    if(!(*g).isRandom) {
        for (auto t: (*g).transform) {
            t->doAction(tim);
        }
        for (auto m: (*g).models) {
            __glewBindBuffer(GL_ARRAY_BUFFER, m->parArray.first);
            glVertexPointer(3, GL_FLOAT, 0, 0);
            glDrawArrays(GL_TRIANGLES, 0, m->parArray.second);
        }
    }
    for (auto subg: (*g).subGroups) {
        drawGroup(subg);
    }
    glPopMatrix();
}

double chekkFPS(){
    double time = glutGet(GLUT_ELAPSED_TIME);
    double elapsed = (time - timebase)/1000;
    return  elapsed;
}

void renderScene() {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (eixos) {
        glBegin(GL_LINES);
        // X axis in red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(cam.radiusMax, 0.0f, 0.0f);
        // Y Axis in Green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, cam.radiusMax, 0.0f);
        // Z Axis in Blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, cam.radiusMax);

        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    cam.setCamera();

    if (drawMode == 0) glPolygonMode(GL_FRONT, GL_FILL);
    if (drawMode == 1) glPolygonMode(GL_FRONT, GL_LINE);

    drawGroup(groupMain);

    tim = chekkFPS();

    glutSwapBuffers();
    glutPostRedisplay();
}



void processKeys(unsigned char c, int xx, int yy) {

    if (c == 'p' || c == 'P')drawMode = 1 - drawMode;
    if (c == 'e' || c == 'E')eixos = 1 - eixos;

    glutPostRedisplay();
}



void processSpecialKeys(int key, int xx, int yy) {
    if (key == GLUT_KEY_DOWN && cam.sensibility >= 0.2)cam.sensibility-=0.1;
    if (key == GLUT_KEY_UP&& cam.sensibility <= 1.8) cam.sensibility+=0.1;

    glutPostRedisplay();
}


int startX, startY, tracking = 0;
float initialPX,initialPY,initialPZ;


void processMouseButtons(int button, int state, int xx, int yy){
    if (state == GLUT_DOWN)  {
        startX = xx;
        startY = yy;
        initialPX = cam.pos[0];
        initialPY = cam.pos[1];
        initialPZ = cam.pos[2];
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else
            tracking = 0;
    }
    else if (state == GLUT_UP) {
        if (tracking == 1) {
            cam.updateDirAngles(xx,yy,startX,startY);
        }
        tracking = 0;
    }
}


void processMouseMotion(int xx, int yy){
    float deltaX, deltaY;
    if (!tracking)
        return;

    deltaX = cam.sensibility * (xx - startX);
    deltaY = cam.sensibility * (yy - startY);

    if (tracking == 1) {
        cam.changeDir(deltaX,deltaY);
    }
    else if (tracking == 2){
        cam.moveCamera(deltaY,initialPX,initialPY,initialPZ);
    }
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
    gluPerspective(cam.fov, ratio, cam.near, cam.far);
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
        return 1;
    }
    char *filename = argv[1];

    try {
        srand(63);
        xmlParser parser(mapArraysModel,&cam);
        groupMain = parser.readXml(filename);
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
    return 0;
}