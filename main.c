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

   /** CREATION RADIO_BOX */
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

   bouton* button = NULL;
   button = initialiser_bouton(radio_box->la_box,NULL,"Valider",0);
   creation_bouton_simple(button);

   /** Position des boutons radio */
   set_position(fixed_container,radio_box->la_box,50,50);

   /** CREATION CHECK_BOX */
   box* check_box = NULL;
   check_box = initialiser_box(sw->swind,'v',1,10);

   /** CREATION BOUTONS CHECK BOX */
   elem_check *check1 = NULL,*check2 = NULL,*check3=NULL,*check4 = NULL,*check5=NULL;
   check1 = initialiser_check_button("Fedora",check_box->la_box);
   check2 = initialiser_check_button("Windows",check_box->la_box);
   check3 = initialiser_check_button("Debian",check_box->la_box);
   check4 = initialiser_check_button("Mint",check_box->la_box);
   check5 = initialiser_check_button("Linux Red Hat",check_box->la_box);

   creation_check_bouton(check1);
   creation_check_bouton(check2);
   creation_check_bouton(check3);
   creation_check_bouton(check4);
   creation_check_bouton(check5);

   /** Bouton */
   bouton* button2 = NULL;
   button2 = initialiser_bouton(check_box->la_box,NULL,"Choisir",0);
   creation_bouton_simple(button2);

   /** Position des boutons check box */
   set_position(fixed_container,check_box->la_box,70,100);


   /** Zone de saisie */
   box *zoneSaisie = NULL;
   zoneSaisie = initialiser_box(sw->swind,'v',0,20);

   zone_saisie *login=NULL,*mdp=NULL;
   login = initialiser_zone_saisie(zoneSaisie->la_box,20,"Login",1,'*');
   mdp = initialiser_zone_saisie(zoneSaisie->la_box,20,"Password",0,'*');
   creation_zone_saisie(login);
   creation_zone_saisie(mdp);

   set_position(fixed_container,zoneSaisie->la_box,210,120);


   /** Zone de texte */
   box *zoneTexte = NULL;
   zoneTexte = initialiser_box(sw->swind,'v',0,10);

   zone_texte *buffer = NULL;
   buffer = initialiser_zone_texte(zoneTexte->la_box);
   creation_zone_texte(buffer);

   //ajouter_container(fixed_container->fix,zoneTexte->la_box);
   set_position(fixed_container,buffer->pTextView,20,220);

   gtk_widget_show_all(mafenetre->window);
   gtk_main();
  return 0;
}
