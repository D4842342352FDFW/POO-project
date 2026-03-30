#include "../include/SecureArchive.h"
#include <iostream>

//constructor
SecureArchive::SecureArchive(const std::string& name, const std::string& content)
: File(name, content), EncryptedFile(name, content), CompressedFile(name, content)
{
    compress();
    encrypt();
}
//decriptare + decompresie
void SecureArchive::decryptAndDecompress()
{
    decrypt();
    decompress();
}
//partea de afisare date 
void SecureArchive::display(int depth) const
{
    for(int i = 0; i < depth; ++i)
        std::cout << " ";
    std::string time = ctime(&timestamp);
    time.pop_back();
    std::cout<< time << " -> Secure Archive: Enc + Comp " << name << " has " << fileSize << " bytes\n";
}

//getter dimensiunea mostenita de la compresare
size_t SecureArchive::getSize() const{
    return CompressedFile::getSize();
}

//partea de citire continut fisier
std::string SecureArchive::readContent() const{
    return File::readContent();
}


//partea de decriptare si decompresare pentru 
//afisarea textului initial 
std::string SecureArchive::readDecryptedAndDecompressedContentContent()
{
    if(!data || fileSize == 0)
        return "";
    this->decrypt();
    this->decompress();
    std::string content = File::readContent();
    this->compress();
    this->encrypt();

    return content;
}