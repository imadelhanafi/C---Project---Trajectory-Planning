#include "SIMNUM.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

//Fonctions de la classe Matrice

//Constructeur
Matrice::Matrice(int d, double v)
{
    dim = d ;
    val = NULL;
    if (d*d <= 0) return ;
    val = new double[d*d]; // allocation
    for(int i=0; i<d*d; i++) val[i]=v; //affectation

}

//Constructeur par copie
Matrice::Matrice(const Matrice& M)
{
    dim = M.dim ;
    val = NULL;
    if (dim*dim <= 0) return ;
    val = new double[dim*dim]; // allocation
    for(int i=0; i<dim*dim; i++) val[i]=M.val[i]; //affectation
}

//Opérateur d'accès
double & Matrice::operator()(int i, int j) const
{
    return val[j*dim + i];
}

//Destructeur
Matrice::~Matrice() {if(val!=0) delete [] val ;}
