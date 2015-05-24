/*
  Name: FigureGUI.c
  Author: Chopin Morgan
  Version: 7.4.2
  Description: 
*/


#include "FigureGUI.h"
#include "FigureGUI_Types.h"
#include "Dessin.h"
#include "network/Perceptron.h"
#include "pretraitement/Image.h"





/*
  PROTOTYPES
*/

// Fonctions auxiliaires
void            window_barre_menu();
void            window_barre_outils();
void            creation_zone_dessin();
void            creation_buffer_pretraitement();

// Les callbacks
gboolean	zone_dessin_realisee(GtkWidget *w, gpointer data);
gboolean	zone_dessin_rafraichir(GtkWidget *w, GdkEventExpose *e, gpointer data);
gboolean	zone_dessin_clic_souris(GtkWidget *w, GdkEventButton *e, gpointer data);
gboolean	zone_dessin_mouvement_souris(GtkWidget *w, GdkEventMotion *e, gpointer data);
gboolean	zone_dessin_effacer(GtkWidget *w, GdkEventMotion *e, gpointer data);
gboolean        window_quitte(GtkWidget *w, GdkEvent *e, gpointer data);
gboolean        a_propos(GtkWidget *w, GdkEvent *e, gpointer data);
gboolean        pas_implemente(GtkWidget *w, GdkEvent *e, gpointer data);

// Fonctions evenement pour la reconnaissance des chiffres
gboolean        ajouter_un_exemple(GtkWidget *w, GdkEventMotion *e, gpointer data);
gboolean        lancer_apprentissage(GtkWidget *w, GdkEventMotion *e, gpointer data);
gboolean        reconnaitre_chiffre(GtkWidget *w, GdkEventMotion *e, gpointer data);


/*
 VARIABLES GLOBALES
*/

#define PAS_CHIFFRES_RECONNUS "Pas de réponse"

// Pointeur sur la structure de l'application et sur la zone de dessin
Window * ptr_window = NULL;
DrawArea * ptr_draw_area = NULL;

// Pointeur sur le buffer contenant l'image pré-traitée
Buffer * buf_pre = NULL;

// Pointeur sur le perceptron
PerceptronMono  * pm = NULL;

// Tableau contenant le nombre d'exemples disponibles pour chaque chiffres
gint nb_exemples_par_chiffre[10] = {0,0,0,0,0,0,0,0,0,0};

// Contient la liste des valeurs reconnus
guchar * chiffres_reconnus = PAS_CHIFFRES_RECONNUS;

// Chaîne de caractères comportant les informations sur le contenu de la base de connaissances et le chiffre reconnu
guchar * string_infos = "Nombre d'exemples par chiffre :\n Chiffre 0 : %d exemple(s)\n Chiffre 1 : %d exemple(s)\n Chiffre 2 : %d exemple(s) \n Chiffre 3: %d exemple(s) \n Chiffre 4: %d exemple(s)\n Chiffre 5: %d exemple(s) \n Chiffre 6: %d exemple(s) \n Chiffre 7: %d exemple(s)\n Chiffre 8: %d exemple(s)\n Chiffre 9: %d exemple(s) \n\n Chiffre(s) reconnu(s) :\n  < %s >";


// Tableau contenant les informations sur la barre de menu
static GtkItemFactoryEntry menu_item[] = {
    { "/_Fichier", NULL, NULL, 0, "<Branch>" },
    { "/Fichier/_Ouvrir Base de Connaissances", NULL, G_CALLBACK(pas_implemente), 0, "<StockItem>", GTK_STOCK_OPEN },
    { "/Fichier/Enregi_strer Base de Connaissances", "<ctrl>S", G_CALLBACK(pas_implemente), 0, "<StockItem>", GTK_STOCK_SAVE },
    { "/Fichier/Sep1", NULL, NULL, 0, "<Separator>" },
    { "/Fichier/_Quitter", NULL,G_CALLBACK(window_quitte), 1, "<StockItem>", GTK_STOCK_QUIT},
    { "/_?", NULL, NULL, 0, "<Branch>" },
    { "/?/_A propos de FiguRE", "<CTRL>A", G_CALLBACK(a_propos), 1, "<StockItem>", GTK_STOCK_HELP}
};




/*
  FONCTIONS AUXILIAIRES
*/

// Fonction de création du menu
void window_barre_menu()
{
  GtkItemFactory * pItemFactory = NULL;
  GtkAccelGroup * pAccel = NULL;
  gint nb_menu_item = sizeof(menu_item) / sizeof(menu_item[0]);

  if(ptr_window==NULL) return;
	
  pAccel = gtk_accel_group_new();
  pItemFactory = gtk_item_factory_new(GTK_TYPE_MENU_BAR,"<main>",pAccel);
  
  gtk_item_factory_create_items(pItemFactory,nb_menu_item, menu_item,(GtkWidget*) ptr_window->win);
  ptr_window->menu_bar = gtk_item_factory_get_widget(pItemFactory, "<main>");
  gtk_window_add_accel_group(GTK_WINDOW(ptr_window->win), pAccel);
  gtk_widget_show_all(ptr_window->menu_bar);
}


// Fonction de création de la barre d'outils
void window_barre_outils()
{

  if(ptr_window==NULL) return;

  ptr_window->toolbar = gtk_toolbar_new();
  
  gtk_toolbar_insert_stock(GTK_TOOLBAR(ptr_window->toolbar),GTK_STOCK_OPEN,"Ouvrir une base de connaissances",NULL ,G_CALLBACK(NULL),NULL,-1);
  gtk_toolbar_insert_stock(GTK_TOOLBAR(ptr_window->toolbar),GTK_STOCK_SAVE ,"Sauvegarder la base de connaissances",NULL, G_CALLBACK(NULL), NULL,-1);
  gtk_toolbar_insert_stock(GTK_TOOLBAR(ptr_window->toolbar),GTK_STOCK_SAVE_AS,"Sauvegarder sous", NULL, G_CALLBACK(NULL), NULL,-1);
}


// Fonction qui crée la zone de dessin 256x256
void creation_zone_dessin()
{
  if(ptr_window==NULL) return;

  GtkWidget * frame;
  GtkWidget * scrollbar;
	
  guint i;

  /* Création de la zone d'écriture */
  ptr_draw_area = (DrawArea*) g_malloc(sizeof(DrawArea));
  if(ptr_draw_area==NULL) return;
 
  ptr_draw_area->drawing_area = gtk_drawing_area_new();
	
  
  ptr_draw_area->buffer.width = 256;
  ptr_draw_area->buffer.height = 256;
  ptr_draw_area->buffer.mat = (guchar*) g_malloc ((sizeof(guchar))*(256)*(256));
	
  // On prèpare la zone d'écriture
  DESSIN_effacer(ptr_draw_area->buffer);

  gtk_drawing_area_size(GTK_DRAWING_AREA(ptr_draw_area->drawing_area),256,256);
	
  gtk_widget_set_events(ptr_draw_area->drawing_area, GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK );
		
  g_signal_connect(G_OBJECT(ptr_draw_area->drawing_area),"realize", G_CALLBACK(zone_dessin_realisee), NULL);
  g_signal_connect(G_OBJECT(ptr_draw_area->drawing_area),"expose_event", G_CALLBACK(zone_dessin_rafraichir), NULL);
  g_signal_connect(G_OBJECT(ptr_draw_area->drawing_area),"button_press_event", G_CALLBACK(zone_dessin_clic_souris), NULL);
  g_signal_connect(G_OBJECT(ptr_draw_area->drawing_area),"motion_notify_event", G_CALLBACK(zone_dessin_mouvement_souris), NULL);
	
  gtk_widget_set_app_paintable(ptr_draw_area->drawing_area, TRUE); //

  gtk_container_add(GTK_CONTAINER(ptr_window->draw), ptr_draw_area->drawing_area);
	
  gtk_widget_show_all(ptr_window->draw);	
}

// Fonction qui crée le buffer 16x16 contenant l'image pré-traitée
void creation_buffer_pretraitement()
{
  gint i;

  buf_pre = (Buffer*) g_malloc(sizeof(Buffer));
  if(buf_pre==NULL) return;
  
  /* Buffer qui va acceuillir l'image pré-traitée */
  buf_pre->width = 16;
  buf_pre->height = 16;
  buf_pre->mat = (guchar*) g_malloc ((sizeof(guchar))*(256));

  /* On vide le buffer */
  for(i=0;i<buf_pre->width*buf_pre->height;i++)
    buf_pre->mat[i] = (guchar) 255;
}

// Met à jour la barre de progression à frac %
void maj_barre_progression(gdouble frac)
{
  // On met a jour la barre de progression
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(ptr_window->progress_bar),frac);

  // On force GTK a reprendre la main pour mettre à jour l'affichage de la barre de progression
  gtk_main_iteration();
}

// Met à jour les informations à afficher
void maj_infos()
{
  guchar * txt = NULL;

  txt = g_strdup_printf(string_infos,nb_exemples_par_chiffre[0],nb_exemples_par_chiffre[1],nb_exemples_par_chiffre[2],nb_exemples_par_chiffre[3],
			nb_exemples_par_chiffre[4],nb_exemples_par_chiffre[5],nb_exemples_par_chiffre[6],nb_exemples_par_chiffre[7],
			nb_exemples_par_chiffre[8],nb_exemples_par_chiffre[9],chiffres_reconnus);

  gtk_label_set_text(GTK_LABEL(ptr_window->infos_txt),txt);

  g_free(txt);
}

// Fonction qui permet de recuperer la chaine de caractères contenant la liste des chiffres reconnus
gint recup_chiffre(Vecteur * reponse_reseau)
{
  gint i,k = 0;
  gint tab[10];
  guchar * s = NULL,* tmp = NULL;

  if(reponse_reseau == NULL) return -1;

  for(i=0;i<10;i++)
    {
      if(reponse_reseau->c[i] == 1.0)
	{
	  tab[k] = i;
	  k++;
	}
    }


  switch(k)
    {
    case 1:
      s = g_strdup_printf("%d",
			  tab[0]);
      break;

    case 2:
      s = g_strdup_printf("%d %d",
			  tab[0],tab[1]);
      break;
    case 3:
      s = g_strdup_printf("%d %d %d",
			  tab[0],tab[1],tab[2]);
      break;

    case 4:
      s = g_strdup_printf("%d %d %d %d",
			  tab[0],tab[1],tab[2],tab[3]);
      break;

    case 5:
      s = g_strdup_printf("%d %d %d %d %d",
			  tab[0],tab[1],tab[2],tab[3],tab[4]);
      break;

    case 6:
      s = g_strdup_printf("%d %d %d %d %d %d",
			  tab[0],tab[1],tab[2],tab[3],tab[4],tab[5]);
      break;

    case 7:
      s = g_strdup_printf("%d %d %d %d %d %d %d",
			  tab[0],tab[1],tab[2],tab[3],tab[4],tab[5],tab[6]);
      break;

    case 8:
      s = g_strdup_printf("%d %d %d %d %d %d %d %d",
			  tab[0],tab[1],tab[2],tab[3],tab[4],tab[5],tab[6],tab[7]);
      break;
    case 9:
      s = g_strdup_printf("%d %d %d %d %d %d %d %d %d",
			  tab[0],tab[1],tab[2],tab[3],tab[4],tab[5],tab[6],tab[7],tab[8]);
      break;

    case 10:
      s = g_strdup_printf("%d %d %d %d %d %d %d %d %d %d",
			  tab[0],tab[1],tab[2],tab[3],tab[4],tab[5],tab[6],tab[7],tab[8],tab[9]);
      break;

    default:
      s = PAS_CHIFFRES_RECONNUS;

    }

  chiffres_reconnus = s;

  return 1;
}


/*
  FONCTIONS PRINCIPALES
*/

// Creation de l'application FiguRE
void FIGUREGUI_create_app()
{
  guchar * txt = NULL;
  gint i;

  ptr_window = (Window*) g_malloc(sizeof(Window));

  /* Création de la fenêtre */
  ptr_window->win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_title(GTK_WINDOW(ptr_window->win), WTITLE);
  gtk_window_set_icon_from_file(GTK_WINDOW(ptr_window->win),WICON,NULL);
  gtk_window_set_default_size(GTK_WINDOW(ptr_window->win), WWIDTH, WHEIGHT);
  gtk_window_set_resizable(GTK_WINDOW(ptr_window->win),FALSE);
  gtk_window_set_position(GTK_WINDOW(ptr_window->win),GTK_WIN_POS_CENTER);
  g_signal_connect(G_OBJECT(ptr_window->win),"delete-event", G_CALLBACK(window_quitte),NULL);

  /* vbox qui contiendra notre interface */
  ptr_window->vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(ptr_window->win), ptr_window->vbox);

  /* barre menu */
  window_barre_menu();
  gtk_box_pack_start(GTK_BOX(ptr_window->vbox), ptr_window->menu_bar, FALSE, TRUE, 0);


  /* barre d'outils */
  window_barre_outils();
  gtk_box_pack_start(GTK_BOX(ptr_window->vbox), ptr_window->toolbar, FALSE, TRUE, 0);

  /* Création de la barre de progression */
  ptr_window->progress_bar = gtk_progress_bar_new();
  gtk_container_add(GTK_CONTAINER(ptr_window->vbox), ptr_window->progress_bar);

  // HBOX : contiendra les 2 cadres apprentissage et reconnaissance
  ptr_window->hbox = gtk_hbox_new(TRUE, 0);
  gtk_box_pack_start(GTK_BOX(ptr_window->vbox), ptr_window->hbox, TRUE, TRUE, 0);
  
  ptr_window->draw = gtk_frame_new("Zone d'ecriture");
  gtk_frame_set_shadow_type(GTK_FRAME(ptr_window->draw), GTK_SHADOW_IN);
  gtk_box_pack_start(GTK_BOX(ptr_window->hbox), ptr_window->draw, TRUE, TRUE, 0);
  
  ptr_window->result = gtk_frame_new("Informations");
  gtk_frame_set_shadow_type(GTK_FRAME(ptr_window->result), GTK_SHADOW_IN);
  gtk_box_pack_start(GTK_BOX(ptr_window->hbox), ptr_window->result, FALSE, TRUE, 0);

  
  // 
  ptr_window->infos_txt = gtk_label_new(NULL);
  gtk_container_add(GTK_CONTAINER(ptr_window->result),ptr_window->infos_txt);
  maj_infos();

  /* On crée la zone de dessin 256x256  */
  creation_zone_dessin();

  /* Creation buffer pre-traitement */
  creation_buffer_pretraitement();

  /* Creation de la zone de saisie */
  ptr_window->hbox_saisie = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(ptr_window->vbox), ptr_window->hbox_saisie, TRUE, TRUE, 0);

  ptr_window->lbl_saisie_chiffre = gtk_label_new("Chiffre : ");
  gtk_box_pack_start(GTK_BOX(ptr_window->hbox_saisie), ptr_window->lbl_saisie_chiffre, FALSE, TRUE, 0);
  ptr_window->saisie_chiffre = gtk_spin_button_new_with_range(0,9,1);
  gtk_box_pack_start(GTK_BOX(ptr_window->hbox_saisie), ptr_window->saisie_chiffre, FALSE, TRUE, 0);
 

  ptr_window->lbl_saisie_pas = gtk_label_new(" Pas d'apprentissage : ");
  gtk_box_pack_start(GTK_BOX(ptr_window->hbox_saisie), ptr_window->lbl_saisie_pas, FALSE, TRUE, 0);
  ptr_window->saisie_pas = gtk_spin_button_new_with_range(0,1,0.1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(ptr_window->saisie_pas),0.7);
  gtk_box_pack_start(GTK_BOX(ptr_window->hbox_saisie), ptr_window->saisie_pas, FALSE, TRUE, 0);

  ptr_window->lbl_saisie_period = gtk_label_new(" Periodes : ");
  gtk_box_pack_start(GTK_BOX(ptr_window->hbox_saisie), ptr_window->lbl_saisie_period, FALSE, TRUE, 0);
  ptr_window->saisie_period = gtk_spin_button_new_with_range(100,100000,1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(ptr_window->saisie_period),10000.0);
  gtk_box_pack_start(GTK_BOX(ptr_window->hbox_saisie), ptr_window->saisie_period, FALSE, TRUE, 0);
  

  /* Création des boutons */
  ptr_window->hbox_btn = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(ptr_window->vbox), ptr_window->hbox_btn, TRUE, TRUE, 0);
  
  ptr_window->btn_aj = gtk_button_new_with_label("Ajouter l'exemple");
  g_signal_connect(G_OBJECT(ptr_window->btn_aj), "clicked", G_CALLBACK(ajouter_un_exemple), NULL);
  gtk_box_pack_start(GTK_BOX(ptr_window->hbox_btn), ptr_window->btn_aj, FALSE, TRUE, 0);
  
  ptr_window->btn_del = gtk_button_new_with_label("Effacer le dessin");
  g_signal_connect(G_OBJECT(ptr_window->btn_del), "clicked", G_CALLBACK(zone_dessin_effacer), NULL);
  gtk_box_pack_start(GTK_BOX(ptr_window->hbox_btn), ptr_window->btn_del, FALSE, TRUE, 0);

  ptr_window->btn_app = gtk_button_new_with_label("Lancer l'apprentissage");
  g_signal_connect(G_OBJECT(ptr_window->btn_app), "clicked", G_CALLBACK(lancer_apprentissage), NULL);
  gtk_box_pack_start(GTK_BOX(ptr_window->hbox_btn), ptr_window->btn_app, FALSE, TRUE, 0);
  
  ptr_window->btn_ana = gtk_button_new_with_label("Reconnaitre le chiffre");
  g_signal_connect(G_OBJECT(ptr_window->btn_ana), "clicked", G_CALLBACK(reconnaitre_chiffre), NULL);
  gtk_box_pack_start(GTK_BOX(ptr_window->hbox_btn), ptr_window->btn_ana, FALSE, TRUE, 0);

  
  /* Affiche tout */
  gtk_widget_show_all(ptr_window->win);
 
  /* Creation du perceptron de 16x16 = 256 entrées avec 10 neurones*/
  pm = PERCEPTRONMONO_new(10,256);
  
}

// Desctruction de l'application
void FIGUREGUI_delete_app()
{
  if(ptr_window!=NULL) g_free(ptr_window);
  if(ptr_draw_area!=NULL) g_free(ptr_draw_area);
  if(buf_pre->mat!=NULL) g_free(buf_pre->mat);
  if(buf_pre!=NULL) g_free(buf_pre);
  PERCEPTRONMONO_free(pm);
  ptr_window = NULL;
  ptr_draw_area = NULL;
  pm = NULL;
  buf_pre->mat = NULL;
}

/*
  FONCTIONS EVENEMENTS POUR LA RECONNAISSANCE
*/

/* 
   Appellée si on clique sur "Ajouter l'exemple"

   L'ajout d'un exemple se fait en plusieurs étapes :

   1) On applique la fonction de pré-traitement sur l'image, on stocke la nouvelle image dans un buffer. (buf_pre)

   2) A partir cette image, on récupère le vecteur caractéristique qui correspond en fait à la matrice de l'image.

   3) Ensuite on récupère la valeur numérique du chiffre saisie, on en déduit alors le vecteur de sortie.

   4) On sauvegarde alors l'exemple (vecteur caractéristique + vecteur de sortie) dans la base de connaissances.

*/
gboolean ajouter_un_exemple(GtkWidget *w, GdkEventMotion *e, gpointer data)
{
  Exemple * ex = NULL;
  gint i,j,k = 0;

  gdouble s = 0.0;
  gint chiffre = 0;
  guchar * txt = NULL;

  printf("<AJOUTER>\n");

  if(BASECONNAISSANCE_est_pleine(pm->bc) == FALSE)
    {
      ex = EXEMPLE_new(buf_pre->width * buf_pre->height,pm->nb_neurone);
      if(ex==NULL) { printf("erreur\n"); return FALSE; }
  
      maj_barre_progression(0.2);

      // Pré-traitement : échantillonage
      IMAGE_echantillonnage(ptr_draw_area->buffer,buf_pre);

      maj_barre_progression(0.4);
      
      // Recupération du vecteur caractéristique et affichage
      printf("Vecteur caractéristique:\n");
      for(i=0;i<buf_pre->width;i++)
	{
	  for(j=0;j<buf_pre->height;j++)
	    {
	      s = (gdouble) 1.0 - (buf_pre->mat[(j*(buf_pre->width)+i)]/255);
	      printf("%.1f ",s);
	      ex->vecCarac->c[k] = s;
	      k++;
	    }
	  printf("\n");
	}
      
      maj_barre_progression(0.6);
      
      // Récupération de la valeur du chiffre écrit
      printf("Vecteur de sortie:\n");
      chiffre = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ptr_window->saisie_chiffre));
      ex->vecSortie->c[chiffre] = 1.0;

      VECTEUR_afficher(ex->vecSortie);

      maj_barre_progression(0.8);
      
      // Sauvegarde de l'exemple dans la base
      BASECONNAISSANCE_ajouter(pm->bc,ex);
      
      // Mise a jour des informations
      nb_exemples_par_chiffre[chiffre]++;
      maj_infos();

      maj_barre_progression(1.0);
      maj_barre_progression(0.0);
      
      return TRUE;
    }
  else
    {
      printf("Base de connaissances pleine\n");
    }
}

// Appellée si on clique sur "Lancer l'apprentissage"
/*
  Pour un descriptif détaillé de l'algorithme voir le rapport
*/
gboolean lancer_apprentissage(GtkWidget *w, GdkEventMotion *e, gpointer data)
{
  gint i,j,k;
  gint erreurs = 1;
  gint nb_essais = 0;
  gint egaux = 0;
  gint max_periode = 10000;

  gdouble nouveau_poids = 0.0;
  gdouble pas = 0.7;
  gdouble pourcentage_effectue = 0.0,pour_pas = .0;
  
  Exemple * exemple_courant = NULL;
  Vecteur * sortie_desiree = NULL;
  Vecteur * vecteur_carac = NULL;
  Vecteur * sortie_obtenue = NULL;

  printf("<APPRENTISSAGE EN COURS>\n");
  fflush(stdout);

  max_periode = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ptr_window->saisie_period));
  pas = gtk_spin_button_get_value(GTK_SPIN_BUTTON(ptr_window->saisie_pas));

  printf("pas = %f , period = %d\n",pas,max_periode);
  fflush(stdout);
  
  while((erreurs == 1) && (nb_essais<max_periode))
    {
      pourcentage_effectue = (gdouble) nb_essais/max_periode;

      if(pourcentage_effectue >= pour_pas)
	{
	  maj_barre_progression(pourcentage_effectue);
	  pour_pas += .1;
	}

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
  

  printf("<TERMINE>\n");
  maj_barre_progression(0.0);

  return TRUE;
}

/* 
   Appellée si on clique sur "Reconnaître le chiffre"

   La reconnaissance se fait, elle aussi, en plusieurs étapes :

   1) On applique la fonction de pré-traitement sur l'image, on stocke la nouvelle image dans un buffer. (buf_pre)

   2) A partir cette image, on récupère le vecteur caractéristique qui correspond en fait à la matrice de l'image.

   3) On calcule alors la réponse du perceptron avec ce vecteur caractéristique.

   4) A partir du vecteur sortie du perceptron, on en déduit la valeur numérique du chiffre que l'on affiche.

*/
gboolean reconnaitre_chiffre(GtkWidget *w, GdkEventMotion *e, gpointer data)
{
  Vecteur * r = NULL, * v = NULL;
  gint i,j,k = 0;
  gdouble s = 0.0;

 printf("<RECONNAISSANCE>\n");

 maj_barre_progression(0.2);

 v = VECTEUR_new(buf_pre->width * buf_pre->width);
 if(v==NULL) { printf("erreur\n"); return FALSE; }

 // Pré-traitement : échantillonage
 IMAGE_echantillonnage(ptr_draw_area->buffer,buf_pre);

 maj_barre_progression(0.4);

  // Recupération du vecteur caractéristique
 printf("Vecteur caractéristique:\n");
  for(i=0;i<buf_pre->width;i++)
    {
      for(j=0;j<buf_pre->height;j++)
	{
	  s = (gdouble) 1.0 - (buf_pre->mat[(j*(buf_pre->width)+i)]/255);
	  v->c[k] = s;
	  printf("%.1f ",s);
	  k++;
	}
      printf("\n");
    }

  r = PERCEPTRONMONO_calculer_sortie(pm,v);
  
  maj_barre_progression(0.6);

  // Affichage du vecteur sortie sur la sortie std
  printf("Vecteur de sortie:\n");
  VECTEUR_afficher(r);

  // Mise a jour des informations
  recup_chiffre(r);
  maj_infos();

  maj_barre_progression(0.8);

  VECTEUR_free(r);
  VECTEUR_free(v);

  maj_barre_progression(0.0);

  return TRUE;
}



/*
  FONCTIONS EVENEMENTS
*/


// Fonction appelée pour demander la confirmation de la fermeture de l'application
gboolean window_quitte(GtkWidget *w, GdkEvent *e, gpointer data)
{
  GtkWidget * win = NULL;
	
  win = gtk_message_dialog_new (GTK_WINDOW(w),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Etes-vous sur de vouloir\nquitter FiguRE ?");
                   
  switch(gtk_dialog_run(GTK_DIALOG(win)))
    {
    case GTK_RESPONSE_YES:
      {
	gtk_widget_destroy(win);
	gtk_main_quit();
	return FALSE;
      }
      break;
    case GTK_RESPONSE_NONE:
    case GTK_RESPONSE_NO:
      gtk_widget_destroy(win);
      break;
    }

  return TRUE;
}

// Lance le "a propos"
gboolean a_propos(GtkWidget *w, GdkEvent *e, gpointer data)
{
  GtkWidget * about = NULL;

  about = gtk_message_dialog_new(GTK_WINDOW(w),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"FiguRE version %s.\nCréé par Morgan Chopin\ndyruel@gmail.com",VERSION);

  gtk_dialog_run(GTK_DIALOG(about));
  gtk_widget_destroy(about);

  return TRUE;
}

// Message affiché lorsqu'une fonction du logiciel n'est pas implémentée
gboolean pas_implemente(GtkWidget *w, GdkEvent *e, gpointer data)
{
  GtkWidget * not_im = NULL;

  not_im = gtk_message_dialog_new(GTK_WINDOW(w),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Cette fonctionnalité n'est pas\nencore implémentée.",VERSION);

  gtk_dialog_run(GTK_DIALOG(not_im));
  gtk_widget_destroy(not_im);

  return TRUE;
}

// Appellée lorsque la fenêtre vient de se créer
gboolean zone_dessin_realisee(GtkWidget *w, gpointer data)
{	
  GdkCursor * cursor = NULL;

  if(!ptr_draw_area)
    return FALSE;
		
  cursor=gdk_cursor_new(GDK_PLUS);
  gdk_window_set_cursor(ptr_draw_area->drawing_area->window, cursor);
  gdk_cursor_unref(cursor);
}


// Appellée lorsque la zone d'écriture a besoin d'être redessinée
gboolean zone_dessin_rafraichir(GtkWidget *drawing, GdkEventExpose *e, gpointer data)
{
	
  if(!ptr_draw_area->buffer.mat)
    return FALSE;
	
  gdk_draw_gray_image(drawing->window, drawing->style->fg_gc[GTK_WIDGET_STATE(drawing)],
		      0,0,
		      ptr_draw_area->buffer.width, ptr_draw_area->buffer.height, 
		      GDK_RGB_DITHER_MAX, 
		      ptr_draw_area->buffer.mat, 
		      ptr_draw_area->buffer.width);
       
  return TRUE;
}

// Appellée lors d'un clic
gboolean zone_dessin_clic_souris(GtkWidget *w, GdkEventButton *e, gpointer data)
{
	gint x,y;
	GdkModifierType state;
	
	if((e->button == 1) && (ptr_draw_area->buffer.mat != NULL))
	{
	  gdk_window_get_pointer (w->window, &x, &y, &state);
	  DESSIN_dessiner_point(ptr_draw_area->buffer,x,y,10);
	  gtk_widget_queue_draw(w);
	}

	return TRUE;
}

// Appellée lorsque la souris bouge
gboolean zone_dessin_mouvement_souris(GtkWidget *w, GdkEventMotion *e, gpointer data)
{
	gint x;
	gint y;
	GdkModifierType state;	

	if(e->is_hint)
	  gdk_window_get_pointer(w->window, &x, &y, &state);
	else
	{
	  x=(gint)e->x;
	  y=(gint)e->y;
	  state=(GdkModifierType)e->state;
	}
	
	if((state & GDK_BUTTON1_MASK) && ptr_draw_area->buffer.mat != NULL)
	{
	  DESSIN_dessiner_point(ptr_draw_area->buffer,x,y,10);
	  gtk_widget_queue_draw(w);
	}

	return TRUE;
}

// Appellée lorsque la zone de dessin doit être effacée
gboolean zone_dessin_effacer(GtkWidget *w, GdkEventMotion *e, gpointer data)
{
  DESSIN_effacer(ptr_draw_area->buffer);

  gtk_widget_queue_draw(ptr_draw_area->drawing_area);

  return TRUE;
}
