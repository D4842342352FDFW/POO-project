#include "../include/Menu.h"
#include "../include/Manager.h"
#include "../include/Exception.h"
#include <iostream>
#include <iomanip>
#include <string>

//functia de rulare a meniului
void Menu::run()
{
    Manager* manager = Manager::getInstance();
    std::string command;
    printWelcome();

    while(true)
    {
        std::cout << "\n" << manager->getCurrentPath() << " > ";
        std::cin >> command;

        try
        {
            if(command == "exit")
            {
                std::cout << "Bye bye\n";
                break;
            }
            else
            {
                if(command == "tree")
                {
                    manager->displayTree();
                }
                else
                {
                    if(command =="mkdir")
                    {
                        handleCreateDirectory();
                    }
                    else
                    {
                        if(command == "touch")
                        {
                            handleCreateFile();
                        }
                        else
                        {
                            if(command == "cd")
                            {
                                handleNavigate();
                            }
                            else
                            {
                                if(command == "find")
                                {
                                    handleSearch();
                                }
                                else
                                {
                                    if(command == "cat")
                                    {
                                        handleReadFile();
                                    }
                                    else
                                    {
                                        if(command == "decrypt")
                                        {
                                            handleDecryptContent();
                                        }
                                        else
                                        {
                                                if(command == "rm")
                                                {
                                                    handleDeleteComponent();
                                                }
                                                else
                                                {
                                                    if(command == "help")
                                                    {
                                                        printHelp();
                                                    }
                                                    else
                                                    {
                                                        std::cout << "Unknown command. Type help for info.\n";
                                                    }
                                                }
                                            
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        catch(const Exception& e)
        {
            std::cerr << "VFS Error: " << e.what() << '\n';
        }
        
        }
    
}

//afisare mesaj intrare in aplicatie
void Menu::printWelcome() const 
{
    std::cout << "------------------------------------------\n";
    std::cout << "----------------Welcome-------------------\n";
    std::cout << "------------------------------------------\n";
    std::cout << "--Type 'help' to see available commands.--\n";
}

//comanda de help care afiseaza lista de comenzi implementate
void Menu::printHelp() const {
    std::cout << "\nAvailable Commands:\n"
              << "  tree      - View directory tree\n"
              << "  mkdir     - Create new folder\n"
              << "  touch     - Create new file\n"
              << "  cd        - Change directory\n"
              << "  find      - Search\n"
              << "  cat       - Display file content\n"
              << "  decrypt   - Display decrypted content\n"
              << "  rm        - Delete file or directory\n"
              << "  help      - Display available commands\n"
              << "  exit      - Shutdown system\n";
}

//afisare erori
void Menu::handleCommandException(const Exception& e) const {
    std::cerr << "VFS Error: " << e.what() << std::endl;
}

//meniu de creare fisier in functie de tip
void Menu::handleCreateFile() {
    try 
    {
        std::string name, content;
        int type;
        std::cout << "Filename: "; 
        std::cin >> name;
        std::cout << "Type (1: Raw, 2: Image, 3: Video, 4: Encrypted, 5: Compressed, 6: SecureArchive): ";
        std::cin >> type;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            throw Exception("Invalid type input");
        }
        
        if(type == 2)
        {
            int width, height, bpp;
            std::string format;
            std::cout << "Width: "; 
            std::cin >> width;
            if (std::cin.fail()) 
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                throw Exception("Invalid width input");
            }
            std::cout << "Height: "; 
            std::cin >> height;
            if (std::cin.fail()) 
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                throw Exception("Invalid height input");
            }
            std::cout << "Bits per pixel: "; 
            std::cin >> bpp;
            if (std::cin.fail()) 
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                throw Exception("Invalid bpp input");
            }
            std::cout << "Format: "; 
            std::cin >> format;
            Manager::getInstance()->createFile(name, "", type, width, height, bpp, 0, 0.0, format);
        }
        else
        {
            if(type == 3)
            {
                int width, height, bpp, fps;
                double duration;
                std::string format;
                std::cout << "Width: "; 
                std::cin >> width;
                if (std::cin.fail()) 
                {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    throw Exception("Invalid width input");
                }
                std::cout << "Height: "; 
                std::cin >> height;
                if (std::cin.fail()) 
                {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    throw Exception("Invalid height input");
                }
                std::cout << "Bits per pixel: "; 
                std::cin >> bpp;
                if (std::cin.fail()) 
                {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    throw Exception("Invalid bpp input");
                }
                std::cout << "FPS: "; 
                std::cin >> fps;
                if (std::cin.fail()) 
                {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    throw Exception("Invalid fps input");
                }
                std::cout << "Duration: "; 
                std::cin >> duration;
                if (std::cin.fail()) 
                {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    throw Exception("Invalid duration input");
                }
                std::cout << "Format: "; 
                std::cin >> format;
                Manager::getInstance()->createFile(name, "", type, width, height, bpp, fps, duration, format);
            }
            else
            {
                if(type >= 1 && type <= 6)
                {
                    std::cout << "Content: ";
                    std::cin.ignore();
                    std::getline(std::cin, content);
                    Manager::getInstance()->createFile(name, content, type);
                }
                else
                {
                    throw Exception("Invalid file type");
                }
            }
        }
        std::cout << "File created successfully.\n";
    } 
    catch (const Exception& e) 
    {
        handleCommandException(e);
    }
}

//logica de decripare/decompresare 
void Menu::handleDecryptContent()
{
    try
    {
        std::string name;
        std::cout << "File to decrypt: "; 
        std::cin >> name;

        Component* component = Manager::getInstance()->getComponent(name);

        if(component->isDirectory())
        {
            throw Exception("Cannot decrypt a directory");
        }

        EncryptedFile* encryptedFile = dynamic_cast<EncryptedFile*>(component);
        SecureArchive* secureArchive = dynamic_cast<SecureArchive*>(component);
        CompressedFile* compressedFile = dynamic_cast<CompressedFile*>(component);

        if(secureArchive)
        {
            std::cout << "Decrypted content of " << name << ":\n";
            std::cout << secureArchive->readDecryptedAndDecompressedContentContent() << '\n';
        }
        else
        {
            if(encryptedFile)
            {
                std::cout << "Decrypted content of " << name << ":\n";
                std::cout << encryptedFile->readDecryptedContent() << '\n';
            }
            else
            {
                if(compressedFile)
                {
                    std::cout << "Decompressed content of " << name << ":\n";
                    std::cout << compressedFile->readDecompressedContent() << '\n';
                }
                else
                {
                    throw Exception("File does not support decrypt");
                }
            }
        }
    }
    catch(const Exception& e)
    {
        handleCommandException(e);
    }
}

//crearea de folder
void Menu::handleCreateDirectory() {
    try 
    {
        std::string name;
        std::cout << "Folder name: "; 
        std::cin >> name;
        Manager::getInstance()->createDirectory(name);
        std::cout << "Directory created successfully.\n";
    } 
    catch (const Exception& e) 
    {
        handleCommandException(e);
    }
}

//trecerea de la un director in altul
void Menu::handleNavigate() {
    try 
    {
        std::string name;
        std::cout << "Go to: "; 
        std::cin >> name;
        Manager::getInstance()->changeDirectory(name);
    } 
    catch (const Exception& e) 
    {
        handleCommandException(e);
    }
}

//partea de cautare dupa nume
void Menu::handleSearch() {
    try 
    {
        std::string name;
        std::cout << "Search for: "; 
        std::cin >> name;
        Manager::getInstance()->search(name);
    } 
    catch (const Exception& e) 
    {
        handleCommandException(e);
    }
}

//afisare continut fisier
void Menu::handleReadFile() {
    try 
    {
        std::string name;
        std::cout << "File to read: "; 
        std::cin >> name;
        std::string content = Manager::getInstance()->readFile(name);
        std::cout << "Content of " << name << ":\n" << content << std::endl;
    } 
    catch (const Exception& e) 
    {
        handleCommandException(e);
    }
}

//stergerea fiserului/folderului
void Menu::handleDeleteComponent() {
    try 
    {
        std::string name;
        std::cout << "Component to delete: "; 
        std::cin >> name;
        Manager::getInstance()->deleteComponent(name);
        std::cout << "Component '" << name << "' deleted successfully.\n";
    } catch (const Exception& e) 
    {
        handleCommandException(e);
    }
}
