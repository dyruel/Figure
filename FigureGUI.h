/*
  Name: FigureGUI.h
  Author: Chopin Morgan
  Description: 

  7.4.2 : apport des champs de saisie du pas et de la periode

  7.4.3 : correction du bug : possibilité d'ecrire un pixel hors zone de dessin -> plantage

  7.4.5 : rafraichissement du code
          correction du bug : base de connaissance infinie
	  amélioration de l'affichage de la réponse du perceptron
*/

#ifndef _FIGUREGUI__H
 #define _FIGUREGUI__H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>

// Paramètres modifiables pour la fenêtre
#define WTITLE "FiguRE" // Titre
#define WICON "ico.png" // Icône
#define WWIDTH 100 // Hauteur
#define WHEIGHT 100 // Largeur
#define VERSION "7.4.5" // Version

// Fonctions outils
void FIGUREGUI_create_app();
void FIGUREGUI_delete_app();

#endif
