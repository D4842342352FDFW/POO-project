#include "../include/EncryptedFile.h"
#include <iostream>

//constructorul
EncryptedFile::EncryptedFile(const std::string& name, const std::string& content) : File(name, content)
{
    encrypt();
}

EncryptedFile::EncryptedFile(const std::string& name, const std::string& content, bool shouldEncrypt) : File(name, content)
{
    if(shouldEncrypt)
    {
        encrypt();
    }
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

std::string EncryptedFile::getTypeName() const
{
    return "Encrypted File";
}

std::vector<std::string> EncryptedFile::getMetadataLines() const
{
    return {"content: encrypted text", "cipher: xor+rotate"};
}

//afisare continut encrypted file
std::string EncryptedFile:: readContent() const{
    if(fileSize == 0)
        return "";
    return File::readContent();
}

//afisare continut encrypted file decriptatat
std::string EncryptedFile:: readDecryptedContent(){
    if(fileSize == 0)
        return "";
    this->decrypt();
    std::string content = File::readContent();
    this->encrypt();
    return content;
}

std::string EncryptedFile::getReadableContent() const
{
    return const_cast<EncryptedFile&>(*this).readDecryptedContent();
}

std::string EncryptedFile::getDisplayContent() const
{
    return readContent();
}

void EncryptedFile::setReadableContent(const std::string& content)
{
    data = content;
    fileSize = data.length();
    encrypt();
}

//persistenta foloseste continutul decriptat
std::string EncryptedFile::getPersistentContent()
{
    return readDecryptedContent();
}
