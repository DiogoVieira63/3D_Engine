//
// Created by diogo on 12/03/22.
//

#include <IL/il.h>


#include <GL/glew.h>
#include <GL/glut.h>

#include "../tinyxml2.h"
#include "Model.h"
#include "Group.h"
#include "xmlParser.h"
#include "Camera.h"

#include <cstdio>
#include <chrono>


#include <vector>
#include <map>
using namespace std;

static int drawMode = 0, eixos =0,stop = 0, lighting = 1;
double timebase,tim = 0, timeStop = 0;

Group *groupMain;
Camera cam;
GLuint textures[100];


void drawGroup(Group *g) {
    glPushMatrix();



    if(!(*g).isRandom) {
        for (auto t: (*g).transform) {
            t->doAction(tim);
        }
        for (auto m: (*g).models) {
            m->color.doColor();

            glBindBuffer(GL_ARRAY_BUFFER, m->parArray.first[0]);
            glVertexPointer(3, GL_FLOAT, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, m->parArray.first[1]);
            glNormalPointer(GL_FLOAT,0,0);

            if((*m).parText.first) {
                glBindBuffer(GL_ARRAY_BUFFER, m->parArray.first[2]);
                glTexCoordPointer(2, GL_FLOAT, 0, 0);
                glBindTexture(GL_TEXTURE_2D, textures[(*m).parText.second]);
            }
            else{
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            if(lighting) glEnable(GL_LIGHTING);
            glDrawArrays(GL_TRIANGLES, 0, m->parArray.second);
            glDisable(GL_LIGHTING);
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
    glDisable(GL_LIGHTING);
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

    if (!stop) tim = chekkFPS();

    glutSwapBuffers();
    if (!stop)
        glutPostRedisplay();
}

void printKeyHelp(string key,string action){
    printf("%-20s%-20s\n", key.c_str(), action.c_str());
}


void displayHelp(){
    printf("HELP: Key Bindings\n");
    printKeyHelp("Key","Action");
    printKeyHelp("Left Mouse Button","Camera look around");
    printKeyHelp("Right Mouse Button","Camera zoom");
    printKeyHelp("Key Arrow Down","Reduce camera sensibility");
    printKeyHelp("Key Arrow Up","Increase camera sensibility");
    printKeyHelp("Key 'p'","Toggle Draw Mode");
    printKeyHelp("Key 'e'","Toogle Show axis");
    printKeyHelp("Key 's'","Toogle Movement");
    printKeyHelp("Key 'l'","Toogle Lighting");
    printKeyHelp("Key 'h'","Show this help");
}

void processKeys(unsigned char c, int xx, int yy) {

    if (c == 'p' || c == 'P')drawMode = 1 - drawMode;
    if (c == 'e' || c == 'E')eixos = 1 - eixos;

    if (c == 's' || c == 'S'){
        if (!stop){
            double time = glutGet(GLUT_ELAPSED_TIME);
            timeStop = (time - timebase)/1000;
        }
        else{
            double time = glutGet(GLUT_ELAPSED_TIME);
            timebase = (time - timeStop*1000);
        }
        stop = 1 - stop;
    }

    if(c=='l' || c=='L') lighting = 1 - lighting;


    if (c=='h' || c=='H') displayHelp();

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
    glutPostRedisplay();

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
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ilInit();



    auto start_time = std::chrono::high_resolution_clock::now();




    if (argc != 2) {
        printf("nº de argumentos inválido\n");
        return 1;
    }
    char *filename = argv[1];

    try {
        srand(63);
        xmlParser parser(&cam,textures);
        groupMain = parser.readXml(filename);
    }
    catch (string s){
        printf("%s\n",s.c_str());
        return 1;
    }
    auto current_time = std::chrono::high_resolution_clock::now();
    float timeSinceStart = (float )std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();



    printf("Ready to draw | time of preparation :%fs\n", timeSinceStart/1000);

    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_RESCALE_NORMAL);



    int lightSize = (int) cam.lights.size();

    GLfloat dark[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat white[4] = {1.0, 1.0, 1.0, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white);
    for (int i = 0; i < lightSize;i++) {
        glEnable(GL_LIGHT0 +i);
        glLightfv(GL_LIGHT0+i, GL_AMBIENT, dark);
        glLightfv(GL_LIGHT0+i, GL_DIFFUSE, white);
        glLightfv(GL_LIGHT0+i, GL_SPECULAR, white);
    }

    //loadTexture();
    glEnable(GL_TEXTURE_2D);



    timebase = glutGet(GLUT_ELAPSED_TIME);
    displayHelp();
    // enter GLUT's main cycle
    glutMainLoop();
    return 0;
}