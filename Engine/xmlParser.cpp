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


Group * xmlParser::readGroup(XMLElement *pElement) {
    auto *group = new Group();
    XMLElement *pElement2;
    pElement2 = pElement->FirstChildElement("models");
    XMLElement *pElement3;
    if (pElement2 != nullptr) {
        pElement3 = pElement2->FirstChildElement("model");
        const char *file;
        while (pElement3 != nullptr) {
            file = pElement3->Attribute("file");
            auto *m = new Model(file);
            (*group).addModel(m);

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

    //posX= radius * cos(beta) * sin(alpha);
    //posY= radius * sin(beta);
    //posZ= radius * cos(beta) * cos(alpha);


    cam->setPos(posX,posY,posZ);

    float d[3];
    beta *= -1;
    alpha+= M_PI;
    d[0] = lookAtX - posX;
    d[1] = lookAtY - posY;
    d[2] = lookAtZ - posZ;

    printf("D %f | %f %f \n",d[0],d[1],d[2]);


    d[0] = cos(utils::toRad(beta)) * sin(utils::toRad(alpha));
    d[1] = sin(utils::toRad(beta));
    d[2] = cos(utils::toRad(beta)) * cos(utils::toRad(alpha));
    d[0] = lookAtX - posX;
    d[1] = lookAtY - posY;
    d[2] = lookAtZ - posZ;

    printf("D2 %f %f %f \n",d[0],d[1],d[2]);

    cam->setAlphaBeta(alpha,beta);

    utils::normalize(d);
    printf("D3 %f %f %f \n",d[0],d[1],d[2]);

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

Group * xmlParser::readXml(const char *filename ) {
    XMLDocument xmlDoc;
    XMLError erro = xmlDoc.LoadFile(filename);
    if (erro != XML_SUCCESS){
        string s = "Ficheiro '" + string (filename) + "': não existe ou erro ao ler.";
        throw string (s);
    }
    XMLNode *pRoot = xmlDoc.FirstChild();

    XMLElement *pElement = pRoot->FirstChildElement("camera");
    readCamera(cam,pElement);

    pElement = pRoot->FirstChildElement("group");
    Group* groupMain = readGroup(pElement);

    readModels(groupMain);

    return groupMain;
}


int xmlParser::readFile(Model *m) {
    string filename = (*m).filename;
    auto parArray = mapArraysModel.find(filename);
    if (parArray->second.second != -1){
        (*m).parArray = parArray->second;
        return 0;
    }

    ifstream file(filename);
    if (file.fail()){
        string s = "Ficheiro model '" + filename + "' não existe.";
        throw (s);
    }
    int nrVertices;
    file >> nrVertices;
    vector<float> vectorPoints;
    for (int i = 0; i < nrVertices; i++) {
        float x, y, z;
        file >> x;
        file >> y;
        file >> z;

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

    return 0;
}

void xmlParser::readModels(Group *g) {

    for (auto m: (*g).models)
        readFile(m);

    for (auto g1: g->subGroups)
        readModels(g1);

}