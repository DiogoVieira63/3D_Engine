//
// Created by diogo on 11/03/22.
//

#include "xmlParser.h"
#include "tinyxml2.h"

using namespace tinyxml2;

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if ((a_eResult) != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; };
#endif

/*
int xmlFunc() {
    XMLDocument xmlDoc;
    XMLNode * pRoot = xmlDoc.NewElement("world");
    xmlDoc.InsertFirstChild(pRoot);
    XMLElement * pElement = xmlDoc.NewElement("group");
    pElement->SetText("Diogo");
    pRoot->InsertEndChild(pElement);
    pElement = xmlDoc.NewElement("camera");
    XMLElement * pElementModel = xmlDoc.NewElement("position");
    pElementModel->SetAttribute("x",10);
    pElementModel->SetAttribute("y",20);
    pElementModel->SetAttribute("z",30);
    pElement->InsertEndChild(pElementModel);
    pElementModel = xmlDoc.NewElement("lookAt");
    pElementModel->SetAttribute("x",10);
    pElementModel->SetAttribute("y",20);
    pElementModel->SetAttribute("z",30);
    pElement->InsertEndChild(pElementModel);

    pElementModel = xmlDoc.NewElement("up");
    pElementModel->SetAttribute("x",10);
    pElementModel->SetAttribute("y",20);
    pElementModel->SetAttribute("z",30);
    pElement->InsertEndChild(pElementModel);

    pElementModel = xmlDoc.NewElement("projection");
    pElementModel->SetAttribute("fov",60);
    pElementModel->SetAttribute("near",1);
    pElementModel->SetAttribute("far",1000);
    pElement->InsertEndChild(pElementModel);

    pRoot->InsertEndChild(pElement);
    pElement = xmlDoc.NewElement("group");

    XMLElement * pElement2 = xmlDoc.NewElement("models");
    pElementModel = xmlDoc.NewElement("model");
    pElementModel->SetAttribute("file","cone.3d");
    pElement2->InsertEndChild(pElementModel);
    pElement->InsertEndChild(pElement2);
    pRoot->InsertEndChild(pElement);

    XMLError eResult = xmlDoc.SaveFile("SavedData.xml");
    XMLCheckResult(eResult);
    return 0;
}
 */


int readXml(){
    XMLDocument xmlDoc;
    xmlDoc.LoadFile("SavedData.xml");
    XMLNode * pRoot = xmlDoc.FirstChild();

    XMLElement * pElement = pRoot->FirstChildElement("camera");
    XMLElement *pElement2 = pElement->FirstChildElement("position");
    int x= pElement2->IntAttribute("x");
    int y = pElement2->IntAttribute("y");
    int z = pElement2->IntAttribute("z");
    printf("Position : %d | %d | %d\n",x,y,z);

    pElement2 = pElement->FirstChildElement("lookAt");
    x= pElement2->IntAttribute("x");
    y = pElement2->IntAttribute("y");
    z = pElement2->IntAttribute("z");
    printf("LookAt : %d | %d | %d\n",x,y,z);

    pElement2 = pElement->FirstChildElement("up");
    x= pElement2->IntAttribute("x");
    y = pElement2->IntAttribute("y");
    z = pElement2->IntAttribute("z");
    printf("up : %d | %d | %d\n",x,y,z);

    pElement2 = pElement->FirstChildElement("projection");
    int fov= pElement2->IntAttribute("fov");
    int near = pElement2->IntAttribute("near");
    int far = pElement2->IntAttribute("far");
    printf("projection : %d | %d | %d\n",fov,near,far);

    pElement = pRoot->FirstChildElement("group");
    pElement2 = pElement->FirstChildElement("models");
    XMLElement *pElement3 = pElement2->FirstChildElement("model");
    while (pElement3 != nullptr){
        const char* filename = pElement3->Attribute("file");
        printf("file -> %s\n",filename);
        pElement3 = pElement3->NextSiblingElement("model");
    }

    //XMLCheckResult(eResult);
}

int main (){
    readXml();
}