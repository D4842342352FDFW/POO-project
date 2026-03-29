#include "../include/ImageFile.h"
#include <iostream>
#include <string>

//coonstructor 
ImageFile::ImageFile(const std::string& name, const int width, const int height, 
    const int bpp, const std::string& format) : Component(name), 
    width(width), height(height), bpp(bpp), format(format)
    {
        this->imageSize = width * height * bpp / 8;
    }


//copy consturtor 
ImageFile::ImageFile(const ImageFile& other) : Component(other.getName()), width(other.width), height(other.height),
bpp(other.bpp), format(other.format), imageSize(other.imageSize){}

//suprascrierea operatorului de atribuire
ImageFile& ImageFile::operator=(const ImageFile& other)
{
    if(this != &other)
    {
        this->name = other.name;
        this->width = other.width;
        this->height = other.height;
        this->bpp = other.bpp;
        this->format = other.format;
        this->imageSize = other.imageSize;
    }
    return *this;
}

//functia de clone
Component* ImageFile::clone() const
{
    return new ImageFile(*this);
}

//functia de afisare date imagine
void ImageFile::display(int depth) const{
    for(int i = 0; i < depth; ++i)
        std::cout <<" ";
    std::string time = ctime(&timestamp);
    time.pop_back();
    std::cout<< time << " -> Image  " << name << " [ " << width << "x" << height << " " << format << " ] has " << imageSize << " bytes\n";
}

//getter de dimensiune imagine
size_t ImageFile::getSize() const
{
    return imageSize;
}

//setter nume
void ImageFile::setName(const std::string& name)
{
    this->name = name;
}



