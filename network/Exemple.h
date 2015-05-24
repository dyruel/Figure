/*
  Name: Exemple.h
  Author: Chopin Morgan
  Version: 7.4.2
  Description: Implémentation des exemples qui seront contenus dans la base de connaissances.
*/


#ifndef _EXEMPLE_H_
 #define _EXEMPLE_H_

#include "Vecteur.h"

typedef struct Exemple
{
  Vecteur * vecCarac;
  Vecteur * vecSortie;
}Exemple;


// Crée un nouvel exemple
Exemple * EXEMPLE_new(gint dim1,gint dim2)
{
  Exemple * ex = NULL;

  ex = (Exemple*) g_malloc(sizeof(Exemple));
  if(ex==NULL) return NULL;

  ex->vecCarac = VECTEUR_new(dim1);
  ex->vecSortie = VECTEUR_new(dim2);

  return ex;
}

// Libère la mémoire utilisé par un exemple
void EXEMPLE_free(Exemple * ex)
{
  if(ex!= NULL)
    {
      VECTEUR_free(ex->vecCarac);
      VECTEUR_free(ex->vecSortie);
      g_free(ex);
    }
}

#endif
