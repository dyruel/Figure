/*
  Name: FigureGUI_Types.h
  Author: Chopin Morgan
  Version: 7.4.2
  Description: Contient tous les types utilisés par FiguRE
*/

#ifndef _FIGUREGUI_TYPES__H
 #define _FIGUREGUI_TYPES__H

// Structure qui contient les informations sur la fenêtre
typedef struct
{
  GtkWidget * win;
  GtkWidget * progress_bar;
  GtkWidget * vbox, * hbox, *hbox_btn,* hbox_saisie;
  GtkWidget * menu_bar;
  GtkWidget * toolbar;
  GtkWidget * draw;
  GtkWidget * result;
  GtkWidget * btn_ana, * btn_del, * btn_app, * btn_aj;
  GtkWidget * saisie_chiffre;
  GtkWidget * saisie_pas;
  GtkWidget * saisie_period;
  GtkWidget * infos_txt;
  GtkWidget * lbl_saisie_chiffre, * lbl_saisie_pas, * lbl_saisie_period;
  GtkWidget * img_pre;
}Window;


typedef struct
{
  guchar * mat;
  guint width,height;
}Buffer;


// Structure qui contient les informations de la zone de dessin
typedef struct
{		
  GtkWidget * drawing_area;	
  Buffer buffer;
}DrawArea;

#endif
