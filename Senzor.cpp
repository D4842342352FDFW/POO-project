#include "Senzor.h"
#include <iostream>

//constructorii 
Senzor::Senzor() : tipSenzor("Necunoscut") {}
Senzor::Senzor(std::string tSen) : tipSenzor(tSen) {}

//destructor
Senzor::~Senzor() {}

//afisare tip senzor
void Senzor::afisareInformatii() const{
    std::cout << "Senzor " << tipSenzor << std::endl;
}