#include "SenzorContaminare.h"
#include <iostream>

//constructorii
SenzorContaminare::SenzorContaminare() : Senzor("Necunoscut"), pragContaminare(0.0) {}
SenzorContaminare::SenzorContaminare(double prag) : Senzor("Contaminare"), pragContaminare(prag) {}

//verificarea contaminarii
bool SenzorContaminare::verifica(const Material& m) const {
    return m.getGradContaminare() <= pragContaminare;
}

//afisarea informatiilor suplimentare
void SenzorContaminare::afisareInformatii() const {
    Senzor::afisareInformatii();
    std::cout << "Pragul maxim admis este: " << pragContaminare * 100 << " % " << std::endl;
}