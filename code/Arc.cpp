#include "SIMNUM.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

//Fonctions de la classe Arc

//Constructeur d'un arc à partir de deux sommets
ArcPlanification::ArcPlanification(const Sommet & A, const Sommet & B)
{
    S1 = A ;
    S2 = B ;

    l = Distance(A,B) ;
}
