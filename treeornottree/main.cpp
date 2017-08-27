#include <iostream>
#include <fstream>


using namespace std;

struct Node
{
    Node(double data=0)
    {
        this->data=data;
    }
    double data;
};

class Graph
{
public:
    Graph();
    ~Graph();
    void Load();
    // Дерево - связный граф без циклов
    bool isTree();
private:
    int** linkMatrix=nullptr; // матрица связности
    int n=0;                  // кол-во вершин
    Node** graph=nullptr;     // data
    int* touch=nullptr;       // для DFS
    void dfs(int);            // gjbcr d uke,bye
};
/// MAIN
int main()
{
    Graph g;
    (g.isTree())? std::cout << "YES\n" : std::cout << "NO\n";
    return 0;
}
/// MAIN END
Graph::Graph()
{
    Load();
}
Graph::~Graph()
{
    for (int i = 0; i < n; i++)
    {
        delete graph[i];

    }
    delete[] graph;

    for (int i = 0; i < n; i++)
    {
        delete[] linkMatrix[i];
    }
    delete[] linkMatrix;

    if(touch!=nullptr)
        delete[] touch;
}
void Graph::Load()
{
    try
    {
        ifstream inp("input.txt");
        if(!inp.is_open()) throw true;
        inp >> n;

        linkMatrix = new int*[n];
        graph = new Node*[n];

        for (int i = 0; i < n; i++)
            linkMatrix[i] = new int [n];

        for (int i = 0; i < n; i++)
        {
            double tmp;
            inp >> tmp;
            graph[i] = new Node(tmp);
        }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                inp >> linkMatrix[i][j];

        inp.close();
    } catch(bool e)
    {
        cout << "File not found\n";
    }

}
bool Graph::isTree()
{
    touch = new int[n] {0};
    int edges = 0;

    // для связности необходимо количество ребер = n-1
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (linkMatrix[i][j])
                edges++;

    if (edges != n-1)
        return false;

    // отсутствие циклов
    dfs(0);
    for (int i = 0; i < n; ++i)
        if (touch[i]>1)
            return false;
  return true;
}

void Graph::dfs(int i)
{
    if (touch[i])
    {
        touch[i]++;
        return;
    }
    touch[i] = true;
    for (int j = 0; j < n; ++j)
        if (linkMatrix[i][j])
            dfs(j);
}
