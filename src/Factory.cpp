#include "../include/Factory.h"
#include "../include/File.h"
#include "../include/ImageFile.h"
#include "../include/VideoFile.h"
#include "../include/EncryptedFile.h"
#include "../include/CompressedFile.h"
#include "../include/SecureArchive.h"
#include "../include/Directory.h"

std::shared_ptr<Component> ConcreteFileFactory::createTextFile(const std::string& name, const std::string& content)
{
    return std::make_shared<File>(name, content);
}

std::shared_ptr<Component> ConcreteFileFactory::createImageFile(const std::string& name, int width, int height, int bpp)
{
    return std::make_shared<ImageFile>(name, width, height, bpp, "bmp");
}

std::shared_ptr<Component> ConcreteFileFactory::createVideoFile(const std::string& name, int width, int height, int bpp, int fps, double duration)
{
    return std::make_shared<VideoFile>(name, width, height, bpp, fps, duration, "gif");
}

std::shared_ptr<Component> ConcreteFileFactory::createEncryptedFile(const std::string& name, const std::string& content)
{
    return std::make_shared<EncryptedFile>(name, content);
}

std::shared_ptr<Component> ConcreteFileFactory::createCompressedFile(const std::string& name, const std::string& content)
{
    return std::make_shared<CompressedFile>(name, content);
}

std::shared_ptr<Component> ConcreteFileFactory::createSecureArchive(const std::string& name, const std::string& content)
{
    return std::make_shared<SecureArchive>(name, content);
}

std::shared_ptr<Component> ConcreteDirectoryFactory::createDirectory(const std::string& name)
{
    return std::make_shared<Directory>(name);
}