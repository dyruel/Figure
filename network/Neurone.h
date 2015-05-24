/*
  Name: Neurone.h
  Author: Chopin Morgan
  Version: 7.4.2
  Description: Implémentation d'un neurone formel.
*/


#ifndef _NEURONE_H_
 #define _NEURONE_H_

#include "Vecteur.h"

typedef struct Neurone
{
  Vecteur * poids;
  double biais;
  gchar f;
  gint nb_poids;
}Neurone;


gint NEURONE_fonction(gchar type,gdouble x,gdouble * y)
{
  switch(type)
    {
    case 'p': // Fonction à pas
      {
	if(x < 0) *y = 0.0;
	else *y = 1.0;
      }
      break;

    case 'l': // Fonction linéaire
      {
	*y = x;
      }
      break;

    default:
      return -1;
      break;
    }

  return 1;
}

// Crée un nouveau neurone
Neurone * NEURONE_new(gint nb_poids)
{
  Neurone * n = NULL;

  n = (Neurone*) g_malloc(sizeof(Neurone));
  if(n==NULL) return NULL;

  n->poids = VECTEUR_new(nb_poids);
  n->biais = 1.0;
  n->f = 'p';
  n->nb_poids = nb_poids;

  return n;
}

// Libère la mémoire utilisé par un neurone
void NEURONE_free(Neurone * n)
{
  if(n!=NULL)
    {
      VECTEUR_free(n->poids);
      g_free(n);
    }
}

gint NEURONE_sortie(Neurone * n,Vecteur * v,gdouble * s)
{
  gdouble ps = 0.0;

  if((n!=NULL) && (v!=NULL) && (n->poids->dim == v->dim))
    {
      if(VECTEUR_produitScalaire(n->poids,v,&ps)<0)
	return -1;

      if(NEURONE_fonction(n->f,(ps + n->biais),s)<0)
	return -1;
    }
  else return -1;

  return 1;
}

#endif
