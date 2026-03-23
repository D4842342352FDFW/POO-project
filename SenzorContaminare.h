#pragma once
#include "Senzor.h"

class SenzorContaminare : public Senzor{
    //date senzor contaminare 
    double pragContaminare;

public:
    //constructori senzor contaminare
    SenzorContaminare();
    SenzorContaminare(double prag);

    //override peste functiile definite in clasa senzor
    bool verifica(const Material& m) const override;
    void afisareInformatii() const override;
};