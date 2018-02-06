#include "SIMNUM.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

//Fonctions de la classe Sommet

//Constructeur par copie
Sommet::Sommet(const Sommet & S)
{
    x= S.x;
    y= S.y;
}

//Surcharge de l'opérateur =
bool Sommet::operator ==(const Sommet & A)
{
    return (x==A.x) && (y==A.y);
}

//Surcharge de l'opérateur !=
bool Sommet::operator !=(const Sommet & A)
{
    return !((*this)==A);
}

//Surcharge de l'opérateur <<
ostream & operator <<(ostream & out, const Sommet & A)
{
    out<<"("<< A.x<<","<<A.y<<")";
    return out;
}
//Surcharge de l'opérateur =
Sommet & Sommet::operator =(const Sommet & A)
{
    x = A.x;
    y = A.y;
}

//Affichage
void Sommet::print()
{
    cout <<"Le sommet est :" <<*this<<endl ;
}

//Lecture d'un sommet dans un fichier
void Sommet::Lecture(ifstream & fichier)
{
    //On entre dans le fichier et on récupère les coordonnées du sommet dans x et y
    fichier>> x>> y;
}

//Calcul de la distance entre deux sommets
double Distance(Sommet A,Sommet B)
{
    return sqrt((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y)) ;
}

//Troisième coordonnée du produit vectoriel (entre deux vecteurs = quatre sommets)
double pv(Sommet A, Sommet B,Sommet C, Sommet D)
{
    return ((B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x));
}
