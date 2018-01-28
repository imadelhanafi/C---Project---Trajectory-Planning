#include <iostream>
#include <fstream>
#include "SIMNUM.hpp"
#include "dijkstra.h"
#include <string>


using namespace std;


// fonction aux pour enregistrer la solution dans un fichier sol.txt

void affichage(Sommet& D, Sommet&A,const vector<int>& Sol,const scene& Sc,int mode)
{

ofstream outputFile("sol.txt");
outputFile<<"depart\n"<<D.x<<" "<<D.y<<endl;
outputFile<<"solution\n";

int t = Sol.size()-2;// Nb de point autre que A et D
outputFile<<t<<endl;

for(int k=Sol.size()-2 ; k>0; k--)
    {
    Sommet S = Sc.Acces(Sol[k]-1);
    outputFile<<S.x<<" "<<S.y<<endl;
    }
outputFile<<"arrivee\n"<<A.x<<" "<<A.y<<endl;
outputFile.close();
//cout<<"open"<<endl;
if(mode ==1) {
system("matlab  -r affichage ");
}
else
{
system("matlab  -r affichage1 ");
}

}



int main()
{

// Déclarer les sommets (départ & arrivée)
Sommet D ;
Sommet A ;

int mode;  // = 1 padding,  =0 sans passing


cout<<"#############PLANIFICATION DES TRAJECTOIRES#################"<<endl;
cout<<"#################PROJET MAP-SIMNUM - ENSTA##################"<<endl;
cout<<"##############Plus de détails : ReadMe.txt#################"<<endl;
cout<<"############################################################"<<endl;
cout<<endl;
cout<<endl;
cout<<"veuillez choisir la scene dans le fichier test.txt"<<endl;
cout<<"############################################################"<<endl;

cout<<"veuillez sélectionner le point de départ :"<<endl;
cout<<"x départ"<<endl;
cin >>D.x;
cout<<"y départ"<<endl;
cin>>D.y;

cout<<"############################################################"<<endl;

cout<<"veuillez sélectionner le point d'arrivée :"<<endl;
cout<<"x arrivée"<<endl;
cin >>A.x;
cout<<"y arrivée"<<endl;
cin>>A.y;

cout<<"############################################################"<<endl;
cout<<"############################################################"<<endl;
cout<<"############################################################"<<endl;
cout<<"MODE : "<<endl;
cout<<"Avec Padding : 1"<<endl;
cout<<"Sans Padding : 0"<<endl;
cin >>mode;
///////////Lecture scene
scene sc1("test.txt");

///////////////////////////////////////////////

///Travailler avec une copie de D et A !!!

//Graphe G = ConstructGraphe1(sc1,D,A) ;

if(mode ==1)
{

double r = 0.2; // le padding
scene S ;
Graphe G = ConstructGraphe2(sc1,D,A,r,S) ;
int N = int(S.sommetsPol.size())+2 ;

///  dijkstra////////////////////////////////////
vector<int> Solution ;
dijkstra(G.Mglob,N-1,0,N,Solution);
cout<<"Notre vector solution obtenu après dijsktra ==> Donne les indices des sommets de la solution"<<endl;
for(int k=0;k<int(Solution.size());k++)
{
    cout<<Solution[k]<<"\t"<<endl;
}
///////////////////////////////////////////////////////////////
/// Affichage --> On va rajouter le départ et arriver dans le fichier source :
cout<<"avant open"<<endl;
//affichage(D,A,Solution,sc1);
affichage(D,A,Solution,S,mode);

}

else
{

Graphe G = ConstructGraphe1(sc1,D,A) ;
int N = int(sc1.sommetsPol.size())+2 ;

///  dijkstra////////////////////////////////////
vector<int> Solution ;
dijkstra(G.Mglob,N-1,0,N,Solution);
cout<<"Notre vector solution obtenu après dijsktra ==> Donne les indices des sommets de la solution"<<endl;
for(int k=0;k<int(Solution.size());k++)
{
    cout<<Solution[k]<<"\t"<<endl;
}
///////////////////////////////////////////////////////////////
/// Affichage --> On va rajouter le départ et arriver dans le fichier source :
cout<<"avant open"<<endl;
//affichage(D,A,Solution,sc1);
affichage(D,A,Solution,sc1,mode);

cout<<"Affichage de la matrice des couts pour un carré"<<endl ;
for(int i=0;i<6;i++)
{
    for(int j=0;j<6;j++)
    {
        cout<<G.Mglob(i,j)<<"\t" ;
    }
    cout<<"\n";
}
cout<<endl;

}

}
