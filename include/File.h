#include "../include/Component.h"
#include <string_view>

class File : public Component{
    protected:
    //date text file
    char* data;
    size_t fileSize;

    //functii de clear si copy
    void freeResource();
    void copyFrom(const File& other);

    public:

    //constructor
    File(const std::string& name, const std::string& content);

    //regula celor 3
    virtual ~File();
    File(const File& other);
    File& operator=(const File& other);

    //functia de clone
    Component* clone() const override;

    //functia afisare date
    void display(int depth) const override;

    //getter de dimensiune
    size_t getSize() const override;

    //setter nume 
    void setName(const std::string& name);
    
    //functii de citire continut fisiere
    virtual std::string readContent() const;
};