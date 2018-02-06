#include "SIMNUM.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

//Fonctions de la classe Segment

//Constructeur par copie
Segment::Segment(const Sommet & A, const Sommet & B)
{
    sommets.resize(2);
    sommets[0]= A;
    sommets[1]= B;
}

//Surcharge de l'opérateur <<
ostream & operator <<(ostream & out, const Segment & S)
{
    out<<"("<< S(1)<<","<<S(2)<<")";
    return out;
}

//Affichage d'un segment
void Segment::print()
{
    cout << "Le segment lie les points "<<*this<<endl ;
}
