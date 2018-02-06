#include "SIMNUM.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

//Fonctions de la classe Polygone
double pi = M_PI;
//Constructeur d'un polygone à 3 côtés
Polygone::Polygone(const Sommet & A, const Sommet & B, const Sommet & C)
{
    sommets.push_back(A);
    sommets.push_back(B);
    sommets.push_back(C);
}

//Constructeur d'un polygone à 4 côtés
Polygone::Polygone(const Sommet & A, const Sommet & B, const Sommet & C, const Sommet & D)
{
    sommets.push_back(A);
    sommets.push_back(B);
    sommets.push_back(C);
    sommets.push_back(D);
}

//Surcharge de l'oérateur << : on est dans une liste, on passe par un itérateur
ostream & operator <<(ostream & out, const Polygone & P)
{
    int i=0;
    list<Sommet>::const_iterator it = P.sommets.begin();
    Sommet S = *it;

    while (i!=int(P.sommets.size()))
    {
        out<<S;
        advance(it,1) ;
        S = *it;
        i++ ;
    }

    out<<endl ;

    return out;
}

//Fonction qui nous permet d'obtenir la normale entrante
Vecteur Polygone::normale(int i) const
{
    /*
    Input :
    i : entier qui correspond à l'indice du sommet qui permet de situer le côté du polygone dont on veut la normale
    Output :
    Vecteur qui correspond à la normale du côté précisé en input
    */

    if(i>int(sommets.size()))
    {
        cout<<"Erreur : "<<i<<" est plus grand que le nombre de cotés du polygone"<<endl;
        exit (-1);
    }

    //On travaille sur une liste : on a besoin de créer un itérateur
    list<Sommet>::const_iterator it = sommets.begin();

    if(i>=1) advance(it,i);

    //Premier sommet du segment considéré
    Sommet S0 = *it;

    //On traite le cas du dernier sommets Sn car à ce moment le segment considéré est celui entre Sn et S0
    if(i== int(sommets.size())-1) it = sommets.begin();
    else it ++;

    //Second sommet du segment considéré
    Sommet S1 = *it;

    //Calcul des coordonnées de la normale
    double x = S1.x - S0.x;
    double y = S1.y - S0.y;

    //Création du Vecteur qui correspond à la normale
    Vecteur V(-y,x);

    //On retourne le vecteur normalisé
    return(V /= norme(V));

}

bool Polygone::intersection(const Segment & L)
{
    /*
    On teste l'intersection entre L et le polygone courant
    Input :
    L : Segment constant
    Output :
    booléen qui vaut 1 s'il y a intersection et 0 s'il n'y a pas intersection
    */

    //Comme nous travaillons sur une liste de sommets = polygone courant, nous avons besoin d'itérateurs.
    //On les initialise.
    list<Sommet>::iterator it = sommets.begin();
    list<Sommet>::iterator ita = sommets.begin();

    //Initialisation des données
    Sommet S0 ;
    Sommet S1;
    bool R  ;
    int k = 0 ;
    double eps = 0.0001;

    //On va vérifier si A (parcourt la liste des sommets du polygone) appartient à L
    //Dès lors que A appartient à L, il y a intersection.
    //Ceci enlève le cas particulier où un sommet du polygone appartient à L.
    Sommet A;

    for(int a=0; a<int(sommets.size()); a++)
    {
        A = *ita;

        double d1 = Distance(A,L(1));
        double d2 = Distance(A,L(2));

        double d = Distance(L(1),L(2)); //longueur de L

        //Condition qui teste si A appartient à L
        if(d1<d&& d2<d && d1+d2<d+eps && d1+d2>d-eps) {return true;}

        ita++;
    }

    //On teste maintenant L avec tous les côtés du polygone
    while (k!=int(sommets.size()))
    {
        //Construction de deux sommets du polygone
        S0 = *it;

        //On traite le cas du dernier sommet du polygone
        if(k == int(sommets.size())-1)
        {
            it = sommets.begin() ;
            S1 = *it ;
        }
        //On traite tous les autres cas
        else
        {
            advance(it,1) ;
            S1 = *it ;
        }

        //On écrit la condition qui teste si L et le côté du polygone sont parallèles.
        double condition = (S1.x - S0.x)*(L(2).y-L(1).y) - (S1.y - S0.y)*(L(2).x-L(1).x) ;

        if (condition == 0) {R = false;} //Cas où les segments sont parallèles
        else
        {
            double delta = (L(2).x-L(1).x)*(S0.y-S1.y) - (L(2).y-L(1).y)*(S0.x-S1.x);//Discriminant du système de Cramer correspondant au point d'intersection.


            //Coordonnées barycentrique du point d'intersection par rapport à S0 et S1
            double alpha = (1/delta)*((S0.x-L(1).x)*(S0.y-S1.y)-(S0.y-L(1).y)*(S0.x-S1.x));
            double beta = (1/delta)*((L(2).x-L(1).x)*(S0.y-L(1).y)-(L(2).y-L(1).y)*(S0.x-L(1).x));

            //On vérifie alors si le point d'intersection est à l'intérieur de L.
            //Il y a intersection si alpha et beta sont strictement égaux à 1 ou 0.
            if (alpha <=0 || alpha >=1 || beta <=0 || beta >=1) {R = false;}
            else
            {
                R = true ;
                break;
            }
         }
         k++ ;
    }

    //On retourne le booléen
    return R ;
}

//Affichage du polygone courant
void Polygone::print()
{
    cout<<"Le polygone est "<<*this;
}

//Lecture d'un polygone dans un fichier d'entrée de forme connue
void Polygone::Lecture(ifstream & fichier)
{
    /*
    On veut lire polygone.
    Input :
    fichier : ifstream qui contient un polygone
    Output :
    Polygone correspondant
    */

    int i = 0 ;
    string s;
    int Nbsom; //Nombre de sommets dans le polygone

    //Une fois entrés dans le fichier, on passe la première ligne d'écriture et on récupère le chiffre dans Nbsom.
    fichier>> s >> Nbsom;

    //On parcourt tous les sommets :
    while (i!=Nbsom)
    {
        Sommet S;
        S.Lecture(fichier); //On lit les coordonnées du sommet S grâce à la lecture de sommet.
        sommets.push_back(S); //On ajoute S à la liste de sommets qui compose le polygone.
        i++;
    }
}

//Opérateur d'accès au ième sommet
Sommet Polygone::Acces (int i)
{
    /*
    On veut accéder au ième sommet du polygone. On est donc dans une liste : itérateur pour povoir se déplacer
    Input :
    i : entier donnant l'indice du sommet auquel on veut accéder
    Output :
    Sommet correspondant au sommet auquel on voulait accéder
    */

    list<Sommet>::iterator it = sommets.begin();

    Sommet S ;

    //Une fois l'itérateur au bon endroit dans la liste, S devient le sommet où se trouve l'itérateur.

    //Si on considère le premier sommet, l'itérateur reste où il est.
    if( i==0 ){S = *it;}

    //Si on veut accéder au sommet "-1", on considère que c'est le dernier sommet du polygone,
    //l'itérateur doit avancer jusqu'au dernier sommet.
    else if( i==-1 ){advance(it,NbSommets()-1); S = *it;}

    //Si on veut accéder au sommet "Nbsommets" on considère que l'on revient au premier sommet,
    //l'itérateur se place sur le débur de la liste.
    else if( i==NbSommets() ){it = sommets.begin(); S = *it;}

    //Si on considère un des sommets du milieu, l'itérateur se place sur le ième sommet par le biais d'une boucle.
    else
    {
        for(int j=0;j<i;j++) it++;
        S=*it;
    }

    return S ;
}


// Fonction qui vérifie si l'arc reliant deux sommets Si et Sj
// est à l'intérieur ou à l'extérieur du polygone.
// Pour traiter les polygones non convexes.
int Polygone::diag(int i, int j)
{

    if(i==j+1 || i==j-1) {return 0;}
    Sommet Si = Acces(i);
    Sommet Sj = Acces(j);
    int n = NbSommets();

    Sommet Sf = Acces(i+1); // sommet suivant (forward)
    Sommet Sb = Acces (i-1); // sommet précédant (backward)
    double pv1 = pv (Si,Sj,Si,Sf); //produit vectoriel de (Si,Sj) et (Si,Sf)
    double pv2 = pv (Si,Sj,Si,Sb); // produit vectoriel de (Si,Sj) et (Si,Sb)
	// idem
    Sommet Sf1 = Acces(j+1);
    Sommet Sb1 = Acces (j-1);
    double pv11 = pv (Sj,Si,Sj,Sf1);
    double pv21 = pv (Sj,Si,Sj,Sb1);
	// on vérifie si les sommets sont orientés dans le même sens
    if(pv1*pv2>0 && pv11*pv21>0) {return 0;} // ce n'est pas une diagonale
    else
        {
            return 1 ; // c'est une diagonale
        }
}


vector<double> Theta(Polygone &P) // vecteur contenant l'ensemble des anglais intérieurs de polygone
{
    vector<double> theta;

    for(int i=0;i<P.NbSommets();i++)
    {
        Sommet S1 = P.Acces(i);

        Sommet S2 = P.Acces(i+1);
        Sommet S0 = P.Acces(i-1);
	// Prouit sclaire
        double ps = (S2.x-S1.x)*(S0.x-S1.x)+(S0.y-S1.y)*(S2.y-S1.y);

        double d1 = Distance(S2,S1);
        double d2 = Distance(S0,S1);
	// on normalise pour avoir ps = cos(theta)
        ps = ps/(d1*d2) ;
        //acos(ps)= theta, c'est l'arccos(ps)=arccos(cos(theta))
        theta.push_back(acos(ps));
    }
    return theta ;
}
