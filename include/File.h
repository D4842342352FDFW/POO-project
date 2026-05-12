#pragma once 
#include "../include/Component.h"
#include <string>
class File : public Component{
    protected:
    //date text file
    std::string data;
    size_t fileSize;

    public:

    //constructor
    File(const std::string& name, const std::string& content);

    //regula celor 3
    virtual ~File() = default;
    File(const File& other);
    File& operator=(const File& other) = default;

    //functia de clone
    std::shared_ptr<Component> clone() const override;

    //functia afisare date
    void display(int depth) const override;
    std::string getTypeName() const override;
    std::vector<std::string> getMetadataLines() const override;

    //getter de dimensiune
    size_t getSize() const override;

    //fisier, nu folder
    bool isDirectory() const override { return false; }
    //setter nume 
    void setName(const std::string& name);

    //functii de citire continut fisiere
    virtual std::string readContent() const;
    bool supportsReadableContent() const override;
    std::string getDisplayContent() const override;
    std::string getReadableContent() const override;
    void setReadableContent(const std::string& content) override;

    //continut logic pentru persistenta
    std::string getPersistentContent() override;
};