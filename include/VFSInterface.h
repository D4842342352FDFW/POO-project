#pragma once

#include "../include/Manager.h"
#include "../include/Component.h"
#include <string>
#include <vector>
#include <memory>

class VFSInterface
{
    //manager-ul aplicatiei
    std::shared_ptr<Manager> manager;

    //starea UI
    std::shared_ptr<Component> selectedComponent;
    bool showContentPreview;

    //starea pentru create folder
    std::string newFolderName;

    //starea pentru create file
    std::string newFileName;
    std::string newFileContent;
    int newFileType;
    int imageWidth;
    int imageHeight;
    int imageBpp;
    int videoWidth;
    int videoHeight;
    int videoBpp;
    int videoFps;
    double videoDuration;
    std::string mediaFormat;

    //starea pentru search
    std::string searchName;

    //stare listare avansata (sortare/filtrare)
    int listSortMode;
    bool listSortAscending;
    int listTypeFilter;

    //starea terminalului integrat
    std::string terminalInput;
    std::vector<std::string> terminalOutput;
    bool nanoEditMode;
    std::string nanoTargetFile;
    std::string nanoBuffer;

    //mesaj de status
    std::string statusMessage;

    //helpers de randare
    void renderToolbar();
    void renderTreePanel();
    void renderTreeNodeRecursive(const std::shared_ptr<Component>& component);
    void renderDetailsPanel();
    void renderActionsPanel();
    void renderTerminalPanel();
    std::shared_ptr<Directory> getCreateTargetDirectory() const;
    std::shared_ptr<Directory> getDeleteParentDirectory() const;
    void executeTerminalCommand(const std::string& commandLine);
    void appendTerminalOutput(const std::string& line);

    void setStatus(const std::string& message);

public:
    VFSInterface();

    //functia principala apelata pe fiecare frame
    void render();
};
