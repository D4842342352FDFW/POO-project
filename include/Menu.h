#pragma once
#include "../include/Exception.h"
#include <string>

class Menu{
    public:
    //rularea aplicatie
    void run();

    private:
    
        //functiile de actiune in aplicatie
        void printWelcome() const;
        void printHelp() const ;
        void handleCreateFile();
        void handleCreateDirectory();
        void handleSearch();
        void handleNavigate();
        void handleReadFile();
        void handleDecryptContent();
        void handleDeleteComponent();
        void handleCommandException(const Exception& e) const;
};
