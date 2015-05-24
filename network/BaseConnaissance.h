/*
  Name: BaseConnaissance.h
  Author: Chopin Morgan
  Version: 7.4.2
  Description: Contient l'ensemble des exemples qui seront utilisés lors de l'apprentissage
*/


#ifndef _BASECONNAISSANCE_H_
 #define _BASECONNAISSANCE_H_

#include "Exemple.h"

typedef struct BaseConnaissance
{
  Exemple ** exs;
  gint dim;
  gint nb;
  gint place_vide;
}BaseConnaissance;

// Crée un nouveau vecteur
BaseConnaissance * BASECONNAISSANCE_new(gint dim)
{
  BaseConnaissance * bc = NULL;
  gint i;

  bc = (BaseConnaissance*) g_malloc(sizeof(BaseConnaissance));
  if(bc==NULL) return NULL;

  bc->dim = dim;
  bc->nb = 0;
  bc->place_vide = 0;
  bc->exs = (Exemple**) g_malloc(dim*sizeof(Exemple*));

  for(i=0;i<dim;i++)
    bc->exs[i] = NULL;

  return bc;
}

// Libère la mémoire utilisé par un vecteur
void BASECONNAISSANCE_free(BaseConnaissance * bc)
{
  gint i;

  if(bc!=NULL)
    {
      for(i=0;i<bc->dim;i++)
	EXEMPLE_free(bc->exs[i]);

      g_free(bc->exs);
      g_free(bc);
    }
}

gint BASECONNAISSANCE_ajouter(BaseConnaissance *  bc,Exemple * ex)
{
  if((bc!=NULL) && (bc->nb < bc->dim))
    {
      bc->exs[bc->place_vide] = ex;
      bc->place_vide++;
      bc->nb++;
    }
  else
    {
      perror("Base de connaissance pleine ou non initialisée");
      return -1;
    }

  return 1;
}

gboolean BASECONNAISSANCE_est_pleine(BaseConnaissance *  bc)
{
 if((bc!=NULL) && (bc->nb < bc->dim))
   return FALSE;
 else
   return TRUE;  
}

#endif
