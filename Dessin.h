/*
  Name: Dessin.h
  Author: Chopin Morgan
  Version: 7.4.2
  Description: Fonctions de dessin de base dans un buffer
*/

#ifndef _DESSIN__H
 #define _DESSIN__H

#include <gtk/gtk.h>
#include "FigureGUI_Types.h"


gboolean DESSIN_dessiner_point(Buffer buffer, gint x, gint y, gint taille);
gboolean DESSIN_effacer(Buffer buffer);

#endif
