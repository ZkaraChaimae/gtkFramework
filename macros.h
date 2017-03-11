#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Ajout de sdd composant widget */
typedef struct
{
    GtkWidget *widget;
}composant;

typedef struct Taille
{
  int largeur;  /* largeur de la fenetre */
  int hauteur;  /* hauteur de la fenetre */
}Taille;

typedef struct
{
    int x,y;
}position;

typedef struct fenetre
{
  GtkWidget *window;  /* Notre fenetre */
  GSList *optionnel;
  char type_fenetre;  /* T pour toplevel et P pour POPUP */
  char titre[25];  /* titre de la fenetre */
  Taille  taille_fenetre;
  /* 0 pour GTK_WIN_POS_NONE : la fenêtre aura une position aléatoire
     lors de son affichage ;
     1 pour GTK_WIN_POS_CENTER : la fenêtre sera centrée à l'écran ;
     2 pour GTK_WIN_POS_MOUSE : le coin supérieur droit de la fenêtre
     correspondra à la position de la souris au moment de l'affichage ;
     3 pour GTK_WIN_POS_CENTER_ALWAYS : la fenêtre sera centrée
     et ne pourra être déplacée ;
     4 pour GTK_WIN_POS_CENTER_ON_PARENT : la fenêtre sera centrée
     par rapport à la fenêtre parente.*/
  int position_depart;
  position position_fenetre;
}fenetre;


fenetre* initialiser_fenetre(char type,char titre[25], int hauteur,int largeur,int pos,int x,int y)
{
    fenetre *mafenetre;
    mafenetre=(fenetre*)malloc(sizeof(fenetre));
    if(!mafenetre)
    {
        printf("\nERREUR D'ALLOCATION!");
        exit(1);
    }
    mafenetre->type_fenetre=type;
    strcpy(mafenetre->titre,titre);
    mafenetre->position_fenetre.x=x;
    mafenetre->position_fenetre.y=y;
    mafenetre->position_depart=pos;
    mafenetre->taille_fenetre.largeur=largeur;
    mafenetre->taille_fenetre.hauteur=hauteur;
    return(fenetre*)mafenetre;
}

void creation_fenetre(fenetre *mafenetre)
{
 if(mafenetre->type_fenetre=='T')
 mafenetre->window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
 if(mafenetre->type_fenetre=='P')
 mafenetre->window=gtk_window_new(GTK_WINDOW_POPUP);

/* Définition de la taille de la fenêtre */
 gtk_window_set_default_size(GTK_WINDOW(mafenetre->window),
                             mafenetre->taille_fenetre.largeur,
                             mafenetre->taille_fenetre.hauteur);
/* Titre de la fenêtre */
 gtk_window_set_title(GTK_WINDOW(mafenetre->window), mafenetre->titre);

if((mafenetre->position_fenetre.x==-1)||(mafenetre->position_fenetre.y==-1))
   /* Définition de la position */
    switch(mafenetre->position_depart)
    {
        case 0 : gtk_window_set_position(GTK_WINDOW(mafenetre->window),
                                         GTK_WIN_POS_NONE);
                break ;
        case 1 : gtk_window_set_position(GTK_WINDOW(mafenetre->window),
                                         GTK_WIN_POS_CENTER);
                break;
        case 2 : gtk_window_set_position(GTK_WINDOW(mafenetre->window),
                                          GTK_WIN_POS_MOUSE);
                break;
        case 3 : gtk_window_set_position(GTK_WINDOW(mafenetre->window),
                                         GTK_WIN_POS_CENTER_ALWAYS);
                break;
        case 4 : gtk_window_set_position(GTK_WINDOW(mafenetre->window),
                                          GTK_WIN_POS_CENTER_ON_PARENT);
                break;
      }
 else
 {
    gtk_window_set_default_size(GTK_WINDOW(mafenetre->window),mafenetre->position_fenetre.x,mafenetre->position_fenetre.y);
 }

}

/** Structure d'une fenetre qui contient scrollBar */
typedef struct scroll_window
{
  GtkWidget *swind;
  GtkWidget *parent;
  /* Choisir la manière selon laquelle va réagir la barre du coté droit
    1:GTK_POLICY_ALWAYS : si on veut que la barre de défilement soit toujours affichée.
    2:GTK_POLICY_AUTOMATIC
  */
  int side_bar;
 //Choisir la manière selon laquelle va réagir la barre du bas
  int bottom_bar;
}scroll_window;

/** Fonction qui ajoute un widget a son parent
    @param : parent le parent du widget
             child le widget a ajouté
    @return : void
*/
void ajouter_container(GtkWidget* parent,GtkWidget* child)
{
    gtk_container_add (GTK_CONTAINER (parent), child);
}

/** Fonction pour initialiser ScrolledWindow
*** @param : sidebar : 1 : toujours
***                    2 : Automatique
***                    3 : Jamais
***          BottomBar : 1 : toujours
***                      2 : Automatique
***                      3 : Jamais
***          SW : Structure de scrolled window
***          parent : le parent du widget
*** @return : le composant scrolled Window.
*/
scroll_window* creation_scroll_window(int sideBar,int bottomBar,GtkWidget *parent)
{
    GtkPolicyType tab[2];
    scroll_window *SW;
    SW = (scroll_window*)malloc(sizeof(scroll_window));
    SW->swind = gtk_scrolled_window_new(NULL,NULL);
    switch(sideBar)
    {
        case 1:tab[0]=GTK_POLICY_ALWAYS;break;
        case 2:tab[0]=GTK_POLICY_AUTOMATIC;break;
        case 3:tab[0]=GTK_POLICY_NEVER;break;
        default:tab[0]=GTK_POLICY_ALWAYS;break;
    }
    switch(bottomBar)
    {
     case 1:tab[1]=GTK_POLICY_ALWAYS;break;
     case 2:tab[1]=GTK_POLICY_AUTOMATIC;break;
     case 3:tab[1]=GTK_POLICY_NEVER;break;
     default:tab[1]=GTK_POLICY_ALWAYS;break;
    }
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(SW->swind),
                                    tab[1],tab[0]);

    //SW->parent->widget = parent;
    SW->side_bar = sideBar;
    SW->bottom_bar = bottomBar;
    //gtk_container_add (GTK_CONTAINER (parent), SW->swind);
    ajouter_container(parent,SW->swind);
    return ((scroll_window*)SW);
}


typedef struct Box
{
  GtkWidget *la_box;
  GtkWidget *parent;
  char type;              /* H: pour horizontal, V: pour vertical */
  int  homogene;          /* 1: pour TRUE , 0: pour FALSE */
  int espacement_widget;  /* l'espace entre les widgets */
}box;

box* initialiser_box(GtkWidget* parent,char type,int homogene,int espace)
{
    box *maBox = (box*)malloc(sizeof(box));
    if(type == 'H' || type == 'h')
        maBox->la_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, espace);
    if(type == 'V' || type == 'v')
        maBox->la_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, espace);
    //ajouter_container(parent,maBox->la_box);
    gtk_box_set_spacing (GTK_BOX (maBox->la_box),espace);
    maBox->parent = parent;
    maBox->type = type;
    maBox->homogene = homogene;
    maBox->espacement_widget = espace;
    return((box*)maBox);
}

typedef struct
{
    GtkWidget* fix;
    GtkWidget* parent;
}fixed;

fixed* initialiser_fixed_container(GtkWidget* parent)
{
    fixed * fixedContainer = (fixed*)malloc(sizeof(fixed));
    fixedContainer->fix = gtk_fixed_new();
    ajouter_container(parent,fixedContainer->fix);
    fixedContainer->parent = parent;
    return ((fixed*)fixedContainer);
}

void set_position(fixed *fixContainer,GtkWidget* widg,int x, int y)
{
    gtk_fixed_put(GTK_FIXED(fixContainer->fix),widg,x,y);
}

//void ajout_fixed_container()

typedef struct elem_radio
{
    GtkWidget *pRadio; // l'objet boutton radio radio
    char nom[30]; // le nom du boutton
    struct elem_radio * suivant; // pointeur sur le suivan
}elem_radio;

typedef struct radio
{
    GtkWidget * parent;
    //GtkWidget *pRadio; // l'objet boutton radio radio : a quoi sert ?
    elem_radio * liste; // les elements de la liste radio
}radio;



/*
ajouter une ligne radio a un boutton radio
entrees : liste des element du Radio
          le nom de la ligne a ajouter
sortie : liste apres l'ajout de l'element
*/
elem_radio * ajouter_radio_fin(elem_radio * l,char nom[30])
{
  elem_radio * elem=(elem_radio*)malloc(sizeof(elem_radio));
  elem->suivant=NULL;
  strcpy(elem->nom,nom);
  if(!l)return elem;
  elem_radio *t=l;
  while(l->suivant) l=l->suivant;
  l->suivant=elem;
  return t;
}

radio* ajouter_groupe_radio(elem_radio* liste,GtkWidget* parent)
{
    radio* radioButtons = (radio*)malloc(sizeof(radio));
    radioButtons->liste = liste;
    radioButtons->parent = parent;
    return ((radio*)radioButtons);
}

 /*
 fonction qui permet la creation d'un boutton radio
 entree : Un pointeur sur la structure Radio
 sortie : creation du boutton
 */
void creation_radio_bouton(radio * r)
{
 /* Creation du premier bouton radio */
/* if(r->liste)
   {r->liste->pRadio=gtk_radio_button_new_with_label(NULL,r->liste->nom);
   ajouter_container(r->parent,r->liste->pRadio);
   }
 else return ;
 */
 //elem_radio * l=r->liste->suivant;
 elem_radio * l=r->liste;
 while(l)
  { /* Ajout des boutons radio de la liste */
    l->pRadio=gtk_radio_button_new_with_label_from_widget
             (GTK_RADIO_BUTTON(r->liste->pRadio),l->nom);
    /** ajouter au container */
    ajouter_container(r->parent,l->pRadio);
    l=l->suivant;
  }
}
