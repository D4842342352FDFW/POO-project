#pragma once

#include "Component.h"
#include <memory>
#include <string>

//interfata abstracta pentru fabrica
class Factory
{
public:
    virtual ~Factory() = default;
};

class ConcreteFileFactory : public Factory
{
public:
    ~ConcreteFileFactory() override = default;

    std::shared_ptr<Component> createTextFile(const std::string& name, const std::string& content);
    std::shared_ptr<Component> createImageFile(const std::string& name, int width, int height, int bpp);
    std::shared_ptr<Component> createVideoFile(const std::string& name, int width, int height, int bpp, int fps, double duration);
    std::shared_ptr<Component> createEncryptedFile(const std::string& name, const std::string& content);
    std::shared_ptr<Component> createCompressedFile(const std::string& name, const std::string& content);
    std::shared_ptr<Component> createSecureArchive(const std::string& name, const std::string& content);
};

class ConcreteDirectoryFactory : public Factory
{
public:
    ~ConcreteDirectoryFactory() override = default;

    std::shared_ptr<Component> createDirectory(const std::string& name);
};