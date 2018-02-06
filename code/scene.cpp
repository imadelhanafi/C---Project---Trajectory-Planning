#include "SIMNUM.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

//Fonctions de la classe scene

//Affichage de la scene courante par polygone.
//On se place donc sur la liste des polygones "polygones" de la scene et on utilise un itérateur
void scene::print()
{
    cout<<"La scène est \n";

    list<Polygone>::const_iterator it = polygones.begin();

    for(;it != polygones.end(); it++)
    {cout<<*it;}

    cout<<endl;
}

//Lecture de la scene dans un fichier d'entrée de forme connue
void scene::Lecture(ifstream & fichier)
{
    /*
    On veut lire la scène entière.
    Input :
    fichier : ifstream qui contient un polygone
    Output :
    Polygone correspondant
    */

    string s;
    int Nbobst; //Nombre d'obstacles dans la scene
    int Nbsomscene; //Nombre de sommets total dans la scene

    //Une fois entrés dans le fichier, on passe la première ligne d'écriture et on récupère le chiffre dans Nbobst.
    fichier>>s>>Nbobst;

    //Pour récupérer les polygones, on boucle sur les obstacles
    for (int i=0;i<Nbobst;i++)
    {
        Polygone P;
        P.Lecture(fichier); //On lit le polygone grâce à la lecture de polygone.

        //On calcule le nombre de sommets total dans la scene.
        //On ajoute à chaque fois le nombre de sommets du polygone que l'on lit
        Nbsomscene += P.NbSommets();

        polygones.push_back(P); //On ajoute le polygone à la liste.

        //On stocke les sommets du polygone dans une liste.
        list<Sommet> S = P.sommets;

        //On veut les récupérer pour les stocker dans la liste des sommets de la scene.
        //On utilise un itérateur et on boucle afin de les ajouter un par un.
        list<Sommet>::const_iterator it=S.begin();

        for(; it!= S.end(); it++)
        {
            Sommet s = *it;
            sommetsPol.push_back(s);
        }
    }

    //On ferme le fichier
    fichier.close();
}

//Opérateur d'accès au ième sommet
const Sommet& scene::Acces(int i) const
{
    /*
    On veut accéder au ième sommet de la scene. On est donc dans une liste : itérateur pour povoir se déplacer
    Input :
    i : entier donnant l'indice du sommet auquel on veut accéder
    Output :
    Sommet correspondant au sommet auquel on voulait accéder
    */

    list<Sommet>::const_iterator it = sommetsPol.begin();

    for(int j=0;j<i;j++) it++;

    return *it;
}

//Opérateur d'accès au pème polygone
const Polygone& scene::AccesPol(int p) const
{
    /*
    On veut accéder au pème polygone. On est donc dans une liste : itérateur pour povoir se déplacer
    Input :
    p : entier donnant l'indice du polygone auquel on veut accéder
    Output :
    Polygone correspondant au polygone auquel on voulait accéder
    */

    list<Polygone>::const_iterator it = polygones.begin();

    for(int j=0;j<p;j++) it++;

    return *it;
}


 //scene::scene( const scene &S)
//{
//    scene S1 ; // S1 = S_copy
//
//    list<Sommet>::const_iterator its = sommetsPol.begin();
 //   list<Polygone>::const_iterator itp = polygones.begin();


//    list<Sommet>::const_iterator its1 = S1.sommetsPol.begin();
//    list<Polygone>::const_iterator itp1 = S1.polygones.begin();

//    while(its!=sommetsPol.end())
//    {
//    *its1 = *its;
 //   ++its1; ++its;
 //   }
 //   while(itp!=polygones.end())
  //  {
  //  *itp1 = *itp;
  //  ++itp1; ++itp;
  //  }


//}
