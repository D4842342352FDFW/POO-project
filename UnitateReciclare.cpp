#include "UnitateReciclare.h"
#include <iostream> 

int UnitateReciclare::contorId = 1;

//constructor default unitate reciclare
UnitateReciclare::UnitateReciclare() : nume("Necunoscut"), adresa("Necunoscuta"), gradFunctionalitate(false)
{
    id = contorId++;
}

//constructor cu parametri unitate reciclare
UnitateReciclare::UnitateReciclare(std::string nm, std::string adr) : nume(nm), adresa(adr), gradFunctionalitate(true)
{
    id = contorId++;
}

//destructor unitate reciclare
UnitateReciclare::~UnitateReciclare()
{

}

// affiare detalii unitate reciclare

void UnitateReciclare::afisareDetalii() const{
    std::cout << "Unitatea " << id << "Nume " << nume << " Adresa " << adresa 
    << (gradFunctionalitate ? "Functinala" : "Nefunctionala") << std::endl; 
}
