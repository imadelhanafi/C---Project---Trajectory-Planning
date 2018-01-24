Planification des trajectoires - Projet C++
===================

*(Description and report in french)*

## C'est quoi ? ##


Ce projet est realisé dans le cadre du cours MAP-SIM2 à l'ENSTA ParisTech Université Paris-Saclay.
Dans ce projet on a utlisé l'algorithme de Dijkstra pour trouver le chemin minimal dans un environnement qui contient plusieurs obstacles.


Features
-------------

Ce projet propose deux options pour résoudre le problème de planification.

> **Les modes:**

> - **Mode = 0** : La solution est donnée pour un objet ponctuel.
> - **Mode = 1** : La solution proposée correspond à un objet circulaire de rayon r ( par défaut r=0.2, dans la partie "usage" on va expliquer comment choisir un rayon quelconque).

> **Environnments :**

L'environnement est défini par des polygones dans le sens trigonométrique.
Pour définir un evironnement, veuillez modifier le fichier **test.txt** sous la forme suivante :

```c
#Nombreobstacles
//Indiquer le nombre d'obstacle (un obstacle est un polygone)
#Polygone
// indiquer le nombre de sommets dans ce polygone
//x1 y1 // les cordonnees des sommets dans l'ordre trigonométrique 
//x2 y2
//x3 y3
#Polygone
// indiquer le nombre de sommets dans ce polygone
//x1 y1    
//x2 y2
//x3 y3
```

Requirements 
-------------

Ce projet est réalisé en **C++** (Code::Blocks 10.05) et on utilise **MatLab** pour l'affichage.

How to use  
-------------
> **Définir l'environnment :**

Pour utliser notre projet vous avez besoin de définir d'abord un environnement dans **test.txt**


> **Choisir un padding (Par défaut c'est 0.2) :**

Dans le fichier **main.cpp** vous pouvez choisir le padding r en modifiant la valeur suivante : 
```c
double r = 0.2; // le padding
```
Ensuite il faut modifier la valeur (0.2) du padding dans le script MatLab **affichage.m**
   ```c
    plot( 0.2*cos(t)+h, 0.2*sin(t)+k ); 
   ```
   
   
   > **Lancer le programme:**

 Lancer le **main.cpp** et suivre les instructions.

   
   



More Information
-------------

Pendant le développement de ce projet nous avons testé la planification pour un objet pontuel dans plusieurs environnements complexes (Des formes géometriques non convexes, le départ ou l'arrivée appartient à la frontière d'un polygone ...). Cepandant, pour la planification avec le padding nous n'assurons pas un bon fonctionnement dans des cas complexes.

**Attention**

Les points du départ et d'arrivé doivent être à l'éxtérieur des polygones.


Authors
-------------
Marine NEYRET,
Hicham KOUHKOUH &
Imad EL HANAFI
(Graduate students at ENSTA ParisTech)
