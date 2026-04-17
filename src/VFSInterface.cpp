#include "../include/VFSInterface.h"
#include "../include/Directory.h"
#include "../include/File.h"
#include "../include/EncryptedFile.h"
#include "../include/CompressedFile.h"
#include "../include/SecureArchive.h"
#include "../include/ImageFile.h"
#include "../include/VideoFile.h"
#include "../include/Exception.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <sstream>

namespace
{
std::string componentPath(Component* component)
{
    if(component == nullptr)
    {
        return "";
    }

    std::string path = component->getName();
    Directory* parent = component->getParent();
    while(parent)
    {
        path = parent->getName() + "/" + path;
        parent = parent->getParent();
    }
    return path;
}

std::string trimLeft(std::string value)
{
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char ch)
    {
        return !std::isspace(ch);
    }));
    return value;
}

std::string toLower(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c)
    {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

bool parseFileTypeToken(const std::string& token, int& outType)
{
    const std::string typeToken = toLower(token);
    if(typeToken == "1" || typeToken == "raw" || typeToken == "normal")
    {
        outType = 1;
        return true;
    }
    if(typeToken == "2" || typeToken == "image")
    {
        outType = 2;
        return true;
    }
    if(typeToken == "3" || typeToken == "video")
    {
        outType = 3;
        return true;
    }
    if(typeToken == "4" || typeToken == "encrypted")
    {
        outType = 4;
        return true;
    }
    if(typeToken == "5" || typeToken == "compressed" || typeToken == "archive" || typeToken == "archived")
    {
        outType = 5;
        return true;
    }
    if(typeToken == "6" || typeToken == "secure" || typeToken == "secure-archive" || typeToken == "secure_archive")
    {
        outType = 6;
        return true;
    }

    return false;
}

const char* fileTypeLabel(const int type)
{
    switch(type)
    {
    case 1:
        return "Raw File";
    case 2:
        return "Image File";
    case 3:
        return "Video File";
    case 4:
        return "Encrypted File";
    case 5:
        return "Compressed File";
    case 6:
        return "Secure Archive";
    default:
        return "Unknown";
    }
}

std::string listSortModeToken(const int sortMode)
{
    if(sortMode == 1)
    {
        return "size";
    }
    if(sortMode == 2)
    {
        return "time";
    }
    return "name";
}

std::string listFilterToken(const int filterIndex)
{
    switch(filterIndex)
    {
    case 1:
        return "directory";
    case 2:
        return "file";
    case 3:
        return "raw";
    case 4:
        return "image";
    case 5:
        return "video";
    case 6:
        return "encrypted";
    case 7:
        return "compressed";
    case 8:
        return "secure";
    default:
        return "";
    }
}
}

VFSInterface::VFSInterface()
{
    manager = Manager::getInstance();
    selectedComponent = manager->getRoot();
    showContentPreview = false;

    newFolderName = "";

    newFileName = "";
    newFileContent = "";
    newFileType = 1;
    imageWidth = 1920;
    imageHeight = 1080;
    imageBpp = 24;
    videoWidth = 1920;
    videoHeight = 1080;
    videoBpp = 24;
    videoFps = 60;
    videoDuration = 10.0;
    mediaFormat = "txt";

    searchName = "";
    listSortMode = 0;
    listSortAscending = true;
    listTypeFilter = 0;
    terminalInput = "";
    terminalOutput.clear();
    nanoEditMode = false;
    nanoTargetFile = "";
    nanoBuffer = "";
    terminalOutput.push_back("Type 'help' for supported VFS commands.");
    statusMessage = "Ready";
}

void VFSInterface::setStatus(const std::string& message)
{
    statusMessage = message;
}

Directory* VFSInterface::getCreateTargetDirectory() const
{
    if(selectedComponent == nullptr)
    {
        return manager->getCurrentDirectory();
    }

    if(selectedComponent->isDirectory())
    {
        return static_cast<Directory*>(selectedComponent);
    }

    return selectedComponent->getParent();
}

Directory* VFSInterface::getDeleteParentDirectory() const
{
    if(selectedComponent == nullptr)
    {
        return nullptr;
    }

    return selectedComponent->getParent();
}

void VFSInterface::render()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
    ImGuiWindowFlags mainWindowFlags = ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("VFS Simulator", nullptr, mainWindowFlags);

    renderToolbar();
    ImGui::Separator();

    ImGui::Columns(2, "MainPanels", true);
    renderTreePanel();
    ImGui::NextColumn();
    renderActionsPanel();
    ImGui::Separator();
    renderDetailsPanel();
    ImGui::Columns(1);

    ImGui::Separator();
    renderTerminalPanel();

    ImGui::Separator();
    ImGui::Text("Status: %s", statusMessage.c_str());

    ImGui::End();
}

void VFSInterface::appendTerminalOutput(const std::string& line)
{
    terminalOutput.push_back(line);
    if(terminalOutput.size() > 300)
    {
        terminalOutput.erase(terminalOutput.begin());
    }
}

void VFSInterface::executeTerminalCommand(const std::string& commandLine)
{
    if(commandLine.empty())
    {
        return;
    }

    appendTerminalOutput(manager->getCurrentPath() + " $ " + commandLine);

    std::istringstream iss(commandLine);
    std::string command;
    iss >> command;

    if(command == "help")
    {
        appendTerminalOutput("Supported VFS commands:");
        appendTerminalOutput("  help");
        appendTerminalOutput("  pwd");
        appendTerminalOutput("  tree");
        appendTerminalOutput("  cd <dir>");
        appendTerminalOutput("  mkdir <name>");
        appendTerminalOutput("  touch <name> [-t <type>] [content]");
        appendTerminalOutput("    Types: 1/raw, 2/image, 3/video, 4/encrypted, 5/compressed, 6/secure");
        appendTerminalOutput("  ls [name|size|time] [asc|desc] [directory|file|raw|image|video|encrypted|compressed|secure]");
        appendTerminalOutput("  info <name>");
        appendTerminalOutput("  stats");
        appendTerminalOutput("  nano <name>");
        appendTerminalOutput("  rm <name>");
        appendTerminalOutput("  cat <name>");
        appendTerminalOutput("  find <name>");
        appendTerminalOutput("  decrypt <name>");
        appendTerminalOutput("  clear");
        return;
    }

    if(command == "clear")
    {
        terminalOutput.clear();
        return;
    }

    if(command == "pwd")
    {
        appendTerminalOutput(manager->getCurrentPath());
        return;
    }

    try
    {
        if(command == "tree")
        {
            const std::vector<std::string> lines = manager->getTreeLines();
            for(const auto& line : lines)
            {
                appendTerminalOutput(line);
            }
            return;
        }

        if(command == "cd")
        {
            std::string target;
            iss >> target;
            if(target.empty())
            {
                appendTerminalOutput("Usage: cd <directory>");
                return;
            }

            manager->changeDirectory(target);
            appendTerminalOutput("Current directory: " + manager->getCurrentPath());
            selectedComponent = manager->getCurrentDirectory();
            return;
        }

        if(command == "mkdir")
        {
            std::string folderName;
            iss >> folderName;
            if(folderName.empty())
            {
                appendTerminalOutput("Usage: mkdir <name>");
                return;
            }

            manager->createDirectory(folderName);
            appendTerminalOutput("Directory created: " + folderName);
            return;
        }

        if(command == "touch")
        {
            std::string fileName;
            iss >> fileName;
            if(fileName.empty())
            {
                appendTerminalOutput("Usage: touch <name> [-t <type>] [content]");
                return;
            }

            int fileType = 1;
            std::string optionToken;
            iss >> std::ws;
            if(iss.peek() == '-')
            {
                iss >> optionToken;
                std::string typeToken;

                if(optionToken == "-t" || optionToken == "--type")
                {
                    if(!(iss >> typeToken))
                    {
                        appendTerminalOutput("Usage: touch <name> [-t <type>] [content]");
                        return;
                    }
                }
                else if(optionToken.rfind("--type=", 0) == 0)
                {
                    typeToken = optionToken.substr(7);
                }
                else
                {
                    appendTerminalOutput("Unknown touch option. Use -t or --type");
                    return;
                }

                if(!parseFileTypeToken(typeToken, fileType))
                {
                    appendTerminalOutput("Invalid type. Use 1/raw, 2/image, 3/video, 4/encrypted, 5/compressed, 6/secure");
                    return;
                }
            }

            std::string content;
            std::getline(iss, content);
            content = trimLeft(content);

            Directory* previousDirectory = manager->getCurrentDirectory();
            try
            {
                manager->setCurrentDirectory(getCreateTargetDirectory());

                if(fileType == 2)
                {
                    manager->createFile(fileName, "", fileType,
                        imageWidth, imageHeight, imageBpp, 0, 0.0, mediaFormat);
                }
                else
                {
                    if(fileType == 3)
                    {
                        manager->createFile(fileName, "", fileType,
                            videoWidth, videoHeight, videoBpp, videoFps, videoDuration, mediaFormat);
                    }
                    else
                    {
                        manager->createFile(fileName, content, fileType);
                    }
                }

                manager->setCurrentDirectory(previousDirectory);
            }
            catch(...)
            {
                manager->setCurrentDirectory(previousDirectory);
                throw;
            }

            appendTerminalOutput(std::string("File created: ") + fileName + " (" + fileTypeLabel(fileType) + ")");
            return;
        }

        if(command == "rm")
        {
            std::string componentName;
            iss >> componentName;
            if(componentName.empty())
            {
                appendTerminalOutput("Usage: rm <name>");
                return;
            }

            manager->deleteComponent(componentName);
            appendTerminalOutput("Removed: " + componentName);
            selectedComponent = manager->getCurrentDirectory();
            showContentPreview = false;
            return;
        }

        if(command == "ls")
        {
            std::string sortBy = "name";
            bool ascending = true;
            std::string typeFilter;

            std::string token;
            while(iss >> token)
            {
                const std::string normalized = toLower(token);
                if(normalized == "name" || normalized == "size" || normalized == "time" || normalized == "timestamp")
                {
                    sortBy = normalized;
                    continue;
                }

                if(normalized == "asc")
                {
                    ascending = true;
                    continue;
                }

                if(normalized == "desc")
                {
                    ascending = false;
                    continue;
                }

                typeFilter = normalized;
            }

            const auto lines = manager->getCurrentDirectoryListing(sortBy, ascending, typeFilter);
            if(lines.empty())
            {
                appendTerminalOutput("(no matching entries)");
            }
            else
            {
                for(const auto& line : lines)
                {
                    appendTerminalOutput(line);
                }
            }
            return;
        }

        if(command == "info")
        {
            std::string name;
            iss >> name;
            if(name.empty())
            {
                appendTerminalOutput("Usage: info <name>");
                return;
            }

            std::istringstream detailsStream(manager->getComponentDetails(name));
            std::string line;
            while(std::getline(detailsStream, line))
            {
                appendTerminalOutput(line);
            }
            return;
        }

        if(command == "stats")
        {
            std::istringstream statsStream(manager->getCurrentDirectoryStats());
            std::string line;
            while(std::getline(statsStream, line))
            {
                appendTerminalOutput(line);
            }
            return;
        }

        if(command == "nano")
        {
            std::string fileName;
            iss >> fileName;
            if(fileName.empty())
            {
                appendTerminalOutput("Usage: nano <file>");
                return;
            }

            Component* component = manager->getComponent(fileName);
            if(component->isDirectory())
            {
                appendTerminalOutput("Cannot edit a directory");
                return;
            }

            if(!component->supportsReadableContent())
            {
                appendTerminalOutput("This file type cannot be edited as text");
                return;
            }

            nanoEditMode = true;
            nanoTargetFile = fileName;
            nanoBuffer = component->getReadableContent();
            appendTerminalOutput("nano: editing " + fileName + " (Save or Cancel below)");
            return;
        }

        if(command == "cat")
        {
            std::string fileName;
            iss >> fileName;
            if(fileName.empty())
            {
                appendTerminalOutput("Usage: cat <file>");
                return;
            }

            appendTerminalOutput(manager->readFile(fileName));
            return;
        }

        if(command == "find")
        {
            std::string target;
            iss >> target;
            if(target.empty())
            {
                appendTerminalOutput("Usage: find <name>");
                return;
            }

            const std::string absPath = manager->getAbsoluteDiskPath(target);
            appendTerminalOutput("Found: " + absPath);
            return;
        }

        if(command == "decrypt")
        {
            std::string fileName;
            iss >> fileName;
            if(fileName.empty())
            {
                appendTerminalOutput("Usage: decrypt <file>");
                return;
            }

            Component* component = manager->getComponent(fileName);
            if(component->isDirectory())
            {
                appendTerminalOutput("Cannot decrypt a directory");
                return;
            }

            if(SecureArchive* secureArchive = dynamic_cast<SecureArchive*>(component))
            {
                appendTerminalOutput(secureArchive->readDecryptedAndDecompressedContentContent());
                return;
            }

            if(EncryptedFile* encryptedFile = dynamic_cast<EncryptedFile*>(component))
            {
                appendTerminalOutput(encryptedFile->readDecryptedContent());
                return;
            }

            if(CompressedFile* compressedFile = dynamic_cast<CompressedFile*>(component))
            {
                appendTerminalOutput(compressedFile->readDecompressedContent());
                return;
            }

            appendTerminalOutput("File does not support decrypt");
            return;
        }

        appendTerminalOutput("Unknown command. Type 'help' for available VFS commands.");
    }
    catch(const Exception& e)
    {
        appendTerminalOutput(std::string("Error: ") + e.what());
    }
}

void VFSInterface::renderTerminalPanel()
{
    ImGui::Text("Embedded Terminal");
    ImGui::BeginChild("TerminalOutput", ImVec2(0, 160), true);
    for(const auto& line : terminalOutput)
    {
        ImGui::TextWrapped("%s", line.c_str());
    }
    if(ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
        ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();

    ImGui::SetNextItemWidth(-90.0f);
    const bool enterPressed = ImGui::InputText("##terminal_input", &terminalInput, ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    if(ImGui::Button("Run") || enterPressed)
    {
        const std::string commandLine = terminalInput;
        terminalInput = "";
        executeTerminalCommand(commandLine);
    }

    if(nanoEditMode)
    {
        ImGui::Separator();
        ImGui::Text("nano editor: %s", nanoTargetFile.c_str());
        ImGui::InputTextMultiline("##nano_editor", &nanoBuffer, ImVec2(0, 180));

        if(ImGui::Button("Save"))
        {
            try
            {
                manager->updateFileContent(nanoTargetFile, nanoBuffer);
                appendTerminalOutput("nano: saved " + nanoTargetFile);
                setStatus("File updated from nano editor");
            }
            catch(const Exception& e)
            {
                appendTerminalOutput(std::string("nano error: ") + e.what());
            }

            nanoEditMode = false;
            nanoTargetFile = "";
            nanoBuffer = "";
        }

        ImGui::SameLine();
        if(ImGui::Button("Cancel"))
        {
            appendTerminalOutput("nano: cancelled");
            nanoEditMode = false;
            nanoTargetFile = "";
            nanoBuffer = "";
        }
    }
}

void VFSInterface::renderToolbar()
{
    ImGui::Text("Current Directory: %s", manager->getCurrentPath().c_str());

    if(selectedComponent)
    {
        ImGui::Text("Selected: %s", componentPath(selectedComponent).c_str());
    }
    else
    {
        ImGui::TextUnformatted("Selected: (none)");
    }
}

void VFSInterface::renderTreePanel()
{
    ImGui::Text("Hierarchy");
    ImGui::Separator();

    Directory* root = manager->getRoot();
    if(root == nullptr)
    {
        ImGui::TextUnformatted("(empty)");
        return;
    }

    renderTreeNodeRecursive(root);

    ImGui::Separator();
    ImGui::Text("Current Directory Explorer");

    const char* sortItems[] = {"Name", "Size", "Timestamp"};
    ImGui::Combo("Sort by", &listSortMode, sortItems, IM_ARRAYSIZE(sortItems));
    ImGui::Checkbox("Ascending", &listSortAscending);

    const char* filterItems[] = {
        "All",
        "Directories",
        "Any File",
        "Raw File",
        "Image File",
        "Video File",
        "Encrypted File",
        "Compressed File",
        "Secure Archive"
    };
    ImGui::Combo("Type Filter", &listTypeFilter, filterItems, IM_ARRAYSIZE(filterItems));

    const auto listingLines = manager->getCurrentDirectoryListing(
        listSortModeToken(listSortMode),
        listSortAscending,
        listFilterToken(listTypeFilter));

    ImGui::BeginChild("DirectoryListing", ImVec2(0, 170), true);
    if(listingLines.empty())
    {
        ImGui::TextUnformatted("(empty)");
    }
    else
    {
        for(const auto& line : listingLines)
        {
            ImGui::TextWrapped("%s", line.c_str());
        }
    }
    ImGui::EndChild();
}

void VFSInterface::renderTreeNodeRecursive(Component* component)
{
    if(component == nullptr)
    {
        return;
    }

    if(component->isDirectory())
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if(component == selectedComponent)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        bool opened = ImGui::TreeNodeEx(component, flags, "%s", component->getName().c_str());
        if(ImGui::IsItemClicked())
        {
            selectedComponent = component;
            manager->setCurrentDirectory(static_cast<Directory*>(component));
            showContentPreview = false;
        }

        if(opened)
        {
            Directory* directory = static_cast<Directory*>(component);
            for(auto child : directory->getChildren())
            {
                renderTreeNodeRecursive(child);
            }
            ImGui::TreePop();
        }
    }
    else
    {
        ImGuiTreeNodeFlags leafFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        if(component == selectedComponent)
        {
            leafFlags |= ImGuiTreeNodeFlags_Selected;
        }

        ImGui::TreeNodeEx(component, leafFlags, "* %s", component->getName().c_str());
        if(ImGui::IsItemClicked())
        {
            selectedComponent = component;
            manager->setCurrentDirectory(component->getParent());
            showContentPreview = false;
        }
    }
}

void VFSInterface::renderActionsPanel()
{
    ImGui::Text("Actions");
    ImGui::Separator();

    ImGui::InputText("Folder Name", &newFolderName);
    if(ImGui::Button("Create Folder"))
    {
        Directory* previousDirectory = manager->getCurrentDirectory();
        try
        {
            if(newFolderName.empty())
            {
                setStatus("Folder name cannot be empty");
            }
            else
            {
                manager->setCurrentDirectory(getCreateTargetDirectory());
                manager->createDirectory(newFolderName);
                newFolderName = "";
                setStatus("Folder created");
            }
            manager->setCurrentDirectory(previousDirectory);
        }
        catch(const Exception& e)
        {
            manager->setCurrentDirectory(previousDirectory);
            setStatus(e.what());
        }
    }

    ImGui::Separator();

    ImGui::InputText("File Name", &newFileName);
    const char* fileTypeItems[] = {
        "Raw File",
        "Image File",
        "Video File",
        "Encrypted File",
        "Compressed File",
        "Secure Archive"
    };
    int selectedTypeIndex = newFileType - 1;
    if(selectedTypeIndex < 0)
    {
        selectedTypeIndex = 0;
    }
    if(selectedTypeIndex > 5)
    {
        selectedTypeIndex = 5;
    }
    ImGui::Combo("File Type", &selectedTypeIndex, fileTypeItems, IM_ARRAYSIZE(fileTypeItems));
    newFileType = selectedTypeIndex + 1;

    if(newFileType == 2)
    {
        ImGui::InputInt("Width", &imageWidth);
        ImGui::InputInt("Height", &imageHeight);
        ImGui::InputInt("Bits Per Pixel", &imageBpp);
        ImGui::InputText("Format", &mediaFormat);
    }
    else
    {
        if(newFileType == 3)
        {
            ImGui::InputInt("Width", &videoWidth);
            ImGui::InputInt("Height", &videoHeight);
            ImGui::InputInt("Bits Per Pixel", &videoBpp);
            ImGui::InputInt("FPS", &videoFps);
            ImGui::InputDouble("Duration", &videoDuration);
            ImGui::InputText("Format", &mediaFormat);
        }
        else
        {
            ImGui::InputTextMultiline("Content", &newFileContent, ImVec2(360, 120));
        }
    }

    if(ImGui::Button("Create File"))
    {
        Directory* previousDirectory = manager->getCurrentDirectory();
        try
        {
            if(newFileName.empty())
            {
                setStatus("File name cannot be empty");
            }
            else
            {
                manager->setCurrentDirectory(getCreateTargetDirectory());

                if(newFileType == 2)
                {
                    manager->createFile(newFileName, "", newFileType,
                        imageWidth, imageHeight, imageBpp, 0, 0.0, mediaFormat);
                }
                else
                {
                    if(newFileType == 3)
                    {
                        manager->createFile(newFileName, "", newFileType,
                            videoWidth, videoHeight, videoBpp, videoFps, videoDuration, mediaFormat);
                    }
                    else
                    {
                        manager->createFile(newFileName, newFileContent, newFileType);
                    }
                }

                newFileName = "";
                newFileContent = "";
                setStatus("File created");
            }

            manager->setCurrentDirectory(previousDirectory);
        }
        catch(const Exception& e)
        {
            manager->setCurrentDirectory(previousDirectory);
            setStatus(e.what());
        }
    }

    ImGui::Separator();

    if(ImGui::Button("Delete Selected"))
    {
        if(selectedComponent == nullptr)
        {
            setStatus("No selected component");
        }
        else
        {
            Directory* previousDirectory = manager->getCurrentDirectory();
            try
            {
                Directory* deleteParent = getDeleteParentDirectory();
                if(deleteParent == nullptr)
                {
                    setStatus("Cannot delete root directory");
                }
                else
                {
                    manager->setCurrentDirectory(deleteParent);
                    manager->deleteComponent(selectedComponent->getName());
                    selectedComponent = manager->getRoot();
                    manager->setCurrentDirectory(manager->getRoot());
                    showContentPreview = false;
                    setStatus("Selected component deleted");
                }

                if(selectedComponent != manager->getRoot())
                {
                    manager->setCurrentDirectory(previousDirectory);
                }
            }
            catch(const Exception& e)
            {
                manager->setCurrentDirectory(previousDirectory);
                setStatus(e.what());
            }
        }
    }

    ImGui::Separator();

    ImGui::SetNextItemWidth(180.0f);
    ImGui::InputText("##search_name", &searchName);
    ImGui::SameLine();
    if(ImGui::Button("Search"))
    {
        if(searchName.empty())
        {
            setStatus("Search field is empty");
        }
        else
        {
            try
            {
                const std::string absPath = manager->getAbsoluteDiskPath(searchName);
                setStatus("Absolute path: " + absPath);
            }
            catch(const Exception& e)
            {
                setStatus(e.what());
            }
        }
    }
}

void VFSInterface::renderDetailsPanel()
{
    ImGui::Text("Details");
    ImGui::Separator();

    if(selectedComponent == nullptr)
    {
        ImGui::TextUnformatted("Select a component from tree");
        return;
    }

    ImGui::Text("Name: %s", selectedComponent->getName().c_str());
    ImGui::Text("Type: %s", selectedComponent->getTypeName().c_str());
    ImGui::Text("Size: %llu bytes", static_cast<unsigned long long>(selectedComponent->getSize()));

    std::time_t ts = selectedComponent->getTimestamp();
    std::string timeText = std::ctime(&ts);
    if(!timeText.empty() && timeText.back() == '\n')
    {
        timeText.pop_back();
    }
    ImGui::Text("Timestamp: %s", timeText.c_str());

    bool encrypted = dynamic_cast<EncryptedFile*>(selectedComponent) != nullptr;
    bool compressed = dynamic_cast<CompressedFile*>(selectedComponent) != nullptr;
    if(dynamic_cast<SecureArchive*>(selectedComponent))
    {
        encrypted = true;
        compressed = true;
    }

    ImGui::Text("Encrypted: %s", encrypted ? "Yes" : "No");
    ImGui::Text("Compressed: %s", compressed ? "Yes" : "No");

    const std::vector<std::string> metadata = selectedComponent->getMetadataLines();
    for(const auto& line : metadata)
    {
        ImGui::TextWrapped("%s", line.c_str());
    }

    if(!selectedComponent->isDirectory())
    {
        if(dynamic_cast<ImageFile*>(selectedComponent) == nullptr && dynamic_cast<VideoFile*>(selectedComponent) == nullptr)
        {
            Directory* parentDirectory = selectedComponent->getParent();

            if(ImGui::Button("Convert to Normal"))
            {
                Directory* previousDirectory = manager->getCurrentDirectory();
                try
                {
                    manager->setCurrentDirectory(parentDirectory);
                    const std::string convertedName = manager->convertFileType(selectedComponent->getName(), 1);
                    selectedComponent = parentDirectory->findComponent(convertedName);
                    manager->setCurrentDirectory(parentDirectory);
                    showContentPreview = false;
                    setStatus("Converted to normal file");
                }
                catch(const Exception& e)
                {
                    setStatus(e.what());
                }
                manager->setCurrentDirectory(previousDirectory);
            }

            ImGui::SameLine();
            if(ImGui::Button("Convert to Encrypted"))
            {
                Directory* previousDirectory = manager->getCurrentDirectory();
                try
                {
                    manager->setCurrentDirectory(parentDirectory);
                    const std::string convertedName = manager->convertFileType(selectedComponent->getName(), 4);
                    selectedComponent = parentDirectory->findComponent(convertedName);
                    manager->setCurrentDirectory(parentDirectory);
                    showContentPreview = false;
                    setStatus("Converted to encrypted file");
                }
                catch(const Exception& e)
                {
                    setStatus(e.what());
                }
                manager->setCurrentDirectory(previousDirectory);
            }

            if(ImGui::Button("Convert to Archived"))
            {
                Directory* previousDirectory = manager->getCurrentDirectory();
                try
                {
                    manager->setCurrentDirectory(parentDirectory);
                    const std::string convertedName = manager->convertFileType(selectedComponent->getName(), 5);
                    selectedComponent = parentDirectory->findComponent(convertedName);
                    manager->setCurrentDirectory(parentDirectory);
                    showContentPreview = false;
                    setStatus("Converted to archived file");
                }
                catch(const Exception& e)
                {
                    setStatus(e.what());
                }
                manager->setCurrentDirectory(previousDirectory);
            }

            ImGui::SameLine();
            if(ImGui::Button("Convert to Secure Archive"))
            {
                Directory* previousDirectory = manager->getCurrentDirectory();
                try
                {
                    manager->setCurrentDirectory(parentDirectory);
                    const std::string convertedName = manager->convertFileType(selectedComponent->getName(), 6);
                    selectedComponent = parentDirectory->findComponent(convertedName);
                    manager->setCurrentDirectory(parentDirectory);
                    showContentPreview = false;
                    setStatus("Converted to secure archive");
                }
                catch(const Exception& e)
                {
                    setStatus(e.what());
                }
                manager->setCurrentDirectory(previousDirectory);
            }

            ImGui::Separator();
        }

        if(selectedComponent->supportsReadableContent() && ImGui::Button("View Content"))
        {
            showContentPreview = !showContentPreview;
        }

        if(!selectedComponent->supportsReadableContent())
        {
            ImGui::TextUnformatted("Readable content preview not available for this type.");
            showContentPreview = false;
        }

        if(showContentPreview)
        {
            ImGui::BeginChild("ContentPreview", ImVec2(0, 220), true);
            try
            {
                const std::string content = selectedComponent->getDisplayContent();
                ImGui::TextWrapped("%s", content.c_str());
            }
            catch(const Exception& e)
            {
                ImGui::TextWrapped("%s", e.what());
            }
            ImGui::EndChild();
        }
    }
}
