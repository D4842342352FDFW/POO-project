#include "../include/FileFactory.h"
#include "../include/File.h"
#include "../include/ImageFile.h"
#include "../include/VideoFile.h"
#include "../include/EncryptedFile.h"
#include "../include/CompressedFile.h"
#include "../include/SecureArchive.h"
#include "../include/Directory.h"

//creare fisier text
std::shared_ptr<Component> ConcreteFileFactory::createTextFile(const std::string& name, const std::string& content)
{
    return std::make_shared<File>(name, content);
}

//creare fisier imagine
std::shared_ptr<Component> ConcreteFileFactory::createImageFile(const std::string& name, int width, int height, int bpp)
{
    return std::make_shared<ImageFile>(name, width, height, bpp, "bmp");
}

//creare fisier video
std::shared_ptr<Component> ConcreteFileFactory::createVideoFile(const std::string& name, int width, int height, int bpp, int fps, double duration)
{
    return std::make_shared<VideoFile>(name, width, height, bpp, fps, duration, "gif");
}

//creare fisier criptat
std::shared_ptr<Component> ConcreteFileFactory::createEncryptedFile(const std::string& name, const std::string& content)
{
    return std::make_shared<EncryptedFile>(name, content);
}

//creare fisier comprimat
std::shared_ptr<Component> ConcreteFileFactory::createCompressedFile(const std::string& name, const std::string& content)
{
    return std::make_shared<CompressedFile>(name, content);
}

//creare arhiva
std::shared_ptr<Component> ConcreteFileFactory::createSecureArchive(const std::string& name, const std::string& content)
{
    return std::make_shared<SecureArchive>(name, content);
}

//creare folder
std::shared_ptr<Component> ConcreteFileFactory::createDirectory(const std::string& name)
{
    return std::make_shared<Directory>(name);
}
