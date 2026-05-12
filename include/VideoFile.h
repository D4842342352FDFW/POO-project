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
    int getFps() const { return fps; }
    double getDuration() const { return duration; }
    std::string getFormat() const { return format; }

    //setter nume 
    void setName(const std::string& name);
};