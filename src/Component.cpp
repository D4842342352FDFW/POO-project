#include "../include/Component.h"
#include <iostream>
#include "../include/Exception.h"

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

bool Component::supportsReadableContent() const
{
    return false;
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
time_t Component::getTimestamp() const{
    return timestamp;
}
std::string Component::getStoragePath() const{
    return storagePath;
}
std::string Component::getReadableContent() const
{
    throw Exception("Readable content is not available for this component type");
}
std::vector<std::string> Component::getMetadataLines() const
{
    return {};
}
std::string Component::getDisplayContent() const
{
    return "";
}
//continut default pentru componente fara payload textual
std::string Component::getPersistentContent()
{
    return "";
}

//settere
void Component::setParent(Directory* newParent)
{
    parent = newParent;
}
void Component::setTimestamp(time_t newTimestamp)
{
    timestamp = newTimestamp;
}
void Component::setStoragePath(const std::string& path)
{
    storagePath = path;
}
void Component::setReadableContent(const std::string&)
{
    throw Exception("Writable content is not available for this component type");
}
