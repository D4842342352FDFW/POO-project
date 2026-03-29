#include "../include/Component.h"
#include <iostream>

//initializare contor componente
int Component::instanceCount = 0;

//constructor componenta  
Component::Component(const std::string& name) : name(name), size(0), timestamp(time(nullptr)) 
{
    parent = nullptr;
    instanceCount++;
}

//destructor componenta
Component::~Component()
{
    instanceCount--;
}

//getterele
int Component::getInstanceCount()
{
    return instanceCount;
}
std::string Component::getName() const{
    return name;
}
Directory* Component::getParent() const{
    return parent;
}

//setter
void Component::setParent(Directory* newParent)
{
    parent = newParent;
}
