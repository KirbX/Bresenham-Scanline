
#include <stdio.h>
#include <stdlib.h>
#include "TP3_graphearbre.h"

graphe init_graphe(int nsommets) {
  return (graphe) malloc(nsommets*sizeof(struct _graphe*));
}

static void ajoute_arete_orientee(graphe g, int n1, int n2) {
  struct _graphe *liste=g[n1],*liste_prec=NULL;
  while(liste!=NULL) {
    if(liste->voisin==n2) return;
    liste_prec=liste;
    liste=liste->suivant;
  }
  liste=(struct _graphe*) malloc(sizeof(struct _graphe));
  if(liste_prec==NULL) g[n1]=liste; else liste_prec->suivant=liste;
  liste->voisin=n2;
}

void ajoute_arete(graphe g, int n1, int n2) {
  ajoute_arete_orientee(g,n1,n2);
  ajoute_arete_orientee(g,n2,n1);
}

void affiche_graphe(graphe g, int nsommets) {
  int i;
  for(i=0;i<nsommets;i++) {
    printf("%d: ",i);
    struct _graphe *liste=g[i];
    while(liste!=NULL) {
      printf(" %d",liste->voisin);
      liste=liste->suivant;
    }
    printf("\n");
  }
}

void libere_graphe(graphe g, int nsommets) {
  int i;
  for(i=0;i<nsommets;i++) {
    struct _graphe *liste=g[i];
    while(liste!=NULL) {
      struct _graphe *liste_prec=liste;
      liste=liste->suivant;
      free(liste_prec);
    }
  }
  free(g);
}

arbre init_arbre(int n) {
  arbre a=(arbre) malloc(sizeof(struct _arbre));
  a->val=n;
  a->fils=NULL;
  return a;
}

void ajoute_frere(arbre a, arbre a2) {
  if(a->frere==NULL)
    a->frere=a2;
  else
    ajoute_frere(a->frere,a2);
}

void ajoute_fils(arbre a,arbre a2) {
  if(a->fils==NULL)
    a->fils=a2;
  else
    ajoute_frere(a->fils,a2);
}

void libere_arbre(arbre a) {
  if(a!=NULL) {
    libere_arbre(a->fils);
    libere_arbre(a->frere);
    free(a);
  }
}

static void affiche_arbre2(arbre a) {
  if(a!=NULL) {
    printf("%d",a->val);
    arbre a2=a->fils;
    if(a2!=NULL) {
      printf("(");
      while(a2!=NULL) {
	affiche_arbre2(a2);
	a2=a2->frere;
	if(a2!=NULL) printf(",");
      }
      printf(")");
    }
  }
}

void affiche_arbre(arbre a) {
  affiche_arbre2(a);
  printf("\n");
}
