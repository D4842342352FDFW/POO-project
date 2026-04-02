#include "../include/File.h"
#include <iostream>
#include <string>


//coonstructor 
File::File(const std::string& name, const std::string& content) : Component(name), data(content), fileSize(content.length()) {}

//copy consturtor 
File::File(const File& other) : Component(other.getName()), data(other.data), fileSize(other.fileSize) {}

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
void File::setName(const std::string& name)
{
    this->name = name;
}

//afisare continut
std::string File::readContent() const{
    if(fileSize == 0)
        return "";
    return data;
}