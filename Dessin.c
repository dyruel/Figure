/*
  Name: Dessin.c
  Author: Chopin Morgan
  Version: 7.4.2
  Description: Fonctions de dessin de base dans un buffer
*/

#include "Dessin.h"



gboolean DESSIN_dessiner_point(Buffer buffer, gint x, gint y,gint taille)
{
  gint i,j=0;

  if(x<0 || x>buffer.width)
    return FALSE;
  if(y<0 || y>buffer.height)
    return FALSE;

  
  if(taille==1)
    buffer.mat[(y*(buffer.width)+x)] =(guchar) 0;
  else
    {
      for(i = x - (taille/2);i < ((x - (taille/2)) + taille); i++)
	for(j = y - (taille/2); j < ((y - (taille/2)) + taille); j++)
	  if((j*(buffer.width)+i) < (buffer.width * buffer.height)) 
	    buffer.mat[(j*(buffer.width)+i)] =(guchar) 0;
    }
}


gboolean DESSIN_effacer(Buffer buffer)
{
  gint i;

  for(i=0;i<(buffer.width * buffer.height);i++)
    buffer.mat[i] =(guchar) 255;
}

