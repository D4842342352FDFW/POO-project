#pragma once 
#include <iostream>
#include <string>
#include "Material.h"

class UnitateReciclare{
protected:
    //date unitate reciclare
    static int contorId;
    int id;
    std::string nume;
    std::string adresa;
    bool gradFunctionalitate;

public:

    //constructori unitate reciclare
    UnitateReciclare();
    UnitateReciclare(std::string nm, std::string adr);

    //destructor 
    virtual ~UnitateReciclare();

    //functie de procesare
    virtual void proceseazaMaterial(Material &material) = 0;

    //afisare detalii u r
    virtual void afisareDetalii() const;
};