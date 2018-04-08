#include "graph.h"
#include <sstream>
#include <fstream>
#include <stack>
#include <queue>
#include <set>
using namespace std;
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

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
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


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( patch::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
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

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( patch::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
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



    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    /* add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
     add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
     add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
     add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
     add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
     add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
     add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
     add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);*/

    /// Les arcs doivent être définis entre des sommets qui existent !
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
/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
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
    if(key[KEY_S])
    {
        Supprimer();
    }

    if(key[KEY_Q])
    {
        Ajouter1();
    }

    if (key[KEY_C])
    {
        savecoord1( m_vertices);
    }

    if (key[KEY_V])
    {
        savecoord2(m_vertices);
    }

    if (key[KEY_B])
    {
        savecoord3(m_vertices);
    }
    if(key[KEY_E])
    {
        initMatriceAdj();
        SCC();
    }
}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
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
     m_vertices[id_vert1].m_in.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);


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
    m_interface->m_top_box.update();


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
        std::cout<<"choisir un sommet de départ"<<std::endl;
        std::cin>> sommet1;

        std::cout<<"choisir un sommet d'arrive"<<std::endl;
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

void Graph::initMatriceAdj()
{

    adj= new list <int> [m_edges.size()];

    for(unsigned int i=0; i<m_edges.size(); i++)
    {
        adj[m_edges[i].m_from].push_back(m_edges[i].m_to);
    }
}
void Graph::SCCutil(int u, int disc[], int low[], stack<int> *st,bool stackmember[])
{


    static int time = 0;

    // Initialize discovery time and low value
    disc[u] = low[u] = ++time;
    st->push(u);
    stackmember[u] = true;

    // Go through all vertices adjacent to this
    list<int>::iterator i;
    for (i = adj[u].begin(); i != adj[u].end(); ++i)
    {
        int v = *i;  // v is current adjacent of 'u'

        // If v is not visited yet, then recur for it
        if (disc[v] == -1)
        {
            SCCutil(v, disc, low, st, stackmember);

            // Check if the subtree rooted with 'v' has a
            // connection to one of the ancestors of 'u'
            // Case 1 (per above discussion on Disc and Low value)
            low[u]  = min(low[u], low[v]);
        }

        // Update low value of 'u' only of 'v' is still in stack
        // (i.e. it's a back edge, not cross edge).
        // Case 2 (per above discussion on Disc and Low value)
        else if (stackmember[v] == true)
            low[u]  = min(low[u], disc[v]);
    }

    // head node found, pop the stack and print an SCC
    int w = 0;  // To store stack extracted vertices
    if (low[u] == disc[u])
    {
        while (st->top() != u)
        {
            w = (int) st->top();
            cout << w << " ";
            stackmember[w] = false;
            st->pop();
        }
        w = (int) st->top();
        cout << w << std::endl;
        stackmember[w] = false;
        st->pop();
    }
}

void Graph::SCC()
{
    int* disc =new int [m_edges.size()];
    int* low = new int [m_edges.size()];
    bool* stackmember = new bool [m_edges.size()];
    std::stack<int>* st= new stack<int>();

    for(unsigned int i=0; i<m_edges.size(); i++)
    {
        disc[i]=-1;
        low[i]=-1;
        stackmember[i]=false;
    }

    for (unsigned int i=0; i<m_edges.size(); i++)
    {
        if (disc[i]==-1)
        {
            SCCutil(i,disc,low,st,stackmember);
        }
    }
}



void Graph::PartieFonctionnelle()
{

    float k=0;

    pred();
   // calculK();
    //calculN();

    for (auto &tx: m_vertices)
    {
        for( auto &ty: tx.second.m_in)
        {
            for(auto &tz:m_edges)
            {      //calcul de k avec un r =0.000001
                  k+= 0.01*tz.second.m_weight* m_vertices[ty].m_value/m_vertices.size();
            }
        }
        //on considère qu'il y a de l'herbe à l'infini
       if(tx.second.m_interface->m_img.m_pic_name=="herbe.jpg")
       {
           tx.second.m_value=100;
       }
        if (tx.second.m_value >0)
        {
            //tx.second.m_value=( tx.second.m_value)+0.01*(1-( tx.second.m_value)/(k));
            tx.second.m_value= tx.second.m_value + 0.01*tx.second.m_value*(1 - (tx.second.m_value/(k/1.3))) -(k/100);
        }
    }





    }


/*void Graph::calculK()
{
    for(unsigned int i=0; i<m_vertices.size(); i++)
    {
        int J=0;

      for(unsigned int j=0;j<m_pred.size();j++)
       {
        int z=m_vertices[i].m_predecesseur[j];

        if(m_edges[i].m_from==z)
        {
           J=J+m_vertices[z].m_value*m_edges[i].m_weight ;
        }

       }
    }

}

void Graph::calculN()
{
     int N=0;
    int K=0;
    int nb;


    for(unsigned int i=0; i<m_vertices.size(); i++)
    { int J=0;
      for(unsigned int j=0;j<m_pred.size();j++)
       {
        int z=m_vertices[i].m_predecesseur[j];

        if(m_edges[i].m_from==z)
        {
           J=J+m_vertices[z].m_value*m_edges[i].m_weight ;
        }

       }
        K=m_vertices[i].m_K;
        N= (m_vertices[i].m_value) + (0.01)*(m_vertices[i].m_value)*(1-(m_vertices[i].m_value)/K);

        /// on recupere le nombre de ses predecesseurs
        nb=m_vertices[i].m_predecesseur.size();

         m_vertices[i].m_value=( m_vertices[i].m_value)+( 0.01)*(1-( m_vertices[i].m_value)/K)-J;
                     std::cout << " le K est: " << K << " nombre de S" << i <<" est : " << m_vertices[i].m_value <<std::endl;

                }
}*/


void Graph::pred()
{
  int arc,sommet,nb;

  for(int i=0; i<m_edges.size();i++)
  {
      nb=m_vertices[i].m_in.size();
      for(int j=0;j<nb;j++)
      {
          arc=m_vertices[i].m_in[j];
          sommet=m_edges[arc].m_to;
          m_vertices[i].m_predecesseur.push_back(sommet);
          /*for(int k=0;k<m_vertices[i].m_predecesseur.size();k++)
          {
              std::cout <<i<< " "<<m_vertices[i].m_predecesseur[k];

          }*/

      }
  }

}


void Graph::erase1edge(int eidx)
{


    Edge &remed=m_edges.at(eidx);
    if (m_interface && remed.m_interface)
    {
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );
    m_edges.erase( eidx );
}


/// eidx index of edge to remove
void Graph::test_remove_edge(int eidx)
{
/// référence vers le Edge à enlever
    Edge &remed=m_edges.at(eidx);

    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

/// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

/// test : on a bien des éléments interfacés
    if (m_interface && remed.m_interface)
    {
/// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge); */
        /* m_edges[idx] = Edge(weight, ei); */
/// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr
/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

/// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !
/// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface
/// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );

/// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

}


