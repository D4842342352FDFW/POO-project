#include "../include/Directory.h"
#include <iostream>

//functia de clear
void Directory::clear()
{
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
Directory::Directory(const Directory& other)
    : Component(other.getName()), std::enable_shared_from_this<Directory>()
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
std::shared_ptr<Component> Directory::clone() const{
    return std::make_shared<Directory>(*this);
}

//afisare recursiva date director
void Directory::display(int depth) const
{
    for(int i = 0; i < depth; ++i)
        std::cout<<" ";
    std::string time = ctime(&timestamp);
    time.pop_back();
    std::cout << time << " -> Directory " << name << " has " << getSize() << " bytes total\n";

    for(const auto& child : children)
    {
        child->display(depth+1);
    }
}
// getter tip de componenta
std::string Directory::getTypeName() const
{
    return "Directory";
}
//getter metadate director
std::vector<std::string> Directory::getMetadataLines() const
{
    return {"children: " + std::to_string(children.size())};
}

//getter calcul dimensiune totala director
size_t Directory::getSize() const{
    size_t total = 0;
    for(const auto& child : children)
    {
        total = total + child->getSize();
    }
    return total;
}

//getter copii
const std::vector<std::shared_ptr<Component>>& Directory::getChildren() const{
    return children;
}

//setter nume
void Directory::setName(const std::string& name)
{
    this->name = name;
}

//adagare componenta 
void Directory::addComponent(const std::shared_ptr<Component>& component)
{
    if(component)
    {
        component->setParent(shared_from_this());
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
            children.erase(it);
            return;
        }
    }
}

//cautare componenta
std::shared_ptr<Component> Directory::findComponent(const std::string& name) const{
    for(const auto& child : children)
    {
        if(child->getName() == name)
            return child;
    }
    return nullptr;
}
