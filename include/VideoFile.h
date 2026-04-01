#pragma once 
#include "../include/Component.h"
#include <string>

class VideoFile : public Component{
    protected:
    //date imagine
    int width;
    int height;
    int bpp;
    int fps;
    double duration;
    std::string format;
    size_t videoSize;

    public:

    //constructor
    VideoFile(const std::string& name, const int width, const int height, const int bpp,
         const int fps,const double duration, const std::string& format);

    //copy constructor si operator =
    VideoFile(const VideoFile& other);
    VideoFile& operator=(const VideoFile& other);

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