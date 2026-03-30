#pragma once
#include "../include/EncryptedFile.h"
#include "../include/CompressedFile.h"

class SecureArchive : public EncryptedFile, public CompressedFile{
    public:
    //constructor
    SecureArchive(const std::string& name, const std::string& content);

    //functie de decriptare si decompresie
    void decryptAndDecompress();
    
    //functiile redefinite
    void display(int depth) const override;
    size_t getSize() const override;
    std::string readContent() const override;

    //functia de afisare continut in forma decriptata si decompresata 
    std::string readDecryptedAndDecompressedContentContent();
};