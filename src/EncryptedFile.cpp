#include "../include/EncryptedFile.h"
#include <iostream>

//constructorul
EncryptedFile::EncryptedFile(const std::string& name, const std::string& content) : File(name, content)
{
    encrypt();
}

//functia de criptare
void EncryptedFile::encrypt()
{
    const size_t keyLen = ENCRYPTION_KEY.size();
    for(size_t i = 0; i < fileSize; ++i)
    {
        unsigned char k = ENCRYPTION_KEY[i % keyLen];
        unsigned char x = data[i];
        x = rotateLeft(x, 5);
        x ^= (k + (i & 0xFF));
        data[i] = x;
    }
}

//functia de decriptare
void EncryptedFile::decrypt()
{
    const size_t keyLen = ENCRYPTION_KEY.size();
    for(size_t i = 0; i < fileSize; ++i)
    {
        unsigned char k = ENCRYPTION_KEY[i % keyLen];
        unsigned char x = data[i];
        x ^= (k + (i & 0xFF));
        x = rotateRight(x, 5);
        data[i] = x;
    }
}

//afisare date fisier criptat
void EncryptedFile::display(int depth) const{
    for(int i = 0; i < depth; ++i)
    {
        std::cout << " ";
    }
    std::string time = ctime(&timestamp);
    time.pop_back();
    std::cout<< time << " -> Encrypted File " << name << " has " << fileSize << " bytes\n";
}

//afisare continut encrypted file
std::string EncryptedFile:: readContent() const{
    if(!data || fileSize == 0)
        return "";
    return File::readContent();
}

//afisare continut encrypted file decriptatat
std::string EncryptedFile:: readDecryptedContent(){
    if(!data || fileSize == 0)
        return "";
    this->decrypt();
    std::string content = File::readContent();
    this->encrypt();
    return content;
}
