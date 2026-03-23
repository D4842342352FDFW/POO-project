#pragma once 
#include "Material.h"
#include <string>
#include <iostream>

class Senzor{
protected:
    //date senzor 
    std::string tipSenzor;
public:

    //contructorii de senzor
    Senzor();
    Senzor(std::string tSen);
    
    //destructorul de senzor 
    virtual ~Senzor();

    //verificare prorprietate material in functie de tipul de senzor 
    virtual bool verifica(const Material& m) const = 0;

    //afisare informatii
    virtual void afisareInformatii() const;
};