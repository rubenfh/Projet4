#include "grman/grman.h"
#include <iostream>

#include "graph.h"
/*void menu()
{
    BITMAP *page;
    BITMAP *image;
    page=create_bitmap(800,600);
    image=load_bitmap("menu.bmp",NULL);
   Graph g;
    bool booleen = false, test = false;
    grman::set_pictures_path("pics");


    while (!key[KEY_ESC])
    {
        blit(image,page,0,0,0,0,800,600);
        blit(page,screen,0,0,0,0,800,600);
        grman::set_pictures_path("pics");
        //g.make_example();
        test = false;


        if (mouse_b&1 && mouse_x>300 && mouse_x<492 && mouse_y>142 && mouse_y<214 )
        {

            booleen = true;
            g.make_example();
            while (booleen == true)
            {
                g.update();
                grman::mettre_a_jour();
               // g.make_example();

                if(key[KEY_S])
                {
                    booleen=false;
                    blit(image,page,0,0,0,0,800,600);
                    blit(page,screen,0,0,0,0,800,600);
                    menu();

                }

            }

        }

        else if (mouse_b&1 && mouse_x>300 && mouse_x<492 && mouse_y>231 && mouse_y<304 )
        {
            booleen = true;
            g.make_example2();
            while (booleen == true)
            {
                g.update();
                grman::mettre_a_jour();

            if(key[KEY_S])
                {
                    booleen=false;
                    blit(image,page,0,0,0,0,800,600);
                    blit(page,screen,0,0,0,0,800,600);
                    menu();
                }
            }


        }

        else if (mouse_b&1 && mouse_x>300 && mouse_x<492 && mouse_y>324 && mouse_y<395)
        {
            booleen = true;
            g.make_example3();
            while (booleen == true)
            {
                g.update();
                grman::mettre_a_jour();

                if(key[KEY_S])
                {
                    booleen=false;
                    blit(image,page,0,0,0,0,800,600);
                    blit(page,screen,0,0,0,0,800,600);
                    menu();

                }
            }


        }
    }
}*/

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe

    Graph g;
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


