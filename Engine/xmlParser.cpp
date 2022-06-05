//
// Created by diogo on 30/04/22.
//

#include "xmlParser.h"

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


void xmlParser::readColor(XMLElement *pElement, Model *m){
    float r,g,b;
    XMLElement *pElement2 = pElement->FirstChildElement("diffuse");
    r = pElement2->FloatAttribute("R", 200);
    g = pElement2->FloatAttribute("G", 200);
    b = pElement2->FloatAttribute("B", 200);


    (*m).color.setDiffuse(r,g,b);

    pElement2 = pElement->FirstChildElement("ambient");
    r = pElement2->FloatAttribute("R", 50);
    g = pElement2->FloatAttribute("G", 50);
    b = pElement2->FloatAttribute("B", 50);
    (*m).color.setAmbient(r,g,b);

    pElement2 = pElement->FirstChildElement("specular");
    r = pElement2->FloatAttribute("R", 0);
    g = pElement2->FloatAttribute("G", 0);
    b = pElement2->FloatAttribute("B", 0);

    (*m).color.setSpecular(r,g,b);

    pElement2 = pElement->FirstChildElement("emissive");
    r = pElement2->FloatAttribute("R", 0);
    g = pElement2->FloatAttribute("G", 0);
    b = pElement2->FloatAttribute("B", 0);

    (*m).color.setEmissive(r,g,b);


    pElement2 = pElement->FirstChildElement("shininess");
    float value = pElement2->FloatAttribute("value", 0);
    (*m).color.shininess = value;
}

Group * xmlParser::readGroup(XMLElement *pElement) {
    auto *group = new Group();
    XMLElement *pElement2;
    pElement2 = pElement->FirstChildElement("models");
    XMLElement *pElement3;
    if (pElement2 != nullptr) {
        pElement3 = pElement2->FirstChildElement("model");
        const char *file;
        Model *m;
        while (pElement3 != nullptr) {
            file = pElement3->Attribute("file");
            m = new Model(file);
            (*group).addModel(m);


            if (mapArraysModel.find(file) == mapArraysModel.end()) {
                unsigned  int size = mapArraysModel.size()*3;
                auto *vectorS = (unsigned int *)malloc(sizeof (unsigned int) *3);
                vectorS[0] = size;
                vectorS[1] = size+1;
                vectorS[2] = size+2;

                pair<unsigned int*,unsigned int> par = pair<unsigned int*,unsigned int>(vectorS,-1);
                mapArraysModel.insert(pair<string,pair<unsigned int*,unsigned int>>(file,par));
            }

            XMLElement *pElement4 = pElement3->FirstChildElement("color");
            if (pElement4 != nullptr){
                readColor(pElement4,m);
            }

            pElement4 = pElement3->FirstChildElement("texture");
            if (pElement4 != nullptr){
                const char* texFilename;
                texFilename = pElement4->Attribute("file");
                (*m).setTexFilename(texFilename);
                if(mapTextures.find(texFilename) == mapTextures.end()){
                    unsigned int size = mapTextures.size();
                    mapTextures.insert(pair<string,pair<bool,unsigned >>(texFilename,pair<bool,unsigned int>(false,size)));
                }
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
            readTransform(pElement2, grupoAdd);
            (*group).addSubGroup(grupoAdd);
        }
    }

    pElement2 = pElement->FirstChildElement("group");
    while (pElement2 != nullptr) {
        Group *subGroup = readGroup(pElement2);
        (*group).addSubGroup(subGroup);
        pElement2 = pElement2->NextSiblingElement("group");
    }

    return group;
}

static float dX, dY, dZ;
static float posX , posY, posZ;
static float lookAtX, lookAtY, lookAtZ;
static float upX = 0, upY = 1, upZ = 0;
static float fov = 60, near = 1, far = 1000;
static float radius, alpha, beta,radiusMax;


void convertToSpherical(Camera *cam) {
    alpha = atan(posX / posZ);
    beta = tan((posY * sin(alpha)) / posX);
    if (beta >= M_PI / 2)beta -= M_PI;
    if (beta <= -M_PI / 2)beta += M_PI;
    radius = posY / sin(beta);

    float betaDeg = utils::toDegree(beta);
    if (betaDeg > 85) beta = 85;
    if (betaDeg < -85) beta = -85;


    cam->setPos(posX,posY,posZ);

    float d[3];
    beta *= -1;
    alpha+= M_PI;
    d[0] = lookAtX - posX;
    d[1] = lookAtY - posY;
    d[2] = lookAtZ - posZ;



    cam->setAlphaBeta(alpha,beta);

    utils::normalize(d);

    cam->setD(d[0],d[1],d[2]);

}


int readCamera(Camera *camera, XMLElement *pElement) {
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
    if (error != XML_SUCCESS) radiusMax = 100000;
    camera->setRadiusMax(radiusMax);

    if (error != XML_SUCCESS)
        radiusMax = 1000000;
    convertToSpherical(camera);

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

    camera->setUp(upX,upY,upZ);

    pElement2 = pElement->FirstChildElement("projection");
    if (pElement2 != nullptr) {
        pElement2->QueryFloatAttribute("fov", &fov);
        pElement2->QueryFloatAttribute("near", &near);
        pElement2->QueryFloatAttribute("far", &far);
    }
    camera->fov = fov;
    camera->far = far;
    camera->near = near;

    return 0;
}
int readLights(Camera *camera, XMLElement *pElement) {
    XMLElement *pElement2 = pElement->FirstChildElement("light");
    while(pElement2 != nullptr){
        const char *type;
        type = pElement2->Attribute("type");


        int lightNum = (int) camera->lights.size();

        if(lightNum >= 9) break;

        if(!strcmp(type,"point")){
            float x, y,z;
            pElement2->QueryFloatAttribute("posX",&x);
            pElement2->QueryFloatAttribute("posY",&y);
            pElement2->QueryFloatAttribute("posZ",&z);
            Ponto ponto = Ponto(x,y,z);

            auto *light = new LightPoint(ponto, lightNum);
            camera->lights.push_back(light);
        }
        if(!strcmp(type,"directional")){
            float x, y,z;
            pElement2->QueryFloatAttribute("dirX",&x);
            pElement2->QueryFloatAttribute("dirY",&y);
            pElement2->QueryFloatAttribute("dirZ",&z);
            Ponto ponto = Ponto(x,y,z);

            auto *light = new LightDirectional(ponto, lightNum);
            camera->lights.push_back(light);
        }
        if(!strcmp(type,"spotlight")){
            float x, y,z;
            pElement2->QueryFloatAttribute("posX",&x);
            pElement2->QueryFloatAttribute("posY",&y);
            pElement2->QueryFloatAttribute("posZ",&z);
            Ponto posPonto = Ponto(x,y,z);
            pElement2->QueryFloatAttribute("dirX",&x);
            pElement2->QueryFloatAttribute("dirY",&y);
            pElement2->QueryFloatAttribute("dirZ",&z);
            Ponto dirPonto = Ponto(x,y,z);
            float cutoff;
            pElement2->QueryFloatAttribute("cutoff",&cutoff);
            auto *light = new LightSpotLight(posPonto,lightNum,dirPonto,cutoff);
            camera->lights.push_back(light);
        }

        pElement2 = pElement2->NextSiblingElement();
    }
    return 0;
}


Group * xmlParser::readXml(const char *filename) {
    XMLDocument xmlDoc;
    XMLError erro = xmlDoc.LoadFile(filename);
    if (erro != XML_SUCCESS){
        string s = "Ficheiro '" + string (filename) + "': não existe ou erro ao ler.";
        throw string (s);
    }
    XMLNode *pRoot = xmlDoc.FirstChild();

    XMLElement *pElement = pRoot->FirstChildElement("camera");
    readCamera(cam,pElement);
    pElement = pRoot->FirstChildElement("lights");
    if(pElement != nullptr) readLights(cam,pElement);



    pElement = pRoot->FirstChildElement("group");
    Group* groupMain = readGroup(pElement);
    printf("Parsing completed\n");

    printf("Reading files and generating buffers...\n");
    readModels(groupMain);

    printf("Buffers generated:\n");
    int size = (int)mapArraysModel.size();
    printf("Models : %d\n",size);
    size = (int)mapTextures.size();
    printf("Textures : %d\n",size);


    return groupMain;
}

vector<float> readVerticesFile(ifstream *file){
    vector<float> vectorPoints;
    int nrVertices;
    if(*file >> nrVertices) {
        for (int i = 0; i < nrVertices; i++) {
            float x, y, z;
            *file >> x;
            *file >> y;
            *file >> z;

            vectorPoints.push_back(x);
            vectorPoints.push_back(y);
            vectorPoints.push_back(z);
        }
    }
    return vectorPoints;
}

vector<float> xmlParser::readVerticesFileText(ifstream *file) {
    int nrVertices;

    vector<float> vectorPoints;
    if(*file >> nrVertices) {
        for (int i = 0; i < nrVertices; i++) {
            float x, y, z;
            *file >> x;
            *file >> y;
            *file >> z;

            vectorPoints.push_back(x);
            vectorPoints.push_back(y);
        }
    }
    return vectorPoints;
}


void xmlParser::loadTexture(Model *m,GLuint num) {

    unsigned int t, tw, th;
    unsigned char *texData;
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring)(*m).texFilename.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1, &textures[num]);



    glBindTexture(GL_TEXTURE_2D, textures[num]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    //glGenerateMipmap(GL_TEXTURE_2D);
}

void xmlParser::setupTexture(Model *m){
    auto parArray = mapArraysModel.find((*m).filename);

    if((*m).parText.second == -1) {

        auto tex = mapTextures.find((*m).texFilename);
        (*m).parText.second = tex->second.second;
        if(!tex->second.first) {

            loadTexture(m,tex->second.second);

            tex->second.first = true;

        }
    }
}

void xmlParser::readFile(Model *m) {

    string filename = (*m).filename;
    auto parArray = mapArraysModel.find(filename);
    if (parArray->second.second != -1){
        (*m).parArray = parArray->second;
        setupTexture(m);
        return;
    }
    ifstream file(filename);
    if (file.fail()){
        string s = "Ficheiro model '" + filename + "' não existe.";
        throw (s);
    }
    vector<float> vertices = readVerticesFile(&file);

    vector<float> normal = readVerticesFile(&file);
    vector<float> tex = readVerticesFileText(&file);

    setupTexture(m);

    file.close();


    glGenBuffers(1, &parArray->second.first[0]);


    glBindBuffer(GL_ARRAY_BUFFER, parArray->second.first[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float) * vertices.size(),vertices.data(),GL_STATIC_DRAW);


    if (!normal.empty()){
        glGenBuffers(1, &parArray->second.first[1]);
        glBindBuffer(GL_ARRAY_BUFFER, parArray->second.first[1]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(float) * normal.size(),normal.data(),GL_STATIC_DRAW);
    }


    if (!tex.empty()) {
        glGenBuffers(1, &parArray->second.first[2]);
        glBindBuffer(GL_ARRAY_BUFFER, parArray->second.first[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tex.size(), tex.data(), GL_STATIC_DRAW);
    }



    unsigned int verticeCount = vertices.size()/3;
    parArray->second.second = verticeCount;
    (*m).parArray = parArray->second;



}

void xmlParser::readModels(Group *g) {

    for (auto m: (*g).models){
        readFile(m);
    }


    for (auto g1: g->subGroups) {
        readModels(g1);
    }


}




