#pragma once
#include <iostream>
#include <string> 

class Material 
{   
    //date material
    char* lotId;
    std::string tip;
    double greutate;
    double gradContaminare;

public:
    //constructori material 
    Material();
    Material(const char* id, std::string tp, double gr, double gc);
    Material(const Material& altul);
 
    //logica de copiere 
    Material& operator=(const Material& altul);

    //destructor material
    ~Material();

    //afisare date material
    void afisare() const;

    //gettere 
    std::string getTip() const 
    {
        return tip;
    }
    double getGreutate() const 
    {
        return greutate;
    }
    double getGradContaminare() const{
        return gradContaminare;
    }
    
};