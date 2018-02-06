#ifndef SIMNUM_HPP_INCLUDED
#define SIMNUM_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include<limits>



using namespace std;


class Vecteur
{
    public :
    double x,y; //Coordonnées du vecteur

    //Constructeur
    Vecteur(double a=0, double b =0):x(a),y(b){};
    Vecteur(const Vecteur &);

    //Surcharge des opérateurs qui nous seront utiles
    Vecteur & operator *=(double);
    Vecteur & operator /=(double);

    //Norme d'un vecteur
    friend double norme(const Vecteur & V);
};


class vecteur //Pour Dijkstra
{
    public :
    int dim; //Dimension du vecteur
    double *val; //Tableau des valeurs

    //Constructeurs
    vecteur(int , double v = 0);
    vecteur(const vecteur&);

    //Opérateur d'acces
    double & operator()(int) const;

    //Destructeur
    ~vecteur();
};


class Matrice
{
    public :
    int dim; //Dimension de la matrice
    double *val; // Tableau de valeurs

    //Constructeur
    //On choisit une valeur initiale très grande pour représenter les arcs infinis
    Matrice(int d= 0, double v = 1000000000);
    Matrice(const Matrice&);


    //Opérateur d'acces
    double & operator ()(int, int) const;

    //Destructeur
    ~Matrice();
};


class Sommet
{
    public :
    double x,y; //Coordonnées du sommet

    //Constructeur et constructeur par copie
    Sommet(double a=0, double b=0):x(a),y(b){};
    Sommet(const Sommet &);

    //Surcharge des opérateurs qui nous seront utiles
    bool operator ==(const Sommet &);
    bool operator !=(const Sommet &);
    Sommet & operator =(const Sommet &);
    friend ostream & operator <<(ostream & out, const Sommet &);


    //Affichage d'un sommet
    void print() ;

    //Lecture d'un sommet dans un fichier
    void Lecture(ifstream &);
};

//Calcul de la distance entre deux sommets
double Distance(Sommet A, Sommet B);

//Fonction qui récupère la troisième coordonnée du produit vectoriel
double pv(Sommet, Sommet,Sommet, Sommet);


class Segment
{
    public :
    vector<Sommet> sommets; //On considère qu'un segment est composé de deux sommets

    //Constructeurs
    Segment(){}
    Segment(const Sommet &, const Sommet &);

    //Opérateurs qui nous seront utiles
    Sommet & operator ()(int i){return sommets[i-1];}
    const Sommet & operator ()(int i) const {return sommets[i-1];}
    friend ostream & operator <<(ostream & out, const Segment &);

    //Affichage d'un segment
    void print();
};


class Polygone
{
    public :
    list<Sommet> sommets; //On considère qu'un polygone est un ensemble de sommets

    //Constructeurs
    Polygone(){}
    Polygone(const Sommet &, const Sommet &, const Sommet &);
    Polygone(const Sommet &, const Sommet &, const Sommet &, const Sommet &);
    Polygone(const string & fic)
    {
        ifstream fichier(fic.c_str());
        Lecture(fichier);
    }

    //Fonction qui ajoute un sommet pour pouvoir avoir des polynomes de taille quelquonque
    void add(const Sommet & A){sommets.push_back(A);}

    //Pour calculer le nombre de sommets du polygone
    int NbSommets() {return sommets.size();} ;

    //Surcharge des opérateurs qui nous seront utiles
    friend ostream & operator <<(ostream & out, const Polygone &);

    //Pour pouvoir savoir si nous sommes à l'intérieur ou à l'extérieur du polygone
    Vecteur normale(int i) const;

    //Test de l'intersection entre un segment et le polygone courant
    bool intersection(const Segment &);

    //Affichage d'un polygone
    void print();

    //Lecture d'un polygone à partir d'un fichier (on n'a finalement pas besoin de nos constructeurs)
    void Lecture(ifstream &);

    //Comme polygones est une liste, on crée un acces au ième sommet
    Sommet Acces(int i);

    //On vérifie si un arc est à l'intérieur ou à l'extérieur du polygone
    int diag(int i, int j) ;
};

// On récupère l'ensemble des angles intérieurs du polygone
vector<double> Theta(Polygone &P);


class scene
{
    public:
    list<Polygone> polygones; //Pour avoir la liste des polygones de la scene
    list<Sommet> sommetsPol; //Pour avoir la liste de sommets de la scene
    int Nbsomscene; //Nombre de sommets dans la scene

    //Constructeur par défaut et constructeur par lecture de fichier
    scene(){}
    scene(const string & fic)
    {
        ifstream fichier(fic.c_str());
        Lecture(fichier);
    }

    //Constructeur par copie
    //scene(const scene&);

    //Affichage de la scene
    void print();

    //Lecture de la scene à partir d'un fichier, utilisation des autres lectures
    void Lecture(ifstream &);

    //On crée un acces au ieme sommet de sommetsPol et au pieme polygone de polygones
    const Sommet& Acces(int i) const;
    const Polygone& AccesPol(int p=0) const;
};


class Arc
{
};


class ArcPlanification : public Arc
{
    public :
    Sommet S1,S2 ; //Un arc, c'est deux sommets et
    double l ; //une longueur

    //Constructeur
    ArcPlanification(const Sommet & A1, const Sommet & A2);

    //Affichage d'un arc
    void print() ;
};


class Graphe
{
    public :
    list<Arc*> arcs; //Un graphe, c'est une liste d'arcs et
    Matrice Mglob; //une matrice de couts

    //Constructeurs
    Graphe(){};
    Graphe(int N):Mglob(N,1000000000){};
};

//Fonction qui construit le graphe (liste d'arcs + matrice de couts) dans le cas de base
Graphe ConstructGraphe1(const scene&, Sommet&, Sommet&);

//Fonction qui construit le graphe (liste d'arcs + matrice de couts) dans le cas du padding
Graphe ConstructGraphe2(const scene& ,Sommet& , Sommet& , double ,scene &);

#endif // SIMNUM_HPP_INCLUDED
