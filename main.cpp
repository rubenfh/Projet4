#include "grman/grman.h"
#include <iostream>

#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
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


