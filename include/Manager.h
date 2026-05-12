#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>
#include <vector>
#include <memory>

class Component;
class Directory;
class FileFactory;

class Manager{

    //pointeri pentru radacina si folderul curent
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> currentDirectory;

    //index global => cautare rapida
    std::unordered_multimap<std::string, std::shared_ptr<Component>> globalIndex;
    std::filesystem::path storageRoot;
    std::shared_ptr<FileFactory> fileFactory;

    //functii de ajutor
    void removeFromIndex(const std::shared_ptr<Component>& component);
    std::string getComponentPath(const std::shared_ptr<Component>& component) const;
    std::shared_ptr<Component> findByName(const std::string& name) const;
    void rebuildIndex(const std::shared_ptr<Component>& component);
    void ensureStorageRoot();
    void loadFromDiskTree();
    std::string getLogicalDataForDisk(const std::shared_ptr<Component>& component) const;
    void deletePhysicalData(const std::shared_ptr<Component>& component);
    std::string sanitizeDiskName(const std::string& name) const;
    void materializeTreeOnDisk() const;
    void materializeNodeOnDisk(const std::shared_ptr<Component>& component, const std::filesystem::path& parentPath) const;
    
    //constructor privat
    Manager();
    static std::shared_ptr<Manager> instance;

    public:

    //dezactivarea constructorului de compiere si a operatorului de atribuire
    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;

    //destructor
    ~Manager() = default;
    
    //metode statice pentru gestionarea singletonului
    static std::shared_ptr<Manager> getInstance();
    static void destroyInstance();
    
    //functii de manipulare 
    void createDirectory(const std::string& name);
    void createFile(const std::string& name, const std::string& content, int type, 
    int width = 0, int height = 0, int bpp = 0, int fps = 0, double duration = 0.0, const std::string& format = "");
    std::string convertFileType(const std::string& name, int targetType);
    void deleteComponent(const std::string& name);
    std::shared_ptr<Component> getComponent(const std::string& name) const;

    //functia de citire continut fisier
    std::string readFile(const std::string& name) const;
    void updateFileContent(const std::string& name, const std::string& content);

    //partea de navigare
    void changeDirectory(const std::string& name);

    //afisare arbore
    void displayTree() const;
    std::vector<std::string> getTreeLines() const;
    std::vector<std::string> getCurrentDirectoryListing(const std::string& sortBy, bool ascending, const std::string& typeFilter) const;
    std::string getComponentDetails(const std::string& name) const;
    std::string getCurrentDirectoryStats() const;
    
    //parte de cautare dupa nume
    void search(const std::string& name) const;
    std::string getAbsoluteDiskPath(const std::string& name) const;

    //functii pentru starea curenta
    std::string getCurrentPath() const;
    void resetToRoot();
    void setCurrentDirectory(const std::shared_ptr<Directory>& directory);

    //gettere utile pentru interfata grafica
    std::shared_ptr<Directory> getRoot() const;
    std::shared_ptr<Directory> getCurrentDirectory() const;
};
