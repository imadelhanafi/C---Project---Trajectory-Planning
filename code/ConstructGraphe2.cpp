
#include "SIMNUM.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////
///	Cette première partie correspond au padding		///
///////////////////////////////////////////////////////////////////

// En plus des arguments de ConstructGraphe1, on rajoute le double R (le rayon de l'objet)
// et aussi une nouvelle scène S, qui sera celle du padding (avec les polygones modifiés)
Graphe ConstructGraphe2(const scene& S1,Sommet& D, Sommet& A, double R,scene &S )
{


int Nbobst1 = int(S1.polygones.size()); //nb obstacle total
double pi = M_PI;
int n = 12 ;// Nombre de discrétisation pour les coins arrondis

for(int i=0;i<Nbobst1;i++)
{
    Polygone P = S1.AccesPol(i); //on recopie les polygones de la scene S1
    vector<double> theta ; // on crée le vecteur des angles
    theta = Theta(P) ; // on recupère les angles intérieures du polygone
    Polygone P1 ;
    Vecteur n1 ;
    for(int j=0; j<P.NbSommets();j++)
    {
        if(j==0) // on récupère le premier vecteur normale
        {
            n1 = P.normale(int(P.NbSommets())-1);
        }
        else
        {
            n1 = P.normale(j-1);
        }

        Sommet S0 = P.Acces(j); // on se place sur un sommet de polygone
        double thetaj = theta[j];

	// 1er cas: 0 < theta <= pi/2
        if(thetaj<= pi/2 && thetaj>0)

        {
                Sommet S1 ;
                Vecteur n2 ;
                n2 = P.normale(j);
                /// On recupère les directrices.
                Vecteur D2;

                D2.x = -n2.y ;
                D2.y = n2.x ;

                S1.x = S0.x + D2.x*R ;
                S1.y = S0.y + D2.y*R ;

                P1.sommets.push_back(S1);
                S.sommetsPol.push_back(S1);

	// on réalise la discrétisation de l'angle arrondi
	// on crée plusieurs sommets dans le sens trigonométrique
        for(int k=1;k<=n;k++)
            {
                Sommet Sk ;
                Sk.x = (S1.x-S0.x)*cos(theta[j]*(k)/(n)) - (S1.y-S0.y)*sin(theta[j]*(k)/(n)) + S0.x ;
                Sk.y = (S1.x-S0.x)*sin(theta[j]*(k)/(n)) + (S1.y-S0.y)*cos(theta[j]*(k)/(n)) + S0.y ;

                P1.sommets.push_back(Sk);
                S.sommetsPol.push_back(Sk);
            }
        }
	// 2ème cas: pi/2 < theta < pi
        if(thetaj>pi/2 && thetaj< pi)
        {
                Sommet SAv = P.Acces(j-1);
                Sommet SAp = P.Acces(j+1);
                double d1 = Distance(SAv,S0);
                double d2 = Distance(SAp,S0) ;


                Sommet S1 ;
                Vecteur n2 ;
                n2 = P.normale(j);
                // On recupère les directrices.

                Vecteur D1,D2,U ;

                D1.x = n1.y ;
                D1.y = -n1.x ;

                D2.x = -n2.y ;
                D2.y = n2.x ;
		// on récupère le vecteur de la bissectrice de l'angle extérieur
                U.x = D1.x+D2.x;
                U.y = D2.y+D1.y ;

                U /= norme(U) ;
		// 1er sous-cas: R < {d1 et d2}
        if(d1 > R && d2> R)
                {
                /// On translate S0

                S1.x = S0.x + D2.x*R ;
                S1.y = S0.y + D2.y*R ;

                for(int k=3;k<=n-3;k++) // la discrétisation en sens trigonométrique
					// on enlève les premièrs et derniers points pour éviter que ça coupe avec les arêtes
                    {
                    Sommet Sk ;
                    Sk.x = (S1.x-S0.x)*cos(theta[j]*(k)/(n)) - (S1.y-S0.y)*sin(theta[j]*(k)/(n)) + S0.x ;
                    Sk.y = (S1.x-S0.x)*sin(theta[j]*(k)/(n)) + (S1.y-S0.y)*cos(theta[j]*(k)/(n)) + S0.y ;

		// on on rajoute les sommets à chaque itération dans la liste des sommets de la scène et du polygone
                    P1.sommets.push_back(Sk);
                    S.sommetsPol.push_back(Sk);
                    }

                }
		// 2ème sous-cas: R => {d1 ou d2}
        else
            {
		// on translate le sommet suivant la bissectrice de l'angle extérieur
                S1.x = S0.x + R*U.x ;
                S1.y = S0.y + R*U.y ;

		// on rajoute ce sommet à la liste des sommets de la scène et du polygone
                P1.sommets.push_back(S1);
                S.sommetsPol.push_back(S1);
            }

        }
	// 3ème cas: pi < theta < 3*pi/2
	// le cas où theta = pi n'existe pas, car il n'y aura pas de sommet
        if(thetaj<= 2*pi && thetaj> pi)
        {

                Sommet SAv = P.Acces(j-1); // sommet précédant (avant)
                Sommet SAp = P.Acces(j+1); // sommet suivant (après)
                double d1 = Distance(SAv,S0);
                double d2 = Distance(SAp,S0) ;

                Sommet S1 ;
                Vecteur n2 ;
                n2 = P.normale(j);
                // On recupère les directrices.
                Vecteur D1,D2,U ;
                D1.x = -n1.y ;
                D1.y = n1.x ;

                D2.x = n2.y ;
                D2.y = -n2.x ;
		// on récupère la bissectrice

                U.x = D1.x+D2.x;
                U.y = D2.y+D1.y ;
                U /= norme(U) ;
		// on récupère la plus courte longueur entre le sommet en question et le centre de l'obstacle
		// c'est aussi la distance dont on va translater le sommet
                double alpha = pi-(thetaj/2); // l'angle extérieur/2


                double l = R/sin(alpha) ;
		// translation du sommet suivant la médiane

                S1.x = S0.x + l*U.x ;
                S1.y = S0.y + l*U.y ;
		// on rajoute le sommet
                P1.sommets.push_back(S1);
                S.sommetsPol.push_back(S1);
        }

    }
        S.polygones.push_back(P1); // on rajoute le polygone dans la nouvelle scène

}

///////////////////////////////////////////////////////////////////
///	La suite correspond exactement à ConstructGraphe1	///
///////////////////////////////////////////////////////////////////

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
                if((n1.x*x+n1.y*y)*(n2.x*x+n2.y*y)>eps2 && res == 1 && P.diag(j,k)==1)
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

