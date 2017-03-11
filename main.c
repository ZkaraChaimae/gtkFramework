#include <stdlib.h>
#include <gtk/gtk.h>
#include "macros.h"


int main (int argc, char *argv[])
{

  /* Initialize GTK+ */
  gtk_init (&argc, &argv);
/*
    fenetre *window=(fenetre*)malloc(sizeof(fenetre));
    initialiser_fenetre(window,"Hello world");
    window = creation_fenetre(window,200,300,"Hello world",'T');
*/

    /** CREATION FENETRE */
   fenetre *mafenetre;

   mafenetre=initialiser_fenetre('T',"initiation",300,400,1,-1,-1);
   creation_fenetre(mafenetre);
   /** Creation scrolledWindow */
   scroll_window *sw;
   sw=creation_scroll_window(1,1,mafenetre->window);

   /** CREATION FIXED CONTAINER */
   fixed* fixed_container = NULL;
   fixed_container = initialiser_fixed_container(sw->swind);

   /** CREATION BOX */
   box* radio_box = NULL;
   radio_box = initialiser_box(sw->swind,'h',1,10);

   /** CREATION BOUTONS RADIO */
   elem_radio* radio_list = NULL;
   radio_list = ajouter_radio_fin(radio_list,"choix 1");
   radio_list = ajouter_radio_fin(radio_list,"choix 2");
   radio_list = ajouter_radio_fin(radio_list,"choix 3");

   radio* R=NULL;
   R = ajouter_groupe_radio(radio_list,radio_box->la_box);
   creation_radio_bouton(R);

   set_position(fixed_container,radio_box->la_box,100,100);

   gtk_widget_show_all(mafenetre->window);
   gtk_main();
  return 0;
}
