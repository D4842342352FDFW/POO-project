#include "SenzorGreutate.h"

//constructorii senzor greutate 
SenzorGreutate::SenzorGreutate() : Senzor("Necunoscut"), limitaGreutate(0.0) {}
SenzorGreutate::SenzorGreutate(double limita) : Senzor("Greutate"), limitaGreutate(limita) {}

//verificarea greutatii 
bool SenzorGreutate::verifica(const Material& m) const{
    return m.getGreutate() <= limitaGreutate;
}

//afisare informatii 
void SenzorGreutate::afisareInformatii() const{
    Senzor::afisareInformatii();
    std::cout << "Pragul maxim admis este: " << limitaGreutate << " kg " << std::endl;
}