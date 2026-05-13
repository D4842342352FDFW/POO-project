#include "../include/VideoFile.h"
#include <iostream>
#include <string>

//coonstructor 
VideoFile::VideoFile(const std::string& name, const int width, const int height, 
    const int bpp, const int fps, const double duration, const std::string& format) : Component(name), 
    width(width), height(height), bpp(bpp), fps(fps), duration(duration), format(format)
    {
        this->videoSize = width * height * bpp / 8 * fps * duration;
    }


//copy consturtor 
VideoFile::VideoFile(const VideoFile& other) : Component(other.getName()), width(other.width), height(other.height),
bpp(other.bpp), fps(other.fps), duration(other.duration), format(other.format), videoSize(other.videoSize){}

//suprascrierea operatorului de atribuire
VideoFile& VideoFile::operator=(const VideoFile& other)
{
    if(this != &other)
    {
        this->name = other.name;
        this->width = other.width;
        this->height = other.height;
        this->bpp = other.bpp;
        this->fps = other.fps;
        this->duration = other.duration;
        this->format = other.format;
        this->videoSize = other.videoSize;
    }
    return *this;
}

//functia de clone
std::shared_ptr<Component> VideoFile::clone() const
{
    return std::make_shared<VideoFile>(*this);
}

//functia de afisare date imagine
void VideoFile::display(int depth) const{
    for(int i = 0; i < depth; ++i)
        std::cout <<" ";
    std::string time = ctime(&timestamp);
    time.pop_back();
    std::cout<< time << " -> Video  " << name << " [ " << width << "x" << height << " " << format << " ] " 
    << " ( " << duration << " s , " << fps << "FPS ) " << videoSize << " bytes\n";
}

std::string VideoFile::getTypeName() const
{
    return "Video File";
}

std::vector<std::string> VideoFile::getMetadataLines() const
{
    return {
        "resolution: " + std::to_string(width) + "x" + std::to_string(height),
        "bpp: " + std::to_string(bpp),
        "fps: " + std::to_string(fps),
        "duration: " + std::to_string(duration) + "s",
        "format: " + format
    };
}

//getter de dimensiune imagine
size_t VideoFile::getSize() const
{
    return videoSize;
}

//setter nume
void VideoFile::setName(const std::string& name)
{
    this->name = name;
}

//fisier video gol
std::string VideoFile::getPersistentContent()
{
    return "";
}



    