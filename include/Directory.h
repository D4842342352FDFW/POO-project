#pragma once 
#include "../include/Component.h"
#include <vector>
#include <string>
#include <memory>

class Directory : public Component, public std::enable_shared_from_this<Directory>{

    //date director
    std::vector<std::shared_ptr<Component>> children;

    //functii recursive de clear si copy
    void clear();
    void copyFrom(const Directory& other);

    public:

    //constructor director
    Directory(const std::string& name);

    //regula celor 3
    virtual ~Directory();
    Directory(const Directory& other);
    Directory& operator=(const Directory& other);

    //functie clone director
    std::shared_ptr<Component> clone() const override;

    //afisare date director
    void display(int depth) const override;
    std::string getTypeName() const override;
    std::vector<std::string> getMetadataLines() const override;

    //getter size + copii 
    size_t getSize() const override;

    //folder, nu fisier
    bool isDirectory() const override { return true; }
    
    //getter copii
    const std::vector<std::shared_ptr<Component>>& getChildren() const;

    //setter nume
    void setName(const std::string& name);

    //functii de adaugare stergere si cautare de componenta in 
    //in structura arborescenta
    void addComponent(const std::shared_ptr<Component>& component);
    void removeComponent(const std::string& name);
    std::shared_ptr<Component> findComponent(const std::string& name) const;
};