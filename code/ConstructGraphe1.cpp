
#include "SIMNUM.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

Graphe ConstructGraphe1(const scene& S, Sommet& D, Sommet& A)
{
    /*
    Input :
    S : la scène dont on veut connaitre le graphe
    D : le sommet de départ
    A : le sommet d'arrivée
    Output :
    Graphe correspondant à la scène
    */

    int Nsom = int(S.sommetsPol.size()); //Nombre de noeuds dans le graphe sans le départ et l'arrivée
    int Nbobst = int(S.polygones.size()); //Nombre d'obstacles total

    //Création d'un graphe vide
    Graphe G(Nsom+2);
    //Création d'une matrice de coût pour le stockage de la matrice de cout que pour les polygones
    Matrice Mcout(Nsom,1000000000);

    //On fixe les termes de la diagonales à O
    for(int i=0; i<Nsom+2; i++){G.Mglob(i,i)=0;}
    for(int i=0; i<Nsom; i++){Mcout(i,i)=0;}


    /// On teste si A et/ou D appartient à l'un des polygones

    for(int i=0;i<Nbobst;i++)
    {
        double eps = 0.00000000000001 ; // eps doit etre << les autres EPS

        Polygone P = S.AccesPol(i); //On se place sur le ième polygone

        for(int j=0; j<P.NbSommets();j++)
        {
            Sommet S0 = P.Acces(j); //On se place sur le jième sommet

            ///Cas où A et/ou D est sur le jieme sommet : on décale infimement A et/ou D
            if(D==S0)
            {
                Vecteur n1 = P.normale(j-1);
                Vecteur n2 = P.normale(j);
                D.x += (-(n1.x+n2.x))*eps ;
                D.y += (-(n1.y+n2.y))*eps ;
            }

            if(A==S0)
            {
                Vecteur n1 = P.normale(j-1);
                Vecteur n2 = P.normale(j);
                A.x += (-(n1.x+n2.x))*eps ;
                A.y += (-(n1.y+n2.y))*eps ;
            }

            ///Cas où A et/ou D est sur l'un des segments : on décale infimement A et/ou D
            //On utilise la même méthode que dans intersection pour teste si un sommet appartient à un segment
            Sommet S1 = P.Acces(j+1); //On se place sur le jième sommet
            double d = Distance(S0,S1);

            //Pour D :
            double d1 = Distance(D,S0);
            double d2 = Distance(D,S1);

            if(d1<d && d2<d && d1+d2==d)
            {
                Vecteur n = P.normale(j);
                D.x += (-n.x)*eps;
                D.y += (-n.y)*eps;
            }

            //Pour A :
            double d3 = Distance(A,S0);
            double d4 = Distance(A,S1);

            if(d3<d && d4<d && d3+d4==d)
            {
                Vecteur n = P.normale(j);
                A.x += (-n.x)*eps;
                A.y += (-n.y)*eps;
            }

        }

    }

    ////////////////////////////////////////////////////////////////////

    ///On va remplir la matrice globale : on remplit les case où A et/ou D sont concernés

    ///On relie D à tous les points de la liste de sommets de la scene
    for(int i=0; i< int(S.sommetsPol.size());i++)
    {
        Segment I(D, S.Acces(i)); //Segment entre D et le Ième sommet de la liste de sommets de la scene
        int compteur = 0; //Pour voir avec combien de polygones il y a intersection

        //A partir du segment créé, on teste s'il y a intersection avec les polygones
        for(int j = 0; j<Nbobst; j++)
        {
            Polygone P = S.AccesPol(j);
            if(P.intersection(I) == 1) compteur += 1;
        }

        //S'il n'y a aucune intersection avec les polygones, on remplit la matrice de couts globale et on ajoute l'arc dans la liste d'arcs
        if(compteur == 0)
        {
            ArcPlanification *J = new ArcPlanification(D,S.Acces(i));

            G.Mglob(0,i+1)= J->l;
            G.Mglob(i+1,0)= J->l;

            G.arcs.push_back(J);
        }
    }


    ///De la même manière, on relie A à tous les points de la liste de sommets de la scene
    for(int i=0; i< int(S.sommetsPol.size());i++)
    {
        Segment I(A, S.Acces(i)); //Segment entre A et le Ième sommet de la liste de sommets de la scene
        int compteur = 0; //Pour voir avec combien de polygones il y a intersection

        for(int j = 0; j<Nbobst; j++)
        {
            Polygone P = S.AccesPol(j);
            if(P.intersection(I) == 1) compteur += 1;
        }

        //S'il n'y a aucune intersection avec les polygones, on remplit la matrice de couts globale et on ajoute l'arc dans la liste d'arcs
        if(compteur == 0)
        {
            ArcPlanification *J =new ArcPlanification(A,S.Acces(i));

            G.Mglob(Nsom+1,i+1)= J->l;
            G.Mglob(i+1,Nsom+1)= J->l;

            G.arcs.push_back(J);
        }
    }

    ///On s'occupe désormais du segment entre A et D
    Segment I(D,A); //Segment entre D et A
    int compteur = 0; //Pour voir avec combien de polygones il y a intersection

    for(int j = 0; j<Nbobst; j++)
    {
        Polygone P = S.AccesPol(j);
        if(P.intersection(I) == 1) compteur += 1;
    }

    //S'il n'y a aucune intersection avec les polygones, on remplit la matrice de couts globale et on ajoute l'arc dans la liste d'arcs
    if(compteur == 0)
    {
        ArcPlanification *J = new ArcPlanification(D,A);

        G.Mglob(0,Nsom+1)= J->l;
        G.Mglob(Nsom+1,0)= J->l;

        G.arcs.push_back(J);
    }

    ////////////////////////////////////////////////////////////////////

    ///On va remplir la matrice de couts de la scene

    int nbr_som_current=0; //utile pour se déplacer dans la liste des sommets de la scene
    double eps2 = 0.0001;

    ///On s'occupe d'abord des arcs au sein d'un même polygone
    for (int i=0;i<Nbobst;i++)
    {
        Polygone P = S.AccesPol(i); //On se place sur le ième polygone
        nbr_som_current+=P.NbSommets(); //Mise à jour pour pouvoir l'utliser dans le remplissage de la matrice des couts

        for(int j=0; j<P.NbSommets(); j++) //Boucle sur les sommets du polygone i
        {
            Sommet S1 = P.Acces(j); //On se place sur le jème sommet du polygone i

            for(int k=j+1; k<P.NbSommets();k++) //Boucle sur les sommets d'après dans le polygone
            {
                Sommet S2 = P.Acces(k); //On se place sur le kème sommet du polygone i
                Segment I(S1, S2);

                int res = 0; //Pour voir avec combien de polygones il y a intersection

                //A partir du segment créé, on teste s'il y a intersection avec les polygones
                for(int h=0;h<Nbobst;h++)
                {
                    Polygone Q = S.AccesPol(h);
                    if(Q.intersection(I) == 1) res+=1;
                }

                //Ici on va tester si le segment est à l'intérieur ou à l'extérieur du polygone.
                //C'est utile pour garder des plus courts chemins dans les polygones non convexes.
                Vecteur n1 = P.normale(j-1);
                Vecteur n2 = P.normale(j);
                double x = S2.x-S1.x;
                double y = S2.y-S1.y;


                //Si c'est à l'extérieur, si il n'y a pas d'intersection et si ce n'est pas une diagonale
                //alors on met à jour la matrice des couts.
                if((n1.x*x+n1.y*y)*(n2.x*x+n2.y*y)<=eps2 && res == 0 && P.diag(j,k)==0)
                {
                    ArcPlanification *J =new  ArcPlanification(S1,S2);

                    Mcout(nbr_som_current-P.NbSommets()+j,nbr_som_current-P.NbSommets()+k) = J->l;
                    Mcout(nbr_som_current-P.NbSommets()+k,nbr_som_current-P.NbSommets()+j) = J->l;

                    G.arcs.push_back(J);
                }

                //Si c'est à l'intérieur, si il n'y a pas d'intersection et si c'est une diagonale
                //alors les coefficients de la matrice des couts correspondants sont infinis
                if((n1.x*x+n1.y*y)*(n2.x*x+n2.y*y)>eps2 && res==0 && P.diag(j,k)==1)
                {
                    Mcout(nbr_som_current-P.NbSommets()+j,nbr_som_current-P.NbSommets()+k) = 1000000000 ;
                    Mcout(nbr_som_current-P.NbSommets()+k,nbr_som_current-P.NbSommets()+j) =1000000000  ;
                }
            }
        }
    }



    ///On s'occupe enfin des arcs entre les polygones

    nbr_som_current =0; //Réinitilisation

    for (int i=0;i<Nbobst;i++)
    {
        Polygone P = S.AccesPol(i); //On se place sur le ième polygone
        nbr_som_current+=P.NbSommets(); //Mise à jour pour pouvoir l'utliser dans le remplissage de la matrice des couts

        for(int j=0; j<P.NbSommets();j++) //Boucle sur les sommets du polygone i
        {
            for(int k=nbr_som_current; k<int(S.sommetsPol.size());k++) //Boucle sur les sommets des polygones suivants
            {
                Segment I(P.Acces(j), S.Acces(k)); //Ici il y a deux niveaux pour les sommets : sommet du polygone et sommet de la scene
                int compteur = 0; //Pour voir avec combien de polygones il y a intersection

                //A partir du segment créé, on teste s'il y a intersection avec les polygones
                for(int h=0;h<Nbobst;h++)
                {
                    Polygone Q = S.AccesPol(h);
                    if(Q.intersection(I) == 1) compteur+=1;
                }

                //S'il n'y a aucune intersection avec les polygones, on remplit la matrice de couts de la scene et on ajoute l'arc dans la liste d'arcs
                if(compteur==0)
                {
                    ArcPlanification *J = new ArcPlanification(P.Acces(j),S.Acces(k));

                    Mcout(nbr_som_current-P.NbSommets()+j,k) = J->l;
                    Mcout(k,nbr_som_current-P.NbSommets()+j) = J->l;

                    G.arcs.push_back(J);
                }
            }
        }
    }


    ////////////////////////////////////////////////////////////////////

    ///On remplit la matrice de couts globale à partir de la matrice de couts de la scene
    for(int i=0;i<Nsom;i++)
    {
        for(int j=0;j<Nsom;j++)
        {
            G.Mglob(i+1,j+1) = Mcout(i,j);
        }
    }

    return G;

}

