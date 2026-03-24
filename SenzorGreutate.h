#pragma once 
#include "Senzor.h"

class SenzorGreutate : public Senzor{
    //date senzor greutate
    double limitaGreutate;

public:
    //constructorii 
    SenzorGreutate();
    SenzorGreutate(double limita);

    //verificarea greutatii 
    bool verifica(const Material& m) const override;

    //afisare informatii 
    void afisareInformatii() const override;
};