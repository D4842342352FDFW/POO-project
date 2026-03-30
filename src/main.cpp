#include <iostream>
#include <string>
#include "../include/Component.h"
#include "../include/Directory.h"
#include "../include/File.h"
#include "../include/ImageFile.h"
#include "../include/VideoFile.h"
#include "../include/EncryptedFile.h"
#include "../include/CompressedFile.h"
int main() {

    // std::cout << Component::getInstanceCount() << '\n';
    // Directory* root = new Directory("root");
    // File* file1 = new File("bla.txt", "cartea de citit");
    // File* file2 = new File("idk.txt", "saaaaaaaa");
    // root->addComponent(file1);
    // root->addComponent(file2);
    // Directory* documents = new Directory("Documents");
    // root->addComponent(documents);
    // File* file3 = new File("ssdasd", "dasdsa");
    // documents->addComponent(file3);
    // root->display(0);
    // std::cout << root->getSize() << '\n';
    // std::cout << file2->readContent() << '\n';
    // std::cout << Component::getInstanceCount() << '\n';
    // root->removeComponent("bla.txt");
    // root->display(0);
    // std::cout << '\n';
    // std::cout << '\n';
    // std::cout << '\n';
    // std::cout << file2->readContent() << '\n';
    // Component* gasit = root->findComponent("idk.txt");
    // if(gasit != nullptr)
    //     std::cout << "daaaa\n";
    // File* file4 = file3;
    // std::cout << file4->readContent() << '\n';
    
    // std::cout << '\n';
    // std::cout << '\n';
    // std::cout << '\n';
    // Directory* downloads = new Directory("downloads");
    // downloads = root;
    // downloads->setName("downloads");
    // downloads->display(0);
    // ImageFile* imagine = new ImageFile("poza.png", 1024, 512, 4, "png");
    // ImageFile* copie = imagine;
    // std::cout << '\n';
    // std::cout << '\n';
    // std::cout << '\n';
    // copie->display(0);
    // std::cout << '\n';
    // std::cout << '\n';
    // std::cout << '\n';
    // downloads->addComponent(imagine);
    // downloads->display(0);
    // std::cout << '\n';
    // std::cout << '\n';
    // std::cout << '\n';
    // downloads->removeComponent("poza.png");
    // downloads->display(0);
    // VideoFile* video = new VideoFile("oala.mp3", 1920, 1080, 24, 60, 180.3, "mp3");
    // downloads->addComponent(video);
    // std::cout << '\n';
    // downloads->display(0);
    // EncryptedFile* file10 = new EncryptedFile("fisier.txt", "am fost la mare");
    // std::cout<< file10->readContent()<< '\n';
    // std::cout<< file10->readDecryptedContent()<<'\n';
    // file4->display(0);
    // root->addComponent(file10);
    // root->display(0);
    // EncryptedFile* file11 =file10;
    // file11->display(0);
    CompressedFile* fileidk = new CompressedFile("noidea.json", "am o casa la mare si una la munte");
    std::cout << fileidk->readContent()<< '\n';
    std:: cout<< fileidk->getSize() << '\n';
    std::cout << fileidk->readDecompressedContent()<< '\n';
    fileidk->decompress();
    std:: cout<< fileidk->getSize() << '\n';


    std::cout << '\n';
    std::cout << '\n';
    std::cout << '\n';
    CompressedFile* fileidk2 = new CompressedFile("no.json", "AABB");
    std::cout << fileidk2->readContent()<< '\n';
    std:: cout<< fileidk2->getSize() << '\n';
    std::cout << fileidk2->readDecompressedContent()<< '\n';
    fileidk->decompress();
    std:: cout<< fileidk2->getSize() << '\n';
    fileidk2->display(0);
    return 0;
}