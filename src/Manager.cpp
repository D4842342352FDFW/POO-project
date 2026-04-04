#include "../include/Manager.h"
#include "../include/Directory.h"
#include "../include/File.h"
#include "../include/ImageFile.h"
#include "../include/VideoFile.h"
#include "../include/EncryptedFile.h"
#include "../include/CompressedFile.h"
#include "../include/SecureArchive.h"
#include "../include/Exception.h"
#include <iostream>


//initializare instanta statica
Manager* Manager::instance = nullptr;

//getter instanta
Manager* Manager::getInstance()
{
    if(!instance)
        instance = new Manager();
    return instance;
}

void Manager::destroyInstance()
{
    delete instance;
    instance = nullptr;
}

//constructor 
Manager::Manager()
{
    root = new Directory("root");
    currentDirectory = root;
    globalIndex.emplace("root", root);
}

//destructor
Manager::~Manager()
{
    delete root;
}

//eliminare din indexul global 
void Manager::removeFromIndex(Component* component)
{
    if(component == nullptr)
    {
        return;
    }

    //daca e folder eliminam recursiv toti copii
    if(component->isDirectory())
    {
        Directory* directory = static_cast<Directory*>(component);
        for(auto child : directory->getChildren())
        {
            removeFromIndex(child);
        }
    }

    //cautam si eliminam componenta specifica
    auto range = globalIndex.equal_range(component->getName());
    for(auto it = range.first; it != range.second; )
    {
        if(it->second == component)
        {
            it = globalIndex.erase(it);
        }
        else
        {
            ++it;   
        }
    }
}

//luam recursiv calea completa 
std::string Manager::getComponentPath(Component* component) const
{
    if(component == nullptr)
    {
        return "";
    }

    std::string path = component->getName();
    Directory* parent = component->getParent();

    while(parent)
    {
        path = parent->getName() + "/" + path;
        parent = parent->getParent();
    }

    return path;
}

//cautam componenta dupa nume initial in folderul curent
//si dupa global prin toata structura
Component* Manager::findByName(const std::string& name) const
{
    Component* fromCurrentDirectory = currentDirectory->findComponent(name);
    if(fromCurrentDirectory)
    {
        return fromCurrentDirectory;
    }

    auto range = globalIndex.equal_range(name);
    if(range.first == range.second)
    {
        throw NotFoundException(name);
    }

    //sa nu existe elemente cu acelasi nume
    auto next = range.first;
    ++next;
    if(next != range.second)
    {
        throw Exception("Multiple elements with the same name exist");
    }

    return range.first->second;
}

//crearea folder
void Manager::createDirectory(const std::string& name)
{
    if(currentDirectory->findComponent(name))
    {
        throw Exception("Element already exists in current directory");
    }

    auto newDir = new Directory(name);
    currentDirectory->addComponent(newDir);
    globalIndex.emplace(name, newDir);
}

//creare fisier
void Manager::createFile(const std::string& name, const std::string& content, int type,
int width, int height, int bpp, int fps, double duration,
const std::string& format)
{
    Component* newFile = nullptr;

    if(currentDirectory->findComponent(name))
    {
        throw Exception("Element already exists in current directory");
    }

    if(type == 1)
        newFile = new File(name, content);
    if(type == 2)
        newFile = new ImageFile(name, width, height, bpp, format);
    if(type == 3)
        newFile = new VideoFile(name, width, height, bpp, fps, duration, format);
    if(type == 4) 
        newFile = new EncryptedFile(name, content);
    if(type == 5)
        newFile = new CompressedFile(name, content);
    if(type == 6)
        newFile = new SecureArchive(name, content);

    if(newFile == nullptr)
    {
        throw Exception("Invalid file type");
    }
    
    currentDirectory->addComponent(newFile);
    globalIndex.emplace(name, newFile);
}

//stergere componenta din directorul curent si din index
void Manager::deleteComponent(const std::string& name)
{
    Component* comp = currentDirectory->findComponent(name);
    if(comp)
    {
        if(comp == root)
        {
            throw Exception("Cannot delete root directory");
        }
        if(comp->getParent())
        {
            removeFromIndex(comp);
            comp->getParent()->removeComponent(name);
        }
    }
    else
    {
        throw NotFoundException(name);
    }
}

//afisare arbore directoare + fisiere
void Manager::displayTree() const{
    std::cout << "\n------------------------------------------\n";
    root->display(0);
    std::cout << "------------------------------------------\n";
} 

//cautare si afisare detalii
void Manager::search(const std::string& name) const{
    auto range = globalIndex.equal_range(name);
    if(range.first != range.second)
    {
        for(auto it = range.first; it != range.second; ++it)
        {
            std::cout << "Found " << getComponentPath(it->second) << " | Size: " << it->second->getSize() << " bytes\n";
        }
    }
    else
    {
        throw NotFoundException(name);
    }
}

//revenire la radacina
void Manager::resetToRoot()
{
    currentDirectory = root;
}

//functia de schimbare a directorului curent
void Manager::changeDirectory(const std::string &name)
{
    if(name == "..")
    {
        if(currentDirectory->getParent())
        {
            currentDirectory = currentDirectory->getParent();
        }
        else
        {
            resetToRoot();
        }
        return;
    }

    Component* comp = currentDirectory->findComponent(name);
    if(comp && comp->isDirectory())
    {
        currentDirectory = static_cast<Directory*>(comp);
    }
    else
    {
        throw NotFoundException(name);
    }
}

//calea directorului curent
std::string Manager::getCurrentPath() const{
    std::string path = currentDirectory->getName();
    Directory* current = currentDirectory->getParent();

    while(current)
    {
        path = current->getName() + "/" + path;
        current = current->getParent();
    }

    return path;
}

//functie citire continut fisier
std::string Manager::readFile(const std::string& name) const{
    Component* component = findByName(name);
    if(!component->isDirectory())
    {
        File *file = dynamic_cast<File*>(component);
        if (file) 
        {
            return file->readContent();
        } 
        else 
        {
            throw Exception("Cannot read content of binary files (images/videos)");
        }
    }
    else
    {
        throw NotFoundException(name);
    }
}

//getter componenta
Component* Manager::getComponent(const std::string& name) const
{
    return findByName(name);
}



