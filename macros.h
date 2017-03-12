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

typedef struct zone_texte
{
    char texte[255];
    GtkWidget* pTextView;
    GtkWidget * parent;

}zone_texte;

typedef struct Select_fichier
{
  GtkWidget *file_selection;
  GtkWidget *parent;
  char *titre;
  char *chemin;
  GtkFileChooserAction action;

}select_fichier;

typedef struct zone_saisie
{
   GtkWidget *entry;
   GtkWidget *parent;
   int longueur_max;
   char message[70];
   int visible;           /* 1: caractères visibles , 0: non visible */
   char invisible;        /* le caractère d'invisibilité */
}zone_saisie;

typedef struct bouton
{
    GtkWidget *pbtn;
    GtkStockItem * icone ;
    char label[30];
    int boolean ; // si le label et avec mnemonic ou non
    GtkWidget * parent;
}bouton ;

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


typedef struct elem_check
{
    GtkWidget *pCheck; // l'objet boutton radio radio
    char nom[30]; // le nom du boutton
    //struct elem_check * suivant; // pointeur sur le suivan
    GtkWidget* parent;
}elem_check;

elem_check* initialiser_check_button(char nom[30],GtkWidget* parent)
{
    elem_check * elem=(elem_check*)malloc(sizeof(elem_check));
    elem->parent = parent;
    strcpy(elem->nom,nom);
    return ((elem_check*)elem);
}

void creation_check_bouton(elem_check * check)
{
    check->pCheck = gtk_check_button_new_with_label (check->nom);
    ajouter_container(check->parent,check->pCheck);
}

typedef struct layout
{
  GtkWidget *conteneur;
  GtkWidget *parent;
  Taille t;
}layout;

void creation_layout(GtkWidget* parent,int largeur,int hauteur)
{
    layout* l = (layout*)malloc(sizeof(layout));
    l->parent = parent;
    l->t.largeur = largeur;
    l->t.hauteur = hauteur;
    l->conteneur=gtk_layout_new (NULL,NULL);
    ajouter_container(parent,l->conteneur);
    gtk_layout_set_size (GTK_LAYOUT(l->conteneur),l->t.largeur,l->t.hauteur);
}

void ajout_composants_layout(layout *l,GtkWidget *child,int x,int y)
{
  gtk_layout_put(GTK_LAYOUT(l->conteneur),child,x,y);
}

void move_child_inlayout(layout *l,GtkWidget *child,int x,int y)
{
   gtk_layout_move (GTK_LAYOUT(l->conteneur),child,x,y);
}

bouton* initialiser_bouton(GtkWidget* parent,GtkStockItem * icone,char titre[30],int boolean)
{
    bouton* btn = (bouton*)malloc(sizeof(bouton));
    btn->parent = parent;
    btn->icone = icone;
    btn->boolean = boolean;
    strcpy(btn->label,titre);
    return ((bouton*)btn);
}

void creation_bouton_simple(bouton * b)
{

    if(b->icone)
     {
        /* Bouton avec un label, un raccourci et une image */
        b->pbtn= gtk_button_new_from_icon_name((char*)b->icone,GTK_ICON_SIZE_BUTTON);
     }
     else
     {
       if(!b->boolean)            /* Bouton avec un label */
            b->pbtn = gtk_button_new_with_label(b->label);
       else           /* Bouton avec un label et un raccourci */
           b->pbtn=gtk_button_new_with_mnemonic(b->label);
     }
     ajouter_container(b->parent,b->pbtn);
}


zone_saisie* initialiser_zone_saisie
        (GtkWidget* parent,int maxlength,char msg[70],int visibility,char invisible)
{
    zone_saisie *Entry = (zone_saisie*)malloc(sizeof(zone_saisie));
    Entry->parent = parent;
    Entry->longueur_max = maxlength;
    strcpy(Entry->message,msg);
    Entry->visible = visibility;
    Entry->invisible = invisible;
    return ((zone_saisie*)Entry);
}

void creation_zone_saisie(zone_saisie *zone)
{
  zone->entry=gtk_entry_new();
  if(zone->longueur_max!=-1)
  gtk_entry_set_max_length((GtkEntry*)zone->entry,zone->longueur_max);

  if(zone->message)
  {
      //zone->message = g_locale_to_utf8(zone->message,-1, NULL, NULL, NULL);
      gtk_entry_set_text(GTK_ENTRY(zone->entry),zone->message);
  }
  if(zone->visible==1)
  gtk_entry_set_visibility(GTK_ENTRY(zone->entry),TRUE);
  else
  {
  gtk_entry_set_visibility(GTK_ENTRY(zone->entry),FALSE);
  gtk_entry_set_invisible_char(GTK_ENTRY(zone->entry),zone->invisible);
  }
  ajouter_container(zone->parent,zone->entry);
}

char *recuperer_entry_text(GtkWidget *entry)
{
   return ((char*)gtk_entry_get_text(GTK_ENTRY(entry)));
}


zone_texte* initialiser_zone_texte(GtkWidget* parent)
{
    zone_texte* texte = (zone_texte*)malloc(sizeof(zone_texte));
    texte->parent = parent;
    return ((zone_texte*)texte);
}
/*
fonction qui permet la creation d'une Zone de texte
entree : pointeur sur la structure zone_texte
sortie : creation du composant
*/
void creation_zone_texte(zone_texte * zt)
{
    zt->pTextView = gtk_text_view_new();
    //ajouter_container(zt->parent,zt->pTextView);
    //gtk_box_pack_start(GTK_BOX(zt->parent),zt->pTextView,TRUE,TRUE,0);
}
