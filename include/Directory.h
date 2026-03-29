#pragma once 
#include "../include/Component.h"
#include <vector>
#include <string>

class Directory : public Component{

    //date director
    std::vector<Component*> children;

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
    Component* clone() const override;

    //afisare date director
    void display(int depth) const override;

    //getter size + copii 
    size_t getSize() const override;
    const std::vector<Component*>& getChildren() const;

    //setter nume
    void setName(const std::string& name);

    //functii de adaugare stergere si cautare de componenta in 
    //in structura arborescenta
    void addComponent(Component* component);
    void removeComponent(const std::string& name);
    Component* findComponent(const std::string& name) const;
};