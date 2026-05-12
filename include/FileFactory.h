#pragma once

#include "Component.h"
#include <string>
#include <memory>

//interfata abstracta pentru fabrica de fisiere
class FileFactory
{
public:
    //destructor virtual
    virtual ~FileFactory() = default;

    //metode virtuale de creare pentru fiecare tip de fisier
    virtual std::shared_ptr<Component> createTextFile(const std::string& name, const std::string& content) = 0;
    virtual std::shared_ptr<Component> createImageFile(const std::string& name, int width, int height, int bpp) = 0;
    virtual std::shared_ptr<Component> createVideoFile(const std::string& name, int width, int height, int bpp, int fps, double duration) = 0;
    virtual std::shared_ptr<Component> createEncryptedFile(const std::string& name, const std::string& content) = 0;
    virtual std::shared_ptr<Component> createCompressedFile(const std::string& name, const std::string& content) = 0;
    virtual std::shared_ptr<Component> createSecureArchive(const std::string& name, const std::string& content) = 0;
    
    //metoda virtuala pentru creare folder
    virtual std::shared_ptr<Component> createDirectory(const std::string& name) = 0;
};

//implementarea concreta a fabricii de fisiere
class ConcreteFileFactory : public FileFactory
{
public:
    //destructor
    ~ConcreteFileFactory() override = default;

    //implementari ale metodelor de creare
    std::shared_ptr<Component> createTextFile(const std::string& name, const std::string& content) override;
    std::shared_ptr<Component> createImageFile(const std::string& name, int width, int height, int bpp) override;
    std::shared_ptr<Component> createVideoFile(const std::string& name, int width, int height, int bpp, int fps, double duration) override;
    std::shared_ptr<Component> createEncryptedFile(const std::string& name, const std::string& content) override;
    std::shared_ptr<Component> createCompressedFile(const std::string& name, const std::string& content) override;
    std::shared_ptr<Component> createSecureArchive(const std::string& name, const std::string& content) override;
    
    //implementare creare folder
    std::shared_ptr<Component> createDirectory(const std::string& name) override;
};
