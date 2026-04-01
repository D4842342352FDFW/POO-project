#pragma once 
#include <string>
#include <ctime>
#include <cstddef>

class Directory;

class Component{
    protected:
    //date componenta
    static int instanceCount;
    std::string name;
    size_t size;
    time_t timestamp;
    Directory* parent;

    public:
    //constructor + destructor componenta 
    Component(const std::string& name);
   
    //regula celor 3
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    virtual ~Component();

    //functie de copiere componenta
    virtual Component* clone() const = 0;

    //afisare date componenta 
    virtual void display(int depth) const = 0;

    //gettere
    static int getInstanceCount();
    virtual size_t getSize() const = 0;
    std::string getName() const;
    Directory* getParent() const;
    
    //director sau fisier
    virtual bool isDirectory() const = 0;
    
    //setter parent 
    void setParent(Directory* newParent);
};