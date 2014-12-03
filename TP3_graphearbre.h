#ifndef _TP2_GRAPHEARBRE_H
#define _TP2_GRAPHEARBRE_H

/* un graphe est un tableau de pointeurs sur struct _graphe */
typedef struct _graphe {
  int voisin;
  struct _graphe *suivant;
}**graphe;


graphe init_graphe(int nsommets);
void ajoute_arete(graphe g, int n1, int n2);
void affiche_graphe(graphe g, int nsommets);
void libere_graphe(graphe g, int nsommets);

typedef struct _arbre {
  int val;
  struct _arbre *fils;
  struct _arbre *frere;
} *arbre;

arbre init_arbre(int label_racine);
void ajoute_fils(arbre a,arbre a2);
void libere_arbre(arbre a);
void affiche_arbre(arbre a);

#endif
