#include <iostream>
#include <fstream>
#include <list>
#include <iterator>

using namespace std;



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
        float initialHeight =0;
        for (int i = 0; i < slices;i++) {
            float angle = ((float) i) * anglePart;
            float nextAngle = angle + anglePart;
            for (int j = 0; j < stacks; j++) {
                glBegin(GL_TRIANGLES);
                float rDown =  r * (float )(stacks-j)/(float )stacks;
                float heightDown = initialHeight+heightPart*(float)j;
                float rUp = r * (float )(stacks-j-1)/(float )stacks;
                float heightUP = initialHeight + heightPart * (float)(j+1);

                //printf("%d | %f R: %f |%f and H: %f | %f \n",j,initialHeight,heightUP,heightDown,rUp,rDown);


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

void drawSphere(float radius, int slices,int stacks){
    float anglePart = 2 * M_PI / slices;
    float heightPart = M_PI/stacks;
    float initialAngle = -M_PI/2;
    for (int i = 0; i < 1;i++) {
        float angle = ((float) i) * anglePart;
        float nextAngle = angle + anglePart;
        for (int j = 0; j < stacks; j++) {
            glBegin(GL_TRIANGLES);
            float angleUp =   initialAngle+ heightPart*(float)(j+1);
            float angleDown = initialAngle+ heightPart*(float)(j);

            printf("%d | %f | %f\n",j,angleUp*(180/M_PI),angleDown*(180/M_PI));

            if(angleUp>=M_PI/2)angleUp=M_PI/2-0.00001;
            if(angleDown<=-M_PI/2)angleDown=-M_PI/2+0.000001;
            glColor3f(1, 1, 0);


            glColor3f(1,1,0);
            glVertex3f(radius* cos(angleUp)*sin(angle),radius*sin(angleUp),radius* cos(angle)* cos(angleUp) ) ;
            glVertex3f(radius* cos(angleDown)*sin(angle),radius*sin(angleDown),radius* cos(angle)* cos(angleDown));
            glVertex3f(radius* cos(angleDown)*sin(nextAngle),radius*sin(angleDown), radius * cos(nextAngle)* cos(angleDown));



            //glColor3f(1,0,0);
            glVertex3f(radius* cos(angleUp)*sin(nextAngle),radius* sin(angleUp), radius * cos(nextAngle)* cos(angleUp));
            glVertex3f(radius* cos(angleUp)*sin(angle),radius* sin(angleUp),radius* cos(angle)* cos(angleUp));
            glVertex3f(radius* cos(angleDown)*sin(nextAngle), radius* sin(angleDown), radius* cos(nextAngle)* cos(angleDown));

            glEnd();
        }
    }


}
void writeVerticetoFile(std::ofstream& file, float x,float y, float z){
    file << x << " " << y << " " << z << '\n';
}

void writeFile(list<float> list,string filename){
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

list<float> readFile(string filename){
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
        printf("%f |%f |%f\n",x,y,z);
        addVerticeToList(list,x,y,z);
    }
    return list;
}

void drawList (list<float>list){
    glBegin(GL_TRIANGLES);
    ::list<float>::iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        float x = *it;
        ++it;
        float y = *it;
        ++it;
        float z = *it;
        glVertex3f(x,y,z);
    }
    glEnd();
}

void drawPlane (float length,int divisions){

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

    writeFile(*list,"plane.3d");

    ::list<float> vertices= readFile("plane.3d");

    drawList(vertices);

}




void drawBox (float length, float divisions){
    float part = length/divisions;
    float halfAltura = length/2;
    float initX = - halfAltura;
    float initZ = initX;

    // Y Constante
    for (int i = 0;i < divisions;i++){
        for (int j = 0;j < divisions;j++){
            float posX = initX + i * part;
            float posZ = initZ + j * part;
            float nextPosX = posX + part;
            float nextPosZ = posZ + part;
            glBegin(GL_TRIANGLES);
            glVertex3f(nextPosX,halfAltura,nextPosZ);
            glVertex3f(nextPosX,halfAltura,posZ);
            glVertex3f(posX,halfAltura,posZ);

            glVertex3f(posX,halfAltura,posZ);
            glVertex3f(posX,halfAltura,nextPosZ);
            glVertex3f(nextPosX,halfAltura,nextPosZ);

            glVertex3f(posX,-halfAltura,posZ);
            glVertex3f(nextPosX,-halfAltura,posZ);
            glVertex3f(nextPosX,-halfAltura,nextPosZ);

            glVertex3f(nextPosX,-halfAltura,nextPosZ);
            glVertex3f(posX,-halfAltura,nextPosZ);
            glVertex3f(posX,-halfAltura,posZ);
            glEnd();
        }
    }
    // X constante
    for (int i = 0; i < divisions;i++){
        for (int j = 0; j < divisions;j++){
            float posY = halfAltura - i * part;
            float posZ = initZ + j * part;
            float netxPosZ = posZ + part;
            float nextPosY=posY-part;
            glBegin(GL_TRIANGLES);
            glVertex3f(initX,posY, netxPosZ);
            glVertex3f(initX,posY, posZ);
            glVertex3f(initX,nextPosY, posZ);

            glVertex3f(initX, posY, posZ + part);
            glVertex3f(initX, nextPosY, posZ);
            glVertex3f(initX, nextPosY, netxPosZ);

            glVertex3f(-initX,nextPosY, posZ);
            glVertex3f(-initX,posY, posZ);
            glVertex3f(-initX,posY, netxPosZ);

            glVertex3f(-initX, nextPosY, netxPosZ);
            glVertex3f(-initX, nextPosY, posZ);
            glVertex3f(-initX, posY, netxPosZ);
            glEnd();
        }
    }
    //Z constante
    for (int i = 0; i < divisions;i++){
        for (int j = 0; j < divisions;j++){
            float posY = halfAltura - i * part;
            float posX = initX + j * part;
            float nextPosY=posY-part;
            float nextPosX=posX+part;
            glBegin(GL_TRIANGLES);
            glVertex3f(posX,nextPosY, initZ);
            glVertex3f(posX,posY, initZ);
            glVertex3f(nextPosX,posY, initZ);

            glVertex3f(nextPosX, nextPosY, initZ);
            glVertex3f(posX, nextPosY, initZ);
            glVertex3f(nextPosX, posY, initZ);

            glVertex3f(posX + part,posY, -initZ);
            glVertex3f(posX,posY, -initZ);
            glVertex3f(posX,nextPosY, -initZ);

            glVertex3f(nextPosX, posY, -initZ);
            glVertex3f(posX, nextPosY, -initZ);
            glVertex3f(nextPosX, nextPosY, -initZ);
            glEnd();
        }
    }


}


void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    //glViewport(0, 0, 500,500);

    // Set perspective
    gluPerspective(40.0f ,1, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);

    // set the camera
    //glMatrixMode(GL_MODELVIEW);


    glLoadIdentity();
    gluLookAt(5,5,3,0,0,0,0.0f,1.0f,0.0f);


    //gluLookAt(radius*cos(betaCamera)* sin(alphaCamera),radius*sin(betaCamera), radius*cos(betaCamera)* cos(alphaCamera),
    //          0,0,0,
    //          0.0f,1.0f,0.0f);



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
    //glTranslatef(0,1,0);
    //drawPlane(2,10);
    //glTranslatef(0,-2,0);
    //glRotatef(180,1,0,1);
    //drawPlane(2,10);
    //glTranslatef(1,-1,0);
    //glRotatef(180,1,1,0);
    drawPlane(1,25);
    //glColor3f(1.0f, 1.0f, 1.0f);
    //drawCone(1,2,4,3);
    //drawSphere(1,20,10);
    //glColor3f(1.0f, 0.0f, 0.0f);
    //drawBox(1,2);
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
    //glutReshapeFunc(changeSize);

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


/*
int main(){
    xmlFunc();
    readXml();
    /*
    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile("SavedData.xml");
    XMLCheckResult(eResult);
    XMLNode * pRoot = xmlDoc.FirstChild();
    XMLElement * pElement = pRoot->FirstChildElement("group");
    if (pElement == nullptr) return XML_ERROR_PARSING_ELEMENT;

    const char * szAttributeText = nullptr;
    szAttributeText = pElement->GetText();
    if (szAttributeText == nullptr) return XML_ERROR_PARSING_ATTRIBUTE;
    std::string strOutMonth = szAttributeText;
    printf("element is %s\n",AttributeText);
    XMLCheckResult(eResult);

}
 */

