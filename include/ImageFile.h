#pragma once 
#include "../include/Component.h"
#include <string>

class ImageFile : public Component{
    protected:
    //date imagine
    int width;
    int height;
    int bpp;
    std::string format;
    size_t imageSize;

    public:

    //constructor
    ImageFile(const std::string& name, const int width, const int height, const int bpp, const std::string& format);

    //copy constructor si operator =
    ImageFile(const ImageFile& other);
    ImageFile& operator=(const ImageFile& other);

    //functia de clone
    Component* clone() const override;

    //functia afisare date
    void display(int depth) const override;

    //fisier, nu folder
    bool isDirectory() const override { return false; }

    //getter de dimensiune
    size_t getSize() const override;

    //setter nume 
    void setName(const std::string& name);
};