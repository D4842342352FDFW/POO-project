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
    std::shared_ptr<Component> clone() const override;

    //functia afisare date
    void display(int depth) const override;
    std::string getTypeName() const override;
    std::vector<std::string> getMetadataLines() const override;
    std::string getPersistentContent() override;

    //fisier, nu folder
    bool isDirectory() const override { return false; }

    //getter de dimensiune
    size_t getSize() const override;
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getBitsPerPixel() const { return bpp; }
    std::string getFormat() const { return format; }

    //setter nume 
    void setName(const std::string& name);
};