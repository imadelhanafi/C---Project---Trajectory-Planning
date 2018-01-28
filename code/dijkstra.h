#ifndef DIJKSTRA_H_INCLUDED
#define DIJKSTRA_H_INCLUDED

#include <iostream>
#include "SIMNUM.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <math.h>
#include <limits>
using namespace std;


double INF = 1000000000;

// fonction pour calculer l'indice du min parmis les indices 'false' (c'est à dire non visité)

int minDistance(const vecteur & dist, bool Set[],int N)
{
   //Initialise la valeur minimale
   double min = numeric_limits<double>::max(), min_index;
   for (int v = 0; v < N; v++)
   {
        if (Set[v] == false && dist(v) <= min) min = dist(v), min_index = v;
   }
   return min_index;
}

// fonction pour affichier
void print(const  vecteur & dist,int N)
{
	printf("Le point   Distance from Source\n");
   	for (int i = 0; i < N; i++) cout<<i<<" \t\t" <<dist(i)<<"\n";
}



//Algorithme de Dijkstra

void dijkstra(const Matrice & c, int arr, int src, int N,vector<int>& Solution)
{
    // Matrice c : la matrice des couts
    // N : Nombre des sommets U {Arrivée et départ}
    // Src : l'indice de départ, dans notre problème src == 0
    // Arr : l'indice d'arrivée, dans notre cas c'est arr== N-1
	vecteur dist(N);
	// le vecteur qui contient les distance à partir de SRC
	bool Set[N] ;
    // Pour marquer les points qui sont visités
	vecteur p(N) ;
	// le vecteur p pour memoriser le chemin pour aller de 'src' à 'arr'
	p(src) =0;
	//Initialisation

	for(int i=0; i<N;i++)
	{
        dist(i)= numeric_limits<double>::max() ;
        Set[i] = false ;
         // à l'instant 0 on a rien visité
	}
	dist(src) = 0;

	// boucle sur les sommets
	for(int j=0;j<N;j++)
	{
        //Chercher le minimum parmis les sommets non visité // 1er iteration u est la source
        int u = minDistance(dist,Set,N);
        Set[u] = true ;

        // update la distance avec les adjacent et appliquer les iterations indiqué dans la page4 du projet
        double max = numeric_limits<double>::max() ;
        for(int v=0; v<N;v++)
        {
            if(!Set[v] && c(u,v) < INF && c(u,v)>0 && dist(u) != max&& dist(u)+c(u,v)<dist(v))
            {
                dist(v) = dist(u)+c(u,v); p(v)= u;
            }
        }
	}

	/// Partie affichage de la solution
    print(dist, N); //afficher les distances from source
    int  j = arr ;
    cout<<"Solution :"<<endl;
    cout<<"le cout:"<<endl;
    cout<<dist(j)<<endl ;
    cout<<"Trajet : "<<endl;
    cout<<j<<endl;
    Solution.push_back(j) ;
    while(j!=0)
    {
        j=p(j) ;
        cout<<j<<endl ;
        Solution.push_back(j) ;
    }
}

#endif // DIJKSTRA_H_INCLUDED

