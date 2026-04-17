#pragma once 
#include <string>
#include <ctime>
#include <cstddef>
#include <vector>

class Directory;

class Component{
    protected:
    //date componenta
    static int instanceCount;
    std::string name;
    size_t size;
    time_t timestamp;
    std::string storagePath;
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

    //metadata polimorfa pentru partea de UI
    virtual std::string getTypeName() const = 0;
    virtual std::vector<std::string> getMetadataLines() const;
    virtual bool supportsReadableContent() const;
    virtual std::string getDisplayContent() const;
    virtual std::string getReadableContent() const;

    //setter pentru continutul text al componentei
    virtual void setReadableContent(const std::string& content);

    //continutul logic folosit la persistenta pe disc
    virtual std::string getPersistentContent();

    //gettere
    static int getInstanceCount();
    virtual size_t getSize() const = 0;
    std::string getName() const;
    Directory* getParent() const;
    time_t getTimestamp() const;
    std::string getStoragePath() const;
    
    //director sau fisier
    virtual bool isDirectory() const = 0;
    
    //setter parent 
    void setParent(Directory* newParent);
    void setTimestamp(time_t newTimestamp);
    void setStoragePath(const std::string& path);
};