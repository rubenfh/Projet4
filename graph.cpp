#include "graph.h"
#include <sstream>
#include <fstream>
namespace patch
{
template < typename T > std::string to_string( const T& n )
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}
}

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( patch::to_string(idx) );
}


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( patch::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( patch::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    ///RETOUR GRAPHE 1
    m_tool_box.add_child(m_box_menu);
    m_box_menu.set_dim(74,50);
    m_box_menu.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_box_menu.set_bg_color(ROUGECLAIR);

    m_box_menu.add_child( m_bouton1 );
    m_bouton1.set_frame(7,7,64,41);
    m_bouton1.set_bg_color(ROUGESOMBRE);

    m_bouton1.add_child(m_bouton1_label);
    m_bouton1_label.set_message("GRAPHE1");

    ///RETOUR GRAPHE 2
    m_tool_box.add_child(m_box_menu2);
    m_box_menu2.set_dim(74,50);
    m_box_menu2.set_gravity_x(grman::GravityX::Left);
    m_box_menu2.set_posy(56);
    m_box_menu2.set_bg_color(ROUGECLAIR);

    m_box_menu2.add_child( m_bouton1bis );
    m_bouton1bis.set_frame(7,7,64,41);
    m_bouton1bis.set_bg_color(ROUGESOMBRE);

    m_bouton1bis.add_child(m_bouton1bis_label);
    m_bouton1bis_label.set_message("GRAPHE2");

    ///RETOUR GRAPHE 3
    m_tool_box.add_child(m_box_menu3);
    m_box_menu3.set_dim(74,50);
    m_box_menu3.set_gravity_x(grman::GravityX::Left);
    m_box_menu3.set_posy(111);
    m_box_menu3.set_bg_color(ROUGECLAIR);

    m_box_menu3.add_child( m_bouton1ter );
    m_bouton1ter.set_frame(7,7,64,41);
    m_bouton1ter.set_bg_color(ROUGESOMBRE);

    m_bouton1ter.add_child(m_bouton1ter_label);
    m_bouton1ter_label.set_message("GRAPHE3");


    ///SAUVE

    m_tool_box.add_child(m_box_sauve);
    m_box_sauve.set_dim(74,50);
    m_box_sauve.set_gravity_x(grman::GravityX::Left);
    m_box_sauve.set_posy(166);
    m_box_sauve.set_bg_color(VERTCLAIR);

    m_box_sauve.add_child( m_bouton2 );
    m_bouton2.set_frame(7,7,64,41);
    m_bouton2.set_bg_color(VERTSOMBRE);

    m_bouton2.add_child(m_bouton2_label);
    m_bouton2_label.set_message("SAVE G1");

    ///SAUVE GRAPHE 2

    m_tool_box.add_child(m_box_sauvebis);
    m_box_sauvebis.set_dim(74,50);
    m_box_sauvebis.set_gravity_x(grman::GravityX::Left);
    m_box_sauvebis.set_posy(221);
    m_box_sauvebis.set_bg_color(VERTCLAIR);

    m_box_sauvebis.add_child( m_bouton2bis );
    m_bouton2bis.set_frame(7,7,64,41);
    m_bouton2bis.set_bg_color(VERTSOMBRE);

    m_bouton2bis.add_child(m_bouton2bis_label);
    m_bouton2bis_label.set_message("SAVE G2");

    ///SAUVE GRAPGHE 3

    m_tool_box.add_child(m_box_sauveter);
    m_box_sauveter.set_dim(74,50);
    m_box_sauveter.set_gravity_x(grman::GravityX::Left);
    m_box_sauveter.set_posy(276);
    m_box_sauveter.set_bg_color(VERTCLAIR);

    m_box_sauveter.add_child( m_bouton2ter );
    m_bouton2ter.set_frame(7,7,64,41);
    m_bouton2ter.set_bg_color(VERTSOMBRE);

    m_bouton2ter.add_child(m_bouton2ter_label);
    m_bouton2ter_label.set_message("SAVE G3");


    ///ADD
    m_tool_box.add_child(m_box_add);
    m_box_add.set_dim(74,50);
    m_box_add.set_gravity_x(grman::GravityX::Left);
    m_box_add.set_bg_color(JAUNECLAIR);
    m_box_add.set_posy(331);

    m_box_add.add_child( m_bouton3 );
    m_bouton3.set_frame(7,7,64,41);
    m_bouton3.set_bg_color(JAUNESOMBRE);

    m_bouton3.add_child(m_bouton3_label);
    m_bouton3_label.set_message("ADD");


    ///REMOVE

    m_tool_box.add_child(m_box_remove);
    m_box_remove.set_dim(74,50);
    m_box_remove.set_gravity_x(grman::GravityX::Left);
    m_box_remove.set_posy(386);
    m_box_remove.set_bg_color(ORANGECLAIR);

    m_box_remove.add_child( m_bouton4 );
    m_bouton4.set_frame(7,7,64,41);
    m_bouton4.set_bg_color(ORANGESOMBRE);

    m_bouton4.add_child(m_bouton4_label);
    m_bouton4_label.set_message("REMOVE");

    ///FORTEMENT CONNEXE

    m_tool_box.add_child(m_box_fconnexe);
    m_box_fconnexe.set_dim(74,50);
    m_box_fconnexe.set_gravity_x(grman::GravityX::Left);
    m_box_fconnexe.set_posy(441);
    m_box_fconnexe.set_bg_color(BLEUCLAIR);

    m_box_fconnexe.add_child( m_bouton5 );
    m_bouton5.set_frame(7,7,64,41);
    m_bouton5.set_bg_color(BLEU);

    m_bouton5.add_child(m_bouton5_label);
    m_bouton5_label.set_message("F CON");

    ///K CONNEXE

    m_tool_box.add_child(m_box_kconnexe);
    m_box_kconnexe.set_dim(74,50);
    m_box_kconnexe.set_gravity_x(grman::GravityX::Left);
    m_box_kconnexe.set_posy(496);
    m_box_kconnexe.set_bg_color(BLEUCLAIR);

    m_box_kconnexe.add_child( m_bouton5bis );
    m_bouton5bis.set_frame(7,7,64,41);
    m_bouton5bis.set_bg_color(BLEU);

    m_bouton5bis.add_child(m_bouton5bis_label);
    m_bouton5bis_label.set_message("K CON");


    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
}


/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    std::ifstream fichier("graphes1.txt",std::ios::in);
    int nbarrette,nbsommet,nsommet,x,y;
    float nombre;
    std::string nom;
    if (fichier)
    {
        fichier>>nbsommet;
        fichier>>nbarrette;
        for(int i=0; i<nbsommet; i++)
        {
            fichier>>nsommet;
            fichier>>nombre;
            fichier>>x;
            fichier>>y;
            fichier>>nom;
            add_interfaced_vertex(nsommet,nombre,x,y,nom);

        }
        fichier.close();

        std::ifstream fichier("grapha1.txt",std::ios::in);
        int indice, sommet1,sommet2;
        float poids;
        for(int i=0; i<nbarrette; i++)
        {

            fichier>>indice,fichier>>sommet1,fichier>>sommet2,fichier>>poids;
            add_interfaced_edge(indice,sommet1,sommet2,poids);

        }
        fichier.close();

    }



    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    /* add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
     add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
     add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
     add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
     add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
     add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
     add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
     add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);*/

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    /* add_interfaced_edge(0, 1, 2, 50.0);
     add_interfaced_edge(1, 0, 1, 50.0);
     add_interfaced_edge(2, 1, 3, 75.0);
     add_interfaced_edge(3, 4, 1, 25.0);
     add_interfaced_edge(4, 6, 3, 25.0);
     add_interfaced_edge(5, 7, 3, 25.0);
     add_interfaced_edge(6, 3, 4, 0.0);
     add_interfaced_edge(7, 2, 0, 100.0);
     add_interfaced_edge(8, 5, 2, 20.0);
     add_interfaced_edge(9, 3, 7, 80.0);*/
}

void Graph::make_example2()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    std::ifstream fichier("graphes2.txt",std::ios::in);
    int nbarrette,nbsommet,nsommet,x,y;
    float nombre;
    std::string nom;
    if (fichier)
    {
        fichier>>nbsommet;
        fichier>>nbarrette;
        for(int i=0; i<nbsommet; i++)
        {
            fichier>>nsommet;
            fichier>>nombre;
            fichier>>x;
            fichier>>y;
            fichier>>nom;
            add_interfaced_vertex(nsommet,nombre,x,y,nom);

        }
        fichier.close();

        std::ifstream fichier("grapha2.txt",std::ios::in);
        int indice, sommet1,sommet2;
        float poids;
        for(int i=0; i<nbarrette; i++)
        {

            fichier>>indice,fichier>>sommet1,fichier>>sommet2,fichier>>poids;
            add_interfaced_edge(indice,sommet1,sommet2,poids);

        }
        fichier.close();
    }
}

void Graph::make_example3()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    std::ifstream fichier("graphes3.txt",std::ios::in);
    int nbarrette,nbsommet,nsommet,x,y;
    float nombre;
    std::string nom;
    if (fichier)
    {
        fichier>>nbsommet;
        fichier>>nbarrette;
        for(int i=0; i<nbsommet; i++)
        {
            fichier>>nsommet;
            fichier>>nombre;
            fichier>>x;
            fichier>>y;
            fichier>>nom;
            add_interfaced_vertex(nsommet,nombre,x,y,nom);

        }
        fichier.close();

        std::ifstream fichier("grapha3.txt",std::ios::in);
        int indice, sommet1,sommet2;
        float poids;
        for(int i=0; i<nbarrette; i++)
        {

            fichier>>indice,fichier>>sommet1,fichier>>sommet2,fichier>>poids;
            add_interfaced_edge(indice,sommet1,sommet2,poids);

        }
        fichier.close();
    }
}
/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    Graph g;

    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();
    for (auto &elt : m_edges)
        elt.second.post_update();
    m_top_box.update();

    if(m_interface -> m_bouton4.clicked())
    {
        Supprimer();
    }

    if(m_interface -> m_bouton3.clicked())
    {
        Ajouter1();
    }

    if (m_interface -> m_bouton2.clicked())
    {
        savecoord1( m_vertices);
    }

    if (m_interface -> m_bouton2bis.clicked())
    {
        savecoord2(m_vertices);
    }

    if (m_interface -> m_bouton2ter.clicked())
    {
        savecoord3(m_vertices);
    }

    int a=1;
    int b=1;
    int c=1;

    if (m_interface -> m_bouton1.clicked())
    {
        while(a==1)
        {
            //g.unload_graph();
            g.make_example();
            g.update();
            grman::mettre_a_jour();
        }
    }

    if (m_interface -> m_bouton1bis.clicked())
    {
        while(b==1)
        {
            //g.unload_graph();
            g.make_example2();
            g.update();
            grman::mettre_a_jour();
        }
    }

    if (m_interface -> m_bouton1ter.clicked())
    {
        while(c==1)
        {
            //g.unload_graph();
            g.make_example3();
            g.update();
            grman::mettre_a_jour();
        }
    }

    /*if (m_interface -> m_bouton5.clicked())
    {

    }

    if (m_interface -> m_bouton5bis.clicked())
    {

    }*/


}

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide � l'ajout d'arcs interfac�s
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    m_edges[idx].m_from=id_vert1;
    m_edges[idx].m_to=id_vert2;
    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_out.push_back(idx);


}



void Graph::savecoord1(std::map<int, Vertex> m_vertices)
{
    std::string buff;
    std::ofstream fichier("graphes1.txt",std::ios::out|std::ios::trunc);

    fichier<<m_vertices.size();
    fichier<< " ";
    fichier<<m_edges.size();
    fichier<<std::endl;

    for(unsigned int i=0; i<m_vertices.size(); i++)
    {
        fichier<<i<<" "<<m_vertices[i].m_value<<" "<< m_vertices[i].m_interface->m_top_box.get_posx()<<" "<<m_vertices[i].m_interface->m_top_box.get_posy()<< " "<<m_vertices[i].m_interface->m_img.m_pic_name;
        fichier<<std::endl;
    }
    fichier.close();

    std::ofstream fichier1("graphea1.txt",std::ios::out|std::ios::trunc);
    for(unsigned int i=0; i<m_edges.size(); i++)
    {
        fichier<< i << " "<< m_edges[i].m_from<< " " << m_edges[i].m_to<< " " << m_edges[i].m_weight;
        fichier<< std::endl;
    }
    fichier1.close();
}

void Graph::savecoord2(std::map<int, Vertex> m_vertices)
{
    std::string buff;
    std::ofstream fichier("graphes2.txt",std::ios::out|std::ios::trunc);

    fichier<<m_vertices.size();
    fichier<< " ";
    fichier<<m_edges.size();
    fichier<<std::endl;

    for(unsigned int i=0; i<m_vertices.size(); i++)
    {
        fichier<<i<<" "<<m_vertices[i].m_value<<" "<< m_vertices[i].m_interface->m_top_box.get_posx()<<" "<<m_vertices[i].m_interface->m_top_box.get_posy()<< " "<<m_vertices[i].m_interface->m_img.m_pic_name;
        fichier<<std::endl;
    }
    fichier.close();

    std::ofstream fichier1("graphea2.txt",std::ios::out|std::ios::trunc);
    for(unsigned int i=0; i<m_edges.size(); i++)
    {
        fichier<< i << " "<< m_edges[i].m_from<< " " << m_edges[i].m_to<< " " << m_edges[i].m_weight;
        fichier<< std::endl;
    }
    fichier1.close();
}

void Graph::savecoord3(std::map<int, Vertex> m_vertices)
{
    std::string buff;
    std::ofstream fichier("graphes3.txt",std::ios::out|std::ios::trunc);

    fichier<<m_vertices.size();
    fichier<< " ";
    fichier<<m_edges.size();
    fichier<<std::endl;

    for(unsigned int i=0; i<m_vertices.size(); i++)
    {
        fichier<<i<<" "<<m_vertices[i].m_value<<" "<< m_vertices[i].m_interface->m_top_box.get_posx()<<" "<<m_vertices[i].m_interface->m_top_box.get_posy()<< " "<<m_vertices[i].m_interface->m_img.m_pic_name;
        fichier<<std::endl;
    }
    fichier.close();

    std::ofstream fichier1("graphea3.txt",std::ios::out|std::ios::trunc);
    for(unsigned int i=0; i<m_edges.size(); i++)
    {
        fichier<< i << " "<< m_edges[i].m_from<< " " << m_edges[i].m_to<< " " << m_edges[i].m_weight;
        fichier<< std::endl;
    }
    fichier1.close();
}

void Graph::Supprimer()
{
    int n;
    //m_interface->m_top_box.update();


    std::cout <<"quel sommet voulez vous suprimer ?" <<std::endl;
    std::cin>>n;


    for(auto& elem:m_edges)
    {
        if((elem.second.m_to==n)||(elem.second.m_from==n))
        {
            test_remove_edge(elem.first);

        }
    }
}


void Graph::Ajouter1()
{
    unsigned int n=0,sommet1,sommet2;
    float poid;
    bool ok=false;
    //1std::ofstream fichier1("grapha1.txt",std::ios::app);
    do
    {
        if(m_edges.count(n)==1)
        {
            n++;
        }
        else
        {
            ok=true;
        }
    }
    while(!ok);


    //fichier1<<n<<" ";
    do
    {
        std::cout<<"choisir un sommet de d�part"<<std::endl;
        std::cin>> sommet1;

        std::cout<<"choisir un sommet d'arriv�"<<std::endl;
        std::cin>>sommet2;
    }
    while((sommet1==sommet2));

    //fichier1<<sommet1<<" ",fichier1<<sommet2<<" ";
    do
    {
        std::cout<<"saisissez un poid"<<std::endl;
        std::cin >>poid;
    }
    while((poid<0)||(poid>100));
    //fichier1<<poid<<std::endl;
    add_interfaced_edge(n,sommet1,sommet2,poid);


    //fichier1.close();

}

/// eidx index of edge to remove
void Graph::test_remove_edge(int eidx)
{
/// r�f�rence vers le Edge � enlever
    Edge &remed=m_edges.at(eidx);

    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

/// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

/// test : on a bien des �l�ments interfac�s
    if (m_interface && remed.m_interface)
    {
/// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge); */
        /* m_edges[idx] = Edge(weight, ei); */
/// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr
/// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
/// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

/// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !
/// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

/// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
/// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface
/// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );

/// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

}

/*void Graph::test_remove_vertex(int eidx)
{
    Vertex &remvert=m_vertices.at(eidx);
    m_interface->m_main_box.remove_child(remvert.m_interface->m_top_box);
    remvert.m_interface->m_select.set_value(false);
    m_vertices_no[eidx]=remvert;
    m_vertices.erase(eidx);
    m_interface->m_second_box.add_child(remvert.m_interface->m_top_box);
    remvert.m_interface->m_top_box.set_pos(remvert.m_interface->m_top_box.get_posx(),remvert.m_interface->m_top_box.get_posy()/4.5);
}


void Graph::suppr()
{
    bool sortie=false;
    int eidx;
    if (m_interface->m_delete_box.m_value==true)
    {
        m_interface->m_delete_box.m_value=false;
        while(sortie!=true)
        {
            for(std::map<int,Edge>::iterator it = m_edges.begin(); it!=m_edges.end(); ++it)
            {
                if((m_vertices[it->second.m_from].m_interface->m_select.get_value()==1)||(m_vertices[it->second.m_to].m_interface->m_select.get_value()==1))
                {
                    eidx=it->first;
                    test_remove_edge(eidx);
                    sortie=false;
                    if(m_nb_arcs==0)
                    {
                        sortie=true;
                    }
                }
                else
                {
                    sortie=true;
                }
            }
            for(std::map<int, Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); it++)
            {
                if(it->second.m_interface->m_select.get_value())
                {
                    test_remove_vertex(it->first);
                    sortie=false;
                    if(m_vertices.size()==0)
                    {
                        sortie=true;
                    }
                    break;
                }
                else
                {
                    sortie=true;
                }
            }
        };
    }
}*/


