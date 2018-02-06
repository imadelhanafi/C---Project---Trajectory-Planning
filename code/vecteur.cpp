#include "SIMNUM.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

//Fonctions de la classe vecteur pour Dijkstra

//Constructeur
vecteur::vecteur(int d, double v)
{
    dim = d ;
    val = NULL;

    if (d <= 0) return ;

    val = new double[d]; // allocation

    for(int i=0; i<d; i++) val[i]=v; //affectation
}

//Constructeur par copie
vecteur::vecteur(const vecteur &V)
{
    dim = V.dim ;
    val = NULL ;
    val = new double [dim]; //allocation
    for (int i=0;i<dim;i++) val[i]=V.val[i]; //affectation par copie
}

double & vecteur::operator()(int i) const
{
    return val[i];
}

//Destructeur
vecteur::~vecteur()
{if(val!=0) delete [] val;}
