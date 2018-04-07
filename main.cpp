#include "grman/grman.h"
#include <iostream>

#include "graph.h"
void menu()
{
    BITMAP* fe;
    BITMAP* buffer;
    fe=  load_bitmap("menu.jpg",NULL);
    buffer= create_bitmap(SCREEN_W,SCREEN_H);
    blit(fe,buffer,0,0,0,0,800,600);
    blit(buffer,screen,0,0,0,0,800,600);
    grman::set_pictures_path("pics");
    bool x = false;
    Graph g;

    while( !key[KEY_ESC])
    {
        if(mouse_b&1 && 308 < mouse_x && mouse_x <507 && 86<mouse_y && mouse_y < 138)
        {

            g.make_example();
            x=true;
            while(x==true)
            {

                g.update();
                grman::mettre_a_jour();

                if(key[KEY_BACKSLASH])
                {
                    x=false;
                    blit(fe,buffer,0,0,0,0,800,600);
                    blit(buffer,fe,0,0,0,0,800,600);
                }
            }
        }

    }
}
/*void menu2()
{
    grman::init();
    grman::set_pictures_path("pics");
   /// Un exemple de graphe
    int choix1;
    Graph g;

    do
    {
        std::cout<<"Choix : ";
        std::cin>>choix1;
    }
    while(choix1 < 1 || choix1 >3);


    switch(choix1)
    {
    case 1 :
        g.make_example();
        break;

    case 2 :
        g.make_example2();
        break;

    case 3:
        g.make_example3();
        break;
    }
}*/

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    Graph g;
    //menu2();

    /// Un exemple de graphe
    int choix1;

    do
    {
        std::cout<<"Choix : ";
        std::cin>>choix1;
    }
    while(choix1 < 1 || choix1 >3);


    switch(choix1)
    {
    case 1 :
        g.make_example();
        break;

    case 2 :
        g.make_example2();
        break;

    case 3:
        g.make_example3();
        break;
    }


    BITMAP* fe;
    BITMAP* buffer;
    fe=  load_bitmap("pics/menu.jpg",NULL);
    buffer= create_bitmap(SCREEN_W,SCREEN_H);
    blit(fe,buffer,0,0,0,0,800,600);
    blit(buffer,screen,0,0,0,0,800,600);

    bool x = false;

    grman::set_pictures_path("pics");

    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


