#pragma once
#include "../include/Component.h"
#include "../include/File.h"
#include <string>

class EncryptedFile : virtual public File{

    protected:

    //cheie criptare
    static inline const std::string ENCRYPTION_KEY = "dragon";

    //shiftare caracter la stanga 
    static unsigned char rotateLeft(unsigned char value, int bits)
    {
        return (value << bits) | (value >> (8 - bits));
    }

    //shiftare caracter la dreapta
    static unsigned char rotateRight(unsigned char value, int bits)
    {
        return (value >> bits) | (value << (8 - bits));
    }

    public:
    //constructor
    EncryptedFile(const std::string& name, const std::string& content);
    EncryptedFile(const std::string& name, const std::string& content, bool shouldEncrypt);

    //functiile de conversie
    void encrypt();
    void decrypt();

    //afisare date fisier
    void display(int depth) const override;
    
    //afisare continut fisier criptat
    std::string readContent() const override;
    //afisare continut fisier decriptat
    std::string readDecryptedContent();
};
