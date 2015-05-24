/*
  Name: Perceptron.h
  Author: Chopin Morgan
  Version: 7.4.2
  Description: Implémentation d'un perceptron monocouche avec fonction d'apprentissage.
*/

#ifndef _PERCEPTRONMONO_H_
 #define _PERCEPTRONMONO_H_

#include "BaseConnaissance.h"
#include "Neurone.h"

#define MAX_EXEMPLES 100

typedef struct PerceptronMono
{
  Neurone ** neurones;
  BaseConnaissance * bc;
  gint nb_neurone;
}PerceptronMono;

// Crée un nouveau perceptron monocouche
PerceptronMono * PERCEPTRONMONO_new(gint nb_neurone,gint taille_entree)
{
  PerceptronMono * pm = NULL;
  gint i;

  pm = (PerceptronMono*) g_malloc(sizeof(PerceptronMono));
  if(pm==NULL) return NULL;

  pm->nb_neurone = nb_neurone;
  pm->neurones = (Neurone**) g_malloc(nb_neurone*sizeof(Neurone*));
  pm->bc = BASECONNAISSANCE_new(MAX_EXEMPLES);

  for(i=0;i<nb_neurone;i++)
    pm->neurones[i] = NEURONE_new(taille_entree);

  return pm;
}

// Libère la mémoire utilisé par un vecteur
void PERCEPTRONMONO_free(PerceptronMono * pm)
{
  gint i;

  if(pm!=NULL)
    {
      for(i=0;i<pm->nb_neurone;i++)
	NEURONE_free(pm->neurones[i]);

      g_free(pm->neurones);
      BASECONNAISSANCE_free(pm->bc);
      g_free(pm);
    }
}


Vecteur * PERCEPTRONMONO_calculer_sortie(PerceptronMono * pm,Vecteur * e)
{
  Vecteur * reponse = VECTEUR_new(pm->nb_neurone);
  gdouble sortie_neurone = 0.0;
  gint i;
		
  for(i=0; i < pm->nb_neurone; i++) 
    {
      NEURONE_sortie(pm->neurones[i],e,&sortie_neurone);
      reponse->c[i] = sortie_neurone;
    }

  return reponse;
}

int PERCEPTRONMONO_apprentissage(PerceptronMono * pm,gdouble pas,gint max_periode)
{
  gint i,j,k;
  gint erreurs = 1;
  gint nb_essais = 0;
  gint egaux = 0;

  gdouble nouveau_poids = 0.0;

  Exemple * exemple_courant = NULL;
  Vecteur * sortie_desiree = NULL;
  Vecteur * vecteur_carac = NULL;
  Vecteur * sortie_obtenue = NULL;
  
  while((erreurs == 1) && (nb_essais<max_periode))
    {

      // On envoie au réseau tous les exemples contenus dans la base
      for(i=0;i<pm->bc->nb;i++)
	{
	  exemple_courant = pm->bc->exs[i];
	  
	  sortie_desiree = exemple_courant->vecSortie;
	  vecteur_carac = exemple_courant->vecCarac;

	  sortie_obtenue = PERCEPTRONMONO_calculer_sortie(pm,vecteur_carac);

	  VECTEUR_egaux(sortie_obtenue,sortie_desiree,&egaux);

	  if(egaux == 0)
	    {
	      // Pour chaque neurone du perceptron
	      for(j=0;j<pm->nb_neurone;j++)
		{
		  // On modifie ses poids
		  for(k=0;k<pm->neurones[j]->nb_poids;k++)
		    {
		      nouveau_poids = pm->neurones[j]->poids->c[k] + pas * (sortie_desiree->c[j] - sortie_obtenue->c[j]) * vecteur_carac->c[k];
		      pm->neurones[j]->poids->c[k] = nouveau_poids;
		    }
		}

	      erreurs = 1;
	    }

	  VECTEUR_free(sortie_obtenue);
	}

      nb_essais++;
    }	   
}

#endif
