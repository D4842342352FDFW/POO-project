#include <iostream>
#include <string>
#include "../include/Component.h"
#include "../include/Directory.h"
#include "../include/File.h"
int main() {

    std::cout << Component::getInstanceCount() << '\n';
    Directory* root = new Directory("root");
    File* file1 = new File("bla.txt", "cartea de citit");
    File* file2 = new File("idk.txt", "saaaaaaaa");
    root->addComponent(file1);
    root->addComponent(file2);
    Directory* documents = new Directory("Documents");
    root->addComponent(documents);
    File* file3 = new File("ssdasd", "dasdsa");
    documents->addComponent(file3);
    root->display(0);
    std::cout << root->getSize() << '\n';
    std::cout << file2->readContent() << '\n';
    std::cout << Component::getInstanceCount() << '\n';
    root->removeComponent("bla.txt");
    root->display(0);
    std::cout << '\n';
    std::cout << '\n';
    std::cout << '\n';
    std::cout << file2->readContent() << '\n';
    Component* gasit = root->findComponent("idk.txt");
    if(gasit != nullptr)
        std::cout << "daaaa\n";
    File* file4 = file3;
    std::cout << file4->readContent() << '\n';
    
    std::cout << '\n';
    std::cout << '\n';
    std::cout << '\n';
    Directory* downloads = new Directory("downloads");
    downloads = root;
    downloads->setName("downloads");
    downloads->display(0);
    return 0;
}