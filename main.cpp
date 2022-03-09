#include <iostream>
#include "tinyxml2.h"

using namespace tinyxml2;

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if ((a_eResult) != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; } else printf("all good\n");
#endif

int xmlFunc() {
    XMLDocument xmlDoc;
    XMLNode * pRoot = xmlDoc.NewElement("Root");
    xmlDoc.InsertFirstChild(pRoot);
    XMLElement * pElement = xmlDoc.NewElement("IntValue");
    pElement->SetText(10);
    pRoot->InsertEndChild(pElement);
    XMLError eResult = xmlDoc.SaveFile("SavedData.xml");
    XMLCheckResult(eResult);
    return 0;
}


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdio>

static float angleY = 0;
static float angleX = 0;

static float betaCamera = M_PI/4;
static float alphaCamera = M_PI/4;

static float radius = 5;

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
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}


void drawCylinder(float r, float height, int slices) {
    float anglePart = 2 * M_PI / slices;
    float halfHeight = height / 2;
    //printf("angle -> %f and %f\n",angleHeight,anglePart);
    float colorAdd = (float) 1 / ((float) slices / 2);
    float color = colorAdd;

    for (int i = 0; i < slices; i++) {
        glBegin(GL_TRIANGLES);
        float angle = ((float) i) * anglePart;
        float nextAngle = angle + anglePart;
        if (color > 1) colorAdd *= -1;

        //glColor3f(color,0,1-color);
        glColor3f(0, 0, 1);
        glVertex3f(r * sin(angle), halfHeight, r * cos(angle));
        glVertex3f(r * sin(angle), -halfHeight, r * cos(angle));
        glVertex3f(r * sin(nextAngle), -halfHeight, r * cos(nextAngle));

        //glColor3f(color,0,1-color);
        glColor3f(0, 0, 1);
        glVertex3f(r * sin(nextAngle), halfHeight, r * cos(nextAngle));
        glVertex3f(r * sin(angle), halfHeight, r * cos(angle));
        glVertex3f(r * sin(nextAngle), -halfHeight, r * cos(nextAngle));

        glColor3f(1, 0, 1);
        glVertex3f(0, halfHeight, 0);
        glVertex3f(r * sin(angle), halfHeight, r * cos(angle));
        glVertex3f(r * sin(nextAngle), halfHeight, r * cos(nextAngle));

        glColor3f(1, 1, 0);
        glVertex3f(r * sin(nextAngle), -halfHeight, r * cos(nextAngle));
        glVertex3f(r * sin(angle), -halfHeight, r * cos(angle));
        glVertex3f(0, -halfHeight, 0);

        glEnd();
        color += colorAdd;
    }
}

    void drawCone(float r, float height, int slices,int stacks) {
        float anglePart = 2 * M_PI / slices;
        float heightPart = height/stacks;

        float initialHeight =- height/2;


        for (int i = 0; i < slices;i++) {
            float angle = ((float) i) * anglePart;
            float nextAngle = angle + anglePart;
            for (int j = 0; j < stacks; j++) {
                glBegin(GL_TRIANGLES);
                float rDown =  r * (float )(stacks-j)/(float )stacks;
                float heightDown = initialHeight+heightPart*(float)j;
                float rUp = r * (float )(stacks-j-1)/(float )stacks;
                float heightUP = initialHeight + heightPart * (float)(j+1);

                printf("%d | %f R: %f |%f and H: %f | %f \n",j,initialHeight,heightUP,heightDown,rUp,rDown);


                glColor3f(1, 1, 0);

                glVertex3f(rUp* sin(nextAngle),heightUP,rUp* cos(nextAngle));
                glVertex3f(rUp* sin(angle),heightUP,rUp* cos(angle));
                glVertex3f(rDown * sin(angle), heightDown, rDown * cos(angle));



                glVertex3f(rDown* sin(angle), heightDown, rDown * cos(angle));
                glVertex3f(rDown* sin(nextAngle),heightDown,rDown* cos(nextAngle));
                glVertex3f(rUp* sin(nextAngle), heightUP, rUp* cos(nextAngle));

                glEnd();
            }
            glBegin(GL_TRIANGLES);
            glColor3f(1, 1, 0);
            glVertex3f(r * sin(nextAngle), initialHeight, r * cos(nextAngle));
            glVertex3f(r * sin(angle), initialHeight, r * cos(angle));
            glVertex3f(0, initialHeight, 0);
            glEnd();
        }
}
/*
void drawSphere(float radius, int slices,int stacks){
    float anglePart = 2 * M_PI / slices;
    float heightPart = radius/stacks;
    float initialAngle = M_PI/stacks;
    for (int i = 0; i < slices;i++) {
        float angle = ((float) i) * anglePart;
        float nextAngle = angle + anglePart;
        for (int j = 0; j < stacks; j++) {
            glBegin(GL_TRIANGLES);
            float rDown =  radius * (float )(stacks-j)/(float )stacks;
            float heightDown = initialHeight+heightPart*(float)j;
            float rUp = radius * (float )(stacks-j-1)/(float )stacks;
            float heightUP = initialHeight + heightPart * (float)(j+1);

            printf("%d | %f R: %f |%f and H: %f | %f \n",j,initialHeight,heightUP,heightDown,rUp,rDown);


            glColor3f(1, 1, 0);

            glVertex3f(rUp* sin(nextAngle),heightUP,rUp* cos(nextAngle));
            glVertex3f(rUp* sin(angle),heightUP,rUp* cos(angle));
            glVertex3f(rDown * sin(angle), heightDown, rDown * cos(angle));



            glVertex3f(rDown* sin(angle), heightDown, rDown * cos(angle));
            glVertex3f(rDown* sin(nextAngle),heightDown,rDown* cos(nextAngle));
            glVertex3f(rUp* sin(nextAngle), heightUP, rUp* cos(nextAngle));

            glEnd();
        }
    }


}

 */
void drawPlane (float length,float divisions){
    float inicial_posX = - (length*divisions)/2;
    float inicial_posZ = inicial_posX;
    for (int i = 0;i < divisions;i++){
        for (int j = 0;j < divisions;j++){
            float posX = inicial_posX + i * length;
            float posZ = inicial_posZ + j * length;
            glBegin(GL_TRIANGLES);
            glVertex3f(posX+length,0,posZ+length);
            glVertex3f(posX+length,0,posZ);
            glVertex3f(posX,0,posZ);

            glVertex3f(posX,0,posZ);
            glVertex3f(posX,0,posZ+length);
            glVertex3f(posX+length,0,posZ+length);
            glEnd();
        }
    }
}


void drawBox (float length, float divisions){
    float initX = - (length * divisions) / 2;
    float initZ = initX;
    float halfAltura = length*divisions/2;
    for (int i = 0;i < divisions;i++){
        for (int j = 0;j < divisions;j++){
            float posX = initX + i * length;
            float posZ = initZ + j * length;
            glBegin(GL_TRIANGLES);
            glVertex3f(posX+length,halfAltura,posZ+length);
            glVertex3f(posX+length,halfAltura,posZ);
            glVertex3f(posX,halfAltura,posZ);

            glVertex3f(posX,halfAltura,posZ);
            glVertex3f(posX,halfAltura,posZ+length);
            glVertex3f(posX+length,halfAltura,posZ+length);

            glVertex3f(posX,-halfAltura,posZ);
            glVertex3f(posX+length,-halfAltura,posZ);
            glVertex3f(posX+length,-halfAltura,posZ+length);

            glVertex3f(posX+length,-halfAltura,posZ+length);
            glVertex3f(posX,-halfAltura,posZ+length);
            glVertex3f(posX,-halfAltura,posZ);
            glEnd();
        }
    }

    for (int i = 0; i < divisions;i++){
        for (int j = 0; j < divisions;j++){
            float posY = halfAltura - i * length;
            float posZ = initZ + j * length;
            glBegin(GL_TRIANGLES);
            glVertex3f(initX,posY, posZ+length);
            glVertex3f(initX,posY, posZ);
            glVertex3f(initX,posY-length, posZ);

            glVertex3f(initX, posY, posZ + length);
            glVertex3f(initX, posY - length, posZ);
            glVertex3f(initX, posY - length, posZ + length);

            glVertex3f(-initX,posY-length, posZ);
            glVertex3f(-initX,posY, posZ);
            glVertex3f(-initX,posY, posZ+length);

            glVertex3f(-initX, posY - length, posZ + length);
            glVertex3f(-initX, posY - length, posZ);
            glVertex3f(-initX, posY, posZ + length);
            glEnd();
        }
    }

    for (int i = 0; i < divisions;i++){
        for (int j = 0; j < divisions;j++){
            float posY = halfAltura - i * length;
            float posX = initX + j * length;
            glBegin(GL_TRIANGLES);
            glVertex3f(posX,posY-length, initZ);
            glVertex3f(posX,posY, initZ);
            glVertex3f(posX + length,posY, initZ);

            glVertex3f(posX+length, posY - length, initZ);
            glVertex3f(posX, posY - length, initZ);
            glVertex3f(posX+length, posY, initZ);

            glVertex3f(posX + length,posY, -initZ);
            glVertex3f(posX,posY, -initZ);
            glVertex3f(posX,posY-length, -initZ);

            glVertex3f(posX+length, posY, -initZ);
            glVertex3f(posX, posY - length, -initZ);
            glVertex3f(posX+length, posY - length, -initZ);
            glEnd();
        }
    }


}


void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(radius*cos(betaCamera)* sin(alphaCamera),radius*sin(betaCamera), radius*cos(betaCamera)* cos(alphaCamera),
              0,0,0,
              0.0f,1.0f,0.0f);

    glRotatef(angleY,0,1,0);
    glRotatef(angleX,1,0,0);


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



    //drawCylinder(1,2,1000);
    //drawPlane(0.5,20);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCone(2,3,50,50);
    glColor3f(1.0f, 0.0f, 0.0f);
    //drawBox(0.1,25);
    // End of frame
    glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {
    if (c == 'a')angleY+=1;
    if (c == 'q')angleX+=1;

    if(c == '1')radius+=1;
    if(c == '2')radius-=1;

    glutPostRedisplay();

}


void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here

    if (key == GLUT_KEY_UP) betaCamera+=0.1;
    if (key == GLUT_KEY_DOWN) betaCamera-=0.1;
    if (key == GLUT_KEY_LEFT) alphaCamera+=0.1;
    if (key == GLUT_KEY_RIGHT) alphaCamera-=0.1;

    if (betaCamera>=1.5)betaCamera=1.5;
    if (betaCamera<=-1.5)betaCamera=-1.5;

    glutPostRedisplay();


}


int main(int argc, char **argv) {

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
