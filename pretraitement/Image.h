/*
  Name: Dessin.c
  Author: Chopin Morgan
  Version: 7.4.2
  Description: Fonctions de traitement d'une image contenue dans un buffer
*/




#ifndef _IMAGE_H_
 #define _IMAGE_H_

#include "stdlib.h"
#include "gtk/gtk.h"
#include "../FigureGUI_Types.h"


gint IMAGE_echantillonnage(Buffer src,Buffer * dst)
{
  gint i,j;
  gint x,y;

  gint nb_pixel_noir = 0;

  for(i=0;i<dst->width;i++)
    for(j=0;j<dst->height;j++)
      dst->mat[j*(dst->width)+i] = (guchar) 255;
	  
  for(i=0;i<16;i++)
    {
      for(j=0;j<16;j++)
	{
	  nb_pixel_noir = 0;

	  for(x = i*16; x < (i*16) + 16;x++)
	    {
	      for(y = j*16; y < (j*16) + 16;y++)
		{
		  if(src.mat[y*src.width+x] == ((guchar) 0))
		    nb_pixel_noir++;
		}
	    }

	  if(nb_pixel_noir > 0)
	    dst->mat[j*(dst->width)+i] = (guchar) 0;
	}
    }
  
  return 1;
}


gint IMAGE_squeletisation(Buffer src,Buffer * dst)
{

  return 1;
}



#endif
