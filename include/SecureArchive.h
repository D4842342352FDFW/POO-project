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
    std::string getTypeName() const override;
    std::vector<std::string> getMetadataLines() const override;
    size_t getSize() const override;
    std::string readContent() const override;
    std::string getDisplayContent() const override;
    std::string getReadableContent() const override;
    void setReadableContent(const std::string& content) override;
    std::string getPersistentContent() override;

    //functia de afisare continut in forma decriptata si decompresata 
    std::string readDecryptedAndDecompressedContentContent();
};