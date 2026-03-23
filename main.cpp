#include <iostream>
#include "Material.h"
#include "UnitateReciclare.h"
#include "Senzor.h"
#include "SenzorContaminare.h"

int main(){
    Material pet("#2445", "Plastic", 0.1, 0.25);
    pet.afisare();
    
    SenzorContaminare s1(0.35);
    s1.afisareInformatii();
    return 0;
}
