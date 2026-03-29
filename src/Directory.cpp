#include "../include/Directory.h"
#include <iostream>

//functia de clear
void Directory::clear()
{
    for(auto child : children)
    {
        delete child;
    }
    children.clear();
}

//functia de copiere element cu element
void Directory::copyFrom(const Directory& other)
{
    for(const auto& child : other.children)
    {
        this->addComponent(child->clone());
    }
}

//constructor director 
Directory::Directory(const std::string& name) : Component(name) {}

//destructor
Directory::~Directory()
{
    clear();
}

//constructor copiere
Directory::Directory(const Directory& other) : Component(other.getName())
{
    this->copyFrom(other);
}

//suprascrierea atribuirii 
Directory& Directory::operator=(const Directory& other)
{
    if(this != &other)
    {
        clear();
        this->name = other.name;
        this->copyFrom(other);
    }
    return *this;
}

//functia de clone 
Component* Directory::clone() const{
    return new Directory(*this);
}

//afisare recursiva date director
void Directory::display(int depth) const
{
    for(int i = 0; i < depth; ++i)
        std::cout<<" ";
    std::string time = ctime(&timestamp);
    time.pop_back();
    std::cout << time << " -> Directory " << name << " has " << getSize() << " bytes total\n";

    for(auto child : children)
    {
        child->display(depth+1);
    }
}

//getter calcul dimensiune totala director
size_t Directory::getSize() const{
    size_t total = 0;
    for(auto child : children)
    {
        total = total + child->getSize();
    }
    return total;
}

//getter copii
const std::vector<Component*>& Directory::getChildren() const{
    return children;
}

//setter nume
void Directory::setName(const std::string& name)
{
    this->name = name;
}

//adagare componenta 
void Directory::addComponent(Component* component)
{
    if(component)
    {
        component->setParent(this);
        children.push_back(component);
    }
}

//stergere componenta
void Directory::removeComponent(const std::string& name)
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        if((*it)->getName() == name)
        {
            delete *it;
            children.erase(it);
            return;
        }
    }
}

//cautare componenta
Component* Directory::findComponent(const std::string& name) const{
    for(auto child : children)
    {
        if(child->getName() == name)
            return child;
    }
    return nullptr;
}
