/*
  Name: Vecteur.h
  Author: Chopin Morgan
  Version: 7.4.2
  Description: Fonctions de base pour la manipulation de vecteurs
  (egalite,produit scalaire et somme)
*/


#ifndef _VECTEUR_H_
 #define _VECTEUR_H_

#include <stdlib.h>
#include <gtk/gtk.h>

typedef struct Vecteur
{
  gdouble  * c;
  gint dim;
}Vecteur;

// Crée un nouveau vecteur
Vecteur * VECTEUR_new(gint dim)
{
  Vecteur * v = NULL;
  gint i = 0;

  v = (Vecteur*) g_malloc(sizeof(Vecteur));
  if(v==NULL) return NULL;

  v->dim = dim;
  v->c = (gdouble*) g_malloc(dim*sizeof(gdouble));

  for(i=0;i<dim;i++)
    v->c[i] = 0.0;

  return v;
}

// Libère la mémoire utilisé par un vecteur
void VECTEUR_free(Vecteur * v)
{
  if(v!=NULL)
    {
      g_free(v->c);
      g_free(v);
    }
}


// Renvoie le produit scalaire r des vecteurs x et y
gint VECTEUR_produitScalaire(Vecteur * x,Vecteur * y,gdouble * r)
{
  gint i;

  if((x!=NULL) && (y!=NULL) && (x->dim == y->dim))
    {
      for(i=0;i<x->dim;i++)
	*r = *r + (x->c[i])*(y->c[i]);
    }
  else
    {
      perror("Operation vectorille non-valide");
      return -1;
    }

  return 1;;
}

// Renvoie la somme s des vecteurs x et y
gint VECTEUR_sommeVecteur(Vecteur * x,Vecteur * y,Vecteur * s)
{
  gint i;

  if((x!=NULL) && (y!=NULL) && (s!=NULL) && (x->dim == y->dim) && (x->dim == s->dim))
    {
      for(i=0;i<x->dim;i++)
	s->c[i] = (x->c[i])+(y->c[i]);
    }
  else
    {
      perror("Operation vectorielle non-valide");
      return -1;
    }

  return 1;
}

// Test l'égalité de 2 vecteurs
gint VECTEUR_egaux(Vecteur * x,Vecteur * y,gint * r)
{
  gint i;
  *r = 0;

  if((x!=NULL) && (y!=NULL) && (x->dim == y->dim))
    {
      *r = 1;
      for(i=0;(i<x->dim) && ((*r)!=0);i++)
	{
	  if((x->c[i])!=(y->c[i])) *r = 0;
	}
    }
  else
    {
      perror("Operation vectorielle non-valide");
      return -1;
    }

  return 1;
}

// Affiche un vecteur sur la sortie standart
gint VECTEUR_afficher(Vecteur * x)
{
  gint i;

  if(x==NULL) return -1;

  for(i=0;i<x->dim;i++)
    {
      printf("%.1f ",x->c[i]);
      fflush(stdout);
    }
  
  printf("\n");
  fflush(stdout);

  return 1;
}


#endif
