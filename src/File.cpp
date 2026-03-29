#include "../include/File.h"
#include <iostream>
#include <string>

//functia de clear fisier text
void File::freeResource()
{
    delete[] data;
    data = nullptr;
}

//functia de copiere fisier text
void File::copyFrom(const File& other)
{
    if(other.data)
    {
        this->data = new char[fileSize + 1];
        for(size_t i = 0; i < this->fileSize; ++i)
        {
            this->data[i] = other.data[i];
        }
        data[fileSize] = '\0';
    }
    else
    {
        this->data = nullptr;
    }
}

//coonstructor 
File::File(const std::string& name, const std::string& content) : Component(name), fileSize(content.length())
{
    data = new char[fileSize + 1];
    for(size_t i = 0; i < fileSize; ++i)
    {
        data[i] = content[i];
    }
    data[fileSize] = '\0';
}

//destrucotr
File::~File()
{
    freeResource();
}

//copy consturtor 
File::File(const File& other) : Component(other.getName()), fileSize(other.fileSize)
{
    copyFrom(other);
}

//suprascrierea operatorului de atribuire
File& File::operator=(const File& other)
{
    if(this != &other)
    {
        freeResource();
        this->name = other.name;
        copyFrom(other);
    }
    return *this;
}

//functia de clone
Component* File::clone() const
{
    return new File(*this);
}

//functia de afisare date fisier text
void File::display(int depth) const{
    for(int i = 0; i < depth; ++i)
        std::cout << " ";
    std::string time = ctime(&timestamp);
    time.pop_back();
    std::cout<< time << " -> File " << name << " has " << fileSize << " bytes\n";
}

//getter de dimensiune fisier
size_t File::getSize() const
{
    return fileSize;
}
//setter nume 
//setter nume
void File::setName(const std::string& name)
{
    this->name = name;
}

//afisare continut
std::string File::readContent() const{
    if(!data || fileSize == 0)
        return "";
    return data;
}

