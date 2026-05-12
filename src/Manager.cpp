#include "../include/Manager.h"
#include "../include/Directory.h"
#include "../include/File.h"
#include "../include/ImageFile.h"
#include "../include/VideoFile.h"
#include "../include/EncryptedFile.h"
#include "../include/CompressedFile.h"
#include "../include/SecureArchive.h"
#include "../include/FileFactory.h"
#include "../include/Exception.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <functional>
#include <utility>
#include <algorithm>
#include <cctype>

namespace
{
//citire continut din fisier fizic
std::string readDiskFile(const std::string& path)
{
    if(path.empty())
    {
        return "";
    }

    std::ifstream in(path, std::ios::binary);
    if(!in.is_open())
    {
        return "";
    }

    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

//scriere continut in fisier fizic
void writeDiskFile(const std::string& path, const std::string& content)
{
    std::filesystem::path diskPath(path);
    if(!diskPath.parent_path().empty())
    {
        std::filesystem::create_directories(diskPath.parent_path());
    }

    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if(!out.is_open())
    {
        throw Exception("Cannot write disk file: " + path);
    }

    out.write(content.data(), static_cast<std::streamsize>(content.size()));
}

std::string toLower(const std::string& value)
{
    std::string result = value;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    return result;
}

std::string normalizeFileNameWithExtension(const std::string& name, int type, [[maybe_unused]] const std::string& format)
{
    std::filesystem::path p(name);

    if(type == 1)
    {
        if(!p.has_extension())
        {
            p.replace_extension(".txt");
        }
        return p.string();
    }

    if(type == 2)
    {
        //imaginile sunt exportate ca bmp momentan sper sa pot sa fac o implementare de generare poza
        //in functie de o descriere cu ai
        p.replace_extension(".bmp");
        return p.string();
    }

    if(type == 3)
    {
        //video este persistat ca gif momentan sper sa pot sa fac o implementare de generare poza
        //in functie de o descriere cu ai
        p.replace_extension(".gif");
        return p.string();
    }

    if(type == 4)
    {
        p.replace_extension(".enc");
        return p.string();
    }

    if(type == 5)
    {
        p.replace_extension(".zip");
        return p.string();
    }

    if(type == 6)
    {
        p.replace_extension(".rar");
        return p.string();
    }

    return name;
}
}

//initializare instanta statica
std::shared_ptr<Manager> Manager::instance = nullptr;

//getter instanta
std::shared_ptr<Manager> Manager::getInstance()
{
    if(!instance)
    {
        instance = std::shared_ptr<Manager>(new Manager());
    }
    return instance;
}

void Manager::destroyInstance()
{
    instance.reset();
}

//constructor
Manager::Manager()
{
    storageRoot = "root";
    ensureStorageRoot();

    root = std::make_shared<Directory>("root");
    currentDirectory = root;
    globalIndex.emplace("root", root);
    fileFactory = std::make_shared<ConcreteFileFactory>();

    //la pornire incercam sa incarcam direct din arborele real de pe disc
    loadFromDiskTree();
}

//eliminare din indexul global
void Manager::removeFromIndex(const std::shared_ptr<Component>& component)
{
    if(component == nullptr)
    {
        return;
    }

    //daca e folder eliminam recursiv toti copii
    if(component->isDirectory())
    {
        auto directory = std::dynamic_pointer_cast<Directory>(component);
        for(const auto& child : directory->getChildren())
        {
            removeFromIndex(child);
        }
    }

    //cautam si eliminam componenta specifica
    auto range = globalIndex.equal_range(component->getName());
    for(auto it = range.first; it != range.second; )
    {
        if(it->second == component)
        {
            it = globalIndex.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

//luam recursiv calea completa
std::string Manager::getComponentPath(const std::shared_ptr<Component>& component) const
{
    if(component == nullptr)
    {
        return "";
    }

    std::string path = component->getName();
    auto parent = component->getParent();

    while(parent)
    {
        path = parent->getName() + "/" + path;
        parent = parent->getParent();
    }

    return path;
}

//cautam componenta dupa nume initial in folderul curent
//si dupa global prin toata structura
std::shared_ptr<Component> Manager::findByName(const std::string& name) const
{
    auto fromCurrentDirectory = currentDirectory->findComponent(name);
    if(fromCurrentDirectory)
    {
        return fromCurrentDirectory;
    }

    auto range = globalIndex.equal_range(name);
    if(range.first == range.second)
    {
        throw NotFoundException(name);
    }

    //sa nu existe elemente cu acelasi nume
    auto next = range.first;
    ++next;
    if(next != range.second)
    {
        throw Exception("Multiple elements with the same name exist");
    }

    return range.first->second;
}

//creare folder de stocare pe disc daca lipseste
void Manager::ensureStorageRoot()
{
    std::filesystem::create_directories(storageRoot);
}

//refacere index global dupa incarcare
void Manager::rebuildIndex(const std::shared_ptr<Component>& component)
{
    if(component == nullptr)
    {
        return;
    }

    globalIndex.emplace(component->getName(), component);
    if(component->isDirectory())
    {
        auto directory = std::dynamic_pointer_cast<Directory>(component);
        for(const auto& child : directory->getChildren())
        {
            rebuildIndex(child);
        }
    }
}

//incarcare completa direct din structura fizica root
void Manager::loadFromDiskTree()
{
    if(!std::filesystem::exists(storageRoot) || !std::filesystem::is_directory(storageRoot))
    {
        return;
    }

    root = std::make_shared<Directory>("root");

    std::function<void(const std::shared_ptr<Directory>&, const std::filesystem::path&)> loadRecursive;
    loadRecursive = [&](const std::shared_ptr<Directory>& parent, const std::filesystem::path& currentPath)
    {
        for(const auto& entry : std::filesystem::directory_iterator(currentPath))
        {
            const std::string nodeName = entry.path().filename().string();
            if(entry.is_directory())
            {
                auto childDirectory = std::make_shared<Directory>(nodeName);
                parent->addComponent(childDirectory);
                loadRecursive(childDirectory, entry.path());
            }
            else
            {
                if(entry.is_regular_file())
                {
                    std::string content = readDiskFile(entry.path().string());
                    auto childFile = std::make_shared<File>(nodeName, content);
                    childFile->setStoragePath(entry.path().string());
                    parent->addComponent(childFile);
                }
            }
        }
    };

    loadRecursive(root, storageRoot);
    currentDirectory = root;
    globalIndex.clear();
    rebuildIndex(root);
}

//extrage continutul pentru fiecare tip de fisier
std::string Manager::getLogicalDataForDisk(const std::shared_ptr<Component>& component) const
{
    if(component == nullptr)
    {
        return "";
    }

    return component->getPersistentContent();
}

//stergere recursiva a fisierelor fizice asociate componentelor
void Manager::deletePhysicalData(const std::shared_ptr<Component>& component)
{
    if(component == nullptr)
    {
        return;
    }

    if(component->isDirectory())
    {
        auto directory = std::dynamic_pointer_cast<Directory>(component);
        for(const auto& child : directory->getChildren())
        {
            deletePhysicalData(child);
        }
        return;
    }

    const std::string storagePath = component->getStoragePath();
    if(storagePath.empty())
    {
        return;
    }

    std::error_code ec;
    std::filesystem::remove(storagePath, ec);
    if(ec)
    {
        throw Exception("Cannot delete disk file: " + storagePath);
    }
}

//sanitizare nume pentru fisiere/directoare reale pe disc
std::string Manager::sanitizeDiskName(const std::string& name) const
{
    std::string result = name;
    for(char& ch : result)
    {
        if(ch == '<' || ch == '>' || ch == ':' || ch == '"' || ch == '/' || ch == '\\' || ch == '|' || ch == '?' || ch == '*')
        {
            ch = '_';
        }
    }

    if(result.empty())
    {
        result = "unnamed";
    }

    return result;
}

//materializare recursiva a structurii VFS in foldere/fisiere reale
void Manager::materializeNodeOnDisk(const std::shared_ptr<Component>& component, const std::filesystem::path& parentPath) const
{
    if(component == nullptr)
    {
        return;
    }

    const std::filesystem::path nodePath = parentPath / sanitizeDiskName(component->getName());

    if(component->isDirectory())
    {
        std::filesystem::create_directories(nodePath);
        auto asDirectory = std::dynamic_pointer_cast<Directory>(component);
        for(const auto& child : asDirectory->getChildren())
        {
            materializeNodeOnDisk(child, nodePath);
        }
        return;
    }

    component->setStoragePath(nodePath.string());
    writeDiskFile(nodePath.string(), getLogicalDataForDisk(component));
}

//refacerea structurii reale in root
void Manager::materializeTreeOnDisk() const
{
    if(std::filesystem::exists(storageRoot))
    {
        std::filesystem::remove_all(storageRoot);
    }

    std::filesystem::create_directories(storageRoot);

    //storageRoot reprezinta deja directorul radacina logic, deci materializam doar copiii
    for(const auto& child : root->getChildren())
    {
        materializeNodeOnDisk(child, storageRoot);
    }
}

//crearea folder
void Manager::createDirectory(const std::string& name)
{
    if(currentDirectory->findComponent(name))
    {
        throw Exception("Element already exists in current directory");
    }

    auto newDir = fileFactory->createDirectory(name);
    currentDirectory->addComponent(newDir);
    globalIndex.emplace(name, newDir);

    //sincronizam imediat arborele real dupa creare
    materializeTreeOnDisk();
}

//creare fisier
void Manager::createFile(const std::string& name, const std::string& content, int type,
int width, int height, int bpp, int fps, double duration,
const std::string& format)
{
    std::shared_ptr<Component> newFile;
    const std::string normalizedName = normalizeFileNameWithExtension(name, type, format);

    if(currentDirectory->findComponent(normalizedName))
    {
        throw Exception("Element already exists in current directory");
    }

    if(type == 1)
        newFile = fileFactory->createTextFile(normalizedName, content);
    if(type == 2)
        newFile = fileFactory->createImageFile(normalizedName, width, height, bpp);
    if(type == 3)
        newFile = fileFactory->createVideoFile(normalizedName, width, height, bpp, fps, duration);
    if(type == 4)
        newFile = fileFactory->createEncryptedFile(normalizedName, content);
    if(type == 5)
        newFile = fileFactory->createCompressedFile(normalizedName, content);
    if(type == 6)
        newFile = fileFactory->createSecureArchive(normalizedName, content);

    if(newFile == nullptr)
    {
        throw Exception("Invalid file type");
    }

    newFile->setStoragePath("");

    currentDirectory->addComponent(newFile);
    globalIndex.emplace(normalizedName, newFile);

    //sincronizam imediat arborele real pentru a pastra o singura copie fizica
    materializeTreeOnDisk();
}

std::string Manager::convertFileType(const std::string& name, int targetType)
{
    if(targetType != 1 && targetType != 4 && targetType != 5 && targetType != 6)
    {
        throw Exception("Invalid conversion target type");
    }

    auto component = currentDirectory->findComponent(name);
    if(component == nullptr)
    {
        throw NotFoundException(name);
    }

    if(component->isDirectory())
    {
        throw Exception("Cannot convert a directory");
    }

    if(std::dynamic_pointer_cast<ImageFile>(component) || std::dynamic_pointer_cast<VideoFile>(component))
    {
        throw Exception("Conversion is available only for text-based files");
    }

    auto parent = component->getParent();
    if(parent == nullptr)
    {
        throw Exception("Invalid file parent");
    }

    std::string plainContent;
    if(auto secureArchive = std::dynamic_pointer_cast<SecureArchive>(component))
    {
        plainContent = secureArchive->readDecryptedAndDecompressedContentContent();
    }
    else
    {
        if(auto encryptedFile = std::dynamic_pointer_cast<EncryptedFile>(component))
        {
            plainContent = encryptedFile->readDecryptedContent();
        }
        else
        {
            if(auto compressedFile = std::dynamic_pointer_cast<CompressedFile>(component))
            {
                plainContent = compressedFile->readDecompressedContent();
            }
            else
            {
                auto normalFile = std::dynamic_pointer_cast<File>(component);
                if(normalFile == nullptr)
                {
                    throw Exception("Unsupported file type for conversion");
                }
                plainContent = normalFile->readContent();
            }
        }
    }

    std::filesystem::path oldPath(component->getName());
    std::string baseName = oldPath.stem().string();
    if(baseName.empty())
    {
        baseName = oldPath.filename().string();
    }

    const std::string newName = normalizeFileNameWithExtension(baseName, targetType, "");
    auto existingWithNewName = parent->findComponent(newName);
    if(existingWithNewName && existingWithNewName != component)
    {
        throw Exception("Element already exists in current directory");
    }

    std::shared_ptr<Component> convertedFile;
    if(targetType == 1)
    {
        convertedFile = fileFactory->createTextFile(newName, plainContent);
    }
    if(targetType == 4)
    {
        convertedFile = fileFactory->createEncryptedFile(newName, plainContent);
    }
    if(targetType == 5)
    {
        convertedFile = fileFactory->createCompressedFile(newName, plainContent);
    }
    if(targetType == 6)
    {
        convertedFile = fileFactory->createSecureArchive(newName, plainContent);
    }

    if(convertedFile == nullptr)
    {
        throw Exception("Conversion failed");
    }

    removeFromIndex(component);
    parent->removeComponent(component->getName());

    convertedFile->setStoragePath("");
    parent->addComponent(convertedFile);
    globalIndex.emplace(newName, convertedFile);

    materializeTreeOnDisk();
    return newName;
}

//stergere componenta din directorul curent si din index
void Manager::deleteComponent(const std::string& name)
{
    auto comp = currentDirectory->findComponent(name);
    if(comp)
    {
        if(comp == root)
        {
            throw Exception("Cannot delete root directory");
        }

        auto parent = comp->getParent();
        if(parent)
        {
            //stergere date fizice inainte de eliberarea memoriei
            deletePhysicalData(comp);
            removeFromIndex(comp);
            parent->removeComponent(name);

            //actualizare imediata a arborelui fizic de pe disc
            materializeTreeOnDisk();
        }
    }
    else
    {
        throw NotFoundException(name);
    }
}

//afisare arbore directoare + fisiere
void Manager::displayTree() const{
    std::cout << "\n------------------------------------------\n";
    root->display(0);
    std::cout << "------------------------------------------\n";
}

std::vector<std::string> Manager::getTreeLines() const
{
    std::vector<std::string> lines;
    if(root == nullptr)
    {
        lines.push_back("(empty)");
        return lines;
    }

    std::function<void(const std::shared_ptr<Component>&, int)> collect;
    collect = [&](const std::shared_ptr<Component>& node, int depth)
    {
        if(node == nullptr)
        {
            return;
        }

        std::string indent(static_cast<size_t>(depth) * 2, ' ');
        const std::string marker = node->isDirectory() ? "[D] " : "[F] ";
        lines.push_back(indent + marker + node->getName());

        if(node->isDirectory())
        {
            auto directory = std::dynamic_pointer_cast<Directory>(node);
            for(const auto& child : directory->getChildren())
            {
                collect(child, depth + 1);
            }
        }
    };

    collect(root, 0);
    return lines;
}

std::vector<std::string> Manager::getCurrentDirectoryListing(const std::string& sortBy, bool ascending, const std::string& typeFilter) const
{
    std::vector<std::shared_ptr<Component>> items;
    for(const auto& child : currentDirectory->getChildren())
    {
        if(child == nullptr)
        {
            continue;
        }

        const std::string normalizedFilter = toLower(typeFilter);
        if(!normalizedFilter.empty())
        {
            const std::string normalizedType = toLower(child->getTypeName());
            if(normalizedFilter == "dir" || normalizedFilter == "directory")
            {
                if(!child->isDirectory())
                {
                    continue;
                }
            }
            else
            {
                if(normalizedFilter == "file" && child->isDirectory())
                {
                    continue;
                }
                if(normalizedFilter != "file" && normalizedType.find(normalizedFilter) == std::string::npos)
                {
                    continue;
                }
            }
        }

        items.push_back(child);
    }

    const std::string normalizedSort = toLower(sortBy);
    std::sort(items.begin(), items.end(), [&](const std::shared_ptr<Component>& a, const std::shared_ptr<Component>& b)
    {
        if(normalizedSort == "size")
        {
            if(a->getSize() == b->getSize())
            {
                return a->getName() < b->getName();
            }
            return a->getSize() < b->getSize();
        }
        if(normalizedSort == "time" || normalizedSort == "timestamp")
        {
            if(a->getTimestamp() == b->getTimestamp())
            {
                return a->getName() < b->getName();
            }
            return a->getTimestamp() < b->getTimestamp();
        }
        return a->getName() < b->getName();
    });

    if(!ascending)
    {
        std::reverse(items.begin(), items.end());
    }

    std::vector<std::string> lines;
    lines.reserve(items.size());
    for(auto item : items)
    {
        const std::string marker = item->isDirectory() ? "[D]" : "[F]";
        lines.push_back(marker + std::string(" ") + item->getName() + " | " + item->getTypeName() + " | " + std::to_string(item->getSize()) + " bytes");
    }

    return lines;
}

std::string Manager::getComponentDetails(const std::string& name) const
{
    auto component = findByName(name);
    if(component == nullptr)
    {
        throw NotFoundException(name);
    }

    std::string details;
    details += "Name: " + component->getName();
    details += "\nType: " + component->getTypeName();
    details += "\nPath: " + getComponentPath(component);
    details += "\nSize: " + std::to_string(component->getSize()) + " bytes";
    details += std::string("\nReadable content: ") + (component->supportsReadableContent() ? "yes" : "no");

    const std::vector<std::string> metadata = component->getMetadataLines();
    if(!metadata.empty())
    {
        details += "\nMetadata:";
        for(const auto& line : metadata)
        {
            details += "\n  - " + line;
        }
    }

    return details;
}

std::string Manager::getCurrentDirectoryStats() const
{
    size_t files = 0;
    size_t directories = 0;
    size_t readableFiles = 0;
    size_t totalSize = 0;

    std::unordered_map<std::string, size_t> typeCounts;
    for(const auto& child : currentDirectory->getChildren())
    {
        if(child == nullptr)
        {
            continue;
        }

        totalSize += child->getSize();
        if(child->isDirectory())
        {
            directories++;
        }
        else
        {
            files++;
            if(child->supportsReadableContent())
            {
                readableFiles++;
            }
        }

        typeCounts[child->getTypeName()]++;
    }

    std::string result;
    result += "Directory stats for " + getCurrentPath();
    result += "\nDirectories: " + std::to_string(directories);
    result += "\nFiles: " + std::to_string(files);
    result += "\nReadable files: " + std::to_string(readableFiles);
    result += "\nTotal child size: " + std::to_string(totalSize) + " bytes";

    if(!typeCounts.empty())
    {
        result += "\nType breakdown:";
        for(const auto& [typeName, count] : typeCounts)
        {
            result += "\n  - " + typeName + ": " + std::to_string(count);
        }
    }

    return result;
}

//cautare si afisare detalii
void Manager::search(const std::string& name) const{
    auto range = globalIndex.equal_range(name);
    if(range.first != range.second)
    {
        for(auto it = range.first; it != range.second; ++it)
        {
            std::cout << "Found " << getComponentPath(it->second) << " | Size: " << it->second->getSize() << " bytes\n";
        }
    }
    else
    {
        throw NotFoundException(name);
    }
}

//intoarce calea absoluta pe disc pentru componenta cautata
std::string Manager::getAbsoluteDiskPath(const std::string& name) const
{
    auto component = findByName(name);
    if(component == nullptr)
    {
        throw NotFoundException(name);
    }

    if(!component->isDirectory() && !component->getStoragePath().empty())
    {
        return std::filesystem::absolute(component->getStoragePath()).string();
    }

    std::string relativePath = getComponentPath(component);
    const std::string rootPrefix = root->getName() + "/";
    if(relativePath == root->getName())
    {
        relativePath.clear();
    }
    else
    {
        if(relativePath.rfind(rootPrefix, 0) == 0)
        {
            relativePath = relativePath.substr(rootPrefix.size());
        }
    }

    const std::filesystem::path absolutePath = relativePath.empty()
        ? std::filesystem::absolute(storageRoot)
        : std::filesystem::absolute(storageRoot / relativePath);
    return absolutePath.string();
}

//revenire la radacina
void Manager::resetToRoot()
{
    currentDirectory = root;
}

//setter director curent (folosit de GUI)
void Manager::setCurrentDirectory(const std::shared_ptr<Directory>& directory)
{
    if(directory == nullptr)
    {
        currentDirectory = root;
        return;
    }

    currentDirectory = directory;
}

//getter radacina pentru tree view in GUI
std::shared_ptr<Directory> Manager::getRoot() const
{
    return root;
}

//getter director curent pentru contextul de creare
std::shared_ptr<Directory> Manager::getCurrentDirectory() const
{
    return currentDirectory;
}

//functia de schimbare a directorului curent
void Manager::changeDirectory(const std::string &name)
{
    if(name == "..")
    {
        if(currentDirectory->getParent())
        {
            currentDirectory = currentDirectory->getParent();
        }
        else
        {
            resetToRoot();
        }
        return;
    }

    auto comp = currentDirectory->findComponent(name);
    if(comp && comp->isDirectory())
    {
        currentDirectory = std::dynamic_pointer_cast<Directory>(comp);
    }
    else
    {
        throw NotFoundException(name);
    }
}

//calea directorului curent
std::string Manager::getCurrentPath() const{
    std::string path = currentDirectory->getName();
    auto current = currentDirectory->getParent();

    while(current)
    {
        path = current->getName() + "/" + path;
        current = current->getParent();
    }

    return path;
}

//functie citire continut fisier
std::string Manager::readFile(const std::string& name) const{
    auto component = findByName(name);
    if(component->isDirectory())
    {
        throw NotFoundException(name);
    }

    if(!component->supportsReadableContent())
    {
        throw Exception("Cannot read content of this file type");
    }

    return component->getDisplayContent();
}

void Manager::updateFileContent(const std::string& name, const std::string& content)
{
    auto component = findByName(name);
    if(component->isDirectory())
    {
        throw Exception("Cannot edit directory content");
    }

    if(!component->supportsReadableContent())
    {
        throw Exception("Cannot edit content of this file type");
    }

    component->setReadableContent(content);
    component->setTimestamp(time(nullptr));
    materializeTreeOnDisk();
}

//getter componenta
std::shared_ptr<Component> Manager::getComponent(const std::string& name) const
{
    return findByName(name);
}
