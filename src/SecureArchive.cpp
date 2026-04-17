#include "../include/SecureArchive.h"
#include <iostream>

//constructor
SecureArchive::SecureArchive(const std::string& name, const std::string& content)
: File(name, content), EncryptedFile(name, content, false), CompressedFile(name, content, false)
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

std::string SecureArchive::getTypeName() const
{
    return "Secure Archive";
}

std::vector<std::string> SecureArchive::getMetadataLines() const
{
    std::vector<std::string> lines = CompressedFile::getMetadataLines();
    lines.insert(lines.begin(), "content: encrypted + compressed");
    return lines;
}

//getter dimensiunea mostenita de la compresare
size_t SecureArchive::getSize() const{
    return CompressedFile::getSize();
}

//partea de citire continut fisier
std::string SecureArchive::readContent() const{
    return File::readContent();
}

//persistenta foloseste continutul decriptat si decomprimat
std::string SecureArchive::getPersistentContent()
{
    return readDecryptedAndDecompressedContentContent();
}


//partea de decriptare si decompresare pentru 
//afisarea textului initial 
std::string SecureArchive::readDecryptedAndDecompressedContentContent()
{
    if(fileSize == 0)
        return "";
    this->decrypt();
    this->decompress();
    std::string content = File::readContent();
    this->compress();
    this->encrypt();

    return content;
}

std::string SecureArchive::getReadableContent() const
{
    return const_cast<SecureArchive*>(this)->readDecryptedAndDecompressedContentContent();
}

std::string SecureArchive::getDisplayContent() const
{
    return readContent();
}

void SecureArchive::setReadableContent(const std::string& content)
{
    decrypt();
    decompress();
    data = content;
    fileSize = data.length();
    compress();
    encrypt();
}
