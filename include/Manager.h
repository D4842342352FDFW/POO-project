#pragma once

#include "../include/Directory.h"
#include "../include/File.h"
#include "../include/ImageFile.h"
#include "../include/VideoFile.h"
#include "../include/EncryptedFile.h"
#include "../include/CompressedFile.h"
#include "../include/SecureArchive.h"
#include <unordered_map>
#include <string>
#include <memory>

class Directory;

class Manager{

    //pointeri pentru radacina si folderul curent
    Directory* root;
    Directory* currentDirectory;

    //index global => cautare rapida
    std::unordered_multimap<std::string, Component*> globalIndex;

    //functii de ajutor
    void removeFromIndex(Component* component);
    std::string getComponentPath(Component* component) const;
    Component* findByName(const std::string& name) const;
    
    //constructor privat
    Manager();
    static Manager* instance;

    public:

    //dezactivarea constructorului de compiere si a operatorului de atribuire
    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;

    //destructor
    ~Manager();
    
    //metode statice pentru gestionarea singletonului
    static Manager* getInstance();
    static void destroyInstance();
    
    //functii de manipulare 
    void createDirectory(const std::string& name);
    void createFile(const std::string& name, const std::string& content, int type, 
    int width = 0, int height = 0, int bpp = 0, int fps = 0, double duration = 0.0, const std::string& format = "");
    void deleteComponent(const std::string& name);
    Component* getComponent(const std::string& name) const;

    //functia de citire continut fisier
    std::string readFile(const std::string& name) const;

    //partea de navigare
    void changeDirectory(const std::string& name);

    //afisare arbore
    void displayTree() const;
    
    //parte de cautare dupa nume
    void search(const std::string& name) const;

    //functii pentru starea curenta
    std::string getCurrentPath() const;
    void resetToRoot();
};
