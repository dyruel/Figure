/*
  Name: FiguRE
  Author: Chopin Morgan
  Version: 7.4.2
  Description: FiguRE est un système de reconnaissance de chiffres manuscrits interactif.
*/

#include "FigureGUI.h"


int main(int argc,char ** argv)
{
 gtk_init(&argc,&argv);

 FIGUREGUI_create_app(); // Création l'application

 gtk_main();

 FIGUREGUI_delete_app(); // Destruction de l'application

 return 0;
}
