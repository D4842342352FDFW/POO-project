#include "Material.h"
#include <iostream>
#include <cstring>

//constructor default material
Material::Material() : tip("Necunoscut"), greutate(0.0), gradContaminare(0.0){
    lotId = new char[2]; 
    strcpy(lotId,  "0");
};

//constructor cu parametri material
Material::Material(const char* id, std::string tp, double gr, double gc) : tip(tp), greutate(gr), gradContaminare(gc)
{
    if(id != nullptr)
    {
        lotId = new char[strlen(id) + 1];
        strcpy(lotId,id);  
    }
    else
    {
        lotId = new char[2];
        strcpy(lotId, "0");
    }
    
};

//constructor de copiere material 
Material::Material(const Material& altul) : tip(altul.tip), greutate(altul.greutate), gradContaminare(altul.gradContaminare)
{
    lotId = new char[strlen(altul.lotId) + 1];
    strcpy(lotId, altul.lotId);
};

//operator de atribuire material
Material& Material::operator=(const Material& altul)
{
    if(this == &altul)
    {
        return *this;
    }   

    delete[] lotId;
    tip = altul.tip;
    greutate = altul.greutate;
    gradContaminare = altul.gradContaminare;
    lotId = new char[strlen(altul.lotId) + 1];
    strcpy(lotId, altul.lotId);
    
    return *this;
}

//destructor material 
Material::~Material()
{
    delete[] lotId;
};

//suprascriere operator afisare
std::ostream& operator<<(std::ostream &os, const Material& m)
{
    os << "Material din lotul: " << m.lotId << " Tip: " << m.tip << " Greutate: " << m.greutate 
    << " kg Contaminare: " << m.gradContaminare * 100 << " % ";
    return os;
}