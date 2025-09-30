#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <limits>
#include <iomanip> // para setw, melhora a amostragem do console
#include <cstdlib> // Para system()
using namespace std;

struct Grafo {
    int numVertices;
    bool dirigido;
    vector<vector<int>> matriz; // matriz de adjacência

    Grafo(int n, bool d = false) { // Construtor recebe numero de vertices, e se é dirigido
        numVertices = n;
        dirigido = d;
        matriz.assign(n, vector<int>(n, 0)); // inicializa com zeros
    }

    void adicionarAresta(int origem, int destino) {
        if ( origem!=destino ) {
            matriz[origem][destino] = 1;
            if (!dirigido) {
                matriz[destino][origem] = 1;
            }
        }
    }

    void adicionarVertice() {
        for ( int i = 0; i < numVertices; i++ ) {
            this->matriz[i].push_back(0);
        }
        this->numVertices++;
        this->matriz.push_back( vector<int>(numVertices, 0) );
    }

    bool removerVertice(int v) { // v é passado pelo usuário
        v--; // reduzimos v em 1, para ser equivalente ao valor da matriz
        if (v < 0 || v >= this->numVertices) return false;
        this->matriz.erase( this->matriz.begin()+v ); // apaga a linha do vertice na matriz
        for ( int i = 0; i < this->numVertices; i++ ) {
            this->matriz[i].erase( this->matriz[i].begin()+v ); // apaga a coluna equivalente do vertice de todos os outros vertices
        }
        this->numVertices--;
        return true;
    }

    void imprimirMatriz() {
        cout << "Matriz " << numVertices << "x" << numVertices << ":\n";
        cout << "      ";
        for (int j = 0; j < numVertices; j++) cout << setw(4) << j+1;
        cout << "\n";
        for (int i = 0; i < numVertices; i++) {
            cout << setw(3) << i+1 << " ->";
            for (int j = 0; j < numVertices; j++) {
                cout << setw(4) << matriz[i][j];
            }
            cout << "\n";
        }
    }
};

vector<int> busca_profundidade ( const vector<vector<int>> &matriz, int comecoXY = 0 ) {
    vector<int> visitados;
    stack<int> pilha;
    int vertice;
    
    pilha.push(comecoXY);
    while ( !pilha.empty() ) {
        vertice = pilha.top(); // top() retorna o elemento no topo, mas não remove
        pilha.pop();  // pop() remove o elemento no topo, mas não o retorna
        if( count(visitados.begin(), visitados.end(), vertice) == 0 ) {
            visitados.push_back(vertice);
            for ( int i = matriz[vertice].size()-1; i >= 0; i-- ) { // Percorre ao contrário para funcionamento correto
                if ( matriz[vertice][i] == 1 ) {
                    pilha.push(i);
                }
            }
        }
    }

    return visitados;
}

vector<int> busca_profundidade_completa ( const vector<vector<int>> &matriz, int comecoXY = 0 ) {
    vector<int> visitados;
    stack<int> pilha;
    int vertice;
    
    pilha.push(comecoXY);
    while ( !pilha.empty() ) {
        vertice = pilha.top(); // top() retorna o elemento no topo, mas não remove
        pilha.pop();  // pop() remove o elemento no topo, mas não o retorna
        if( count(visitados.begin(), visitados.end(), vertice) == 0 ) {
            visitados.push_back(vertice);
            for ( int i = matriz[vertice].size()-1; i >= 0; i-- ) { // Percorre ao contrário para funcionamento correto
                if ( matriz[vertice][i] == 1 ) {
                    pilha.push(i);
                }
            }
        }
    }

    if ( visitados.size() < matriz.size() ) { // Verifica se existem Vertices nao exploradas
        for ( int i = 0; i < matriz.size()-1; i++ ) {
            if( count(visitados.begin(), visitados.end(), i) == 0 ) { // Se o Vertice nao foi visitado, performa DFS a partir dele
                pilha.push(i);
                while ( !pilha.empty() ) {
                    vertice = pilha.top();
                    pilha.pop();
                    if( count(visitados.begin(), visitados.end(), vertice) == 0 ) {
                        visitados.push_back(vertice);
                        for ( int i = matriz[vertice].size()-1; i >= 0; i-- ) {
                            if ( matriz[vertice][i] == 1 ) {
                                pilha.push(i);
                            }
                        }
                    }
                }
            }
        }
    }

    return visitados;
}

vector<int> busca_largura_completa ( const vector<vector<int>> &matriz, int comecoXY = 0 ) {
    vector<int> visitados;
    queue<int> fila;
    int vertice;
    
    fila.push(comecoXY);
    while ( !fila.empty() ) {
        vertice = fila.front(); // front() retorna o elemento na frente, mas não remove
        fila.pop();  // pop() remove o elemento na frente, mas não o retorna
        if( count(visitados.begin(), visitados.end(), vertice) == 0 ) {
            visitados.push_back(vertice);
            for ( int i = 0; i < matriz[vertice].size(); i++ ) {
                if ( matriz[vertice][i] == 1 ) {
                    fila.push(i);
                }
            }
        }
    }

    if ( visitados.size() < matriz.size() ) { // Verifica se existem Vertices nao explorados
        for ( int i = 0; i < matriz.size()-1; i++ ) {
            if( count(visitados.begin(), visitados.end(), i) == 0 ) { // Se o Vertice nao foi visitado, performa BFS a partir dele
                fila.push(i);
                while ( !fila.empty() ) {
                    vertice = fila.front();
                    fila.pop();
                    if( count(visitados.begin(), visitados.end(), vertice) == 0 ) {
                        visitados.push_back(vertice);
                        for ( int i = 0; i < matriz[vertice].size(); i++ ) {
                            if ( matriz[vertice][i] == 1 ) {
                                fila.push(i);
                            }
                        }
                    }
                }
            }
        }
    }
    return visitados;
}

void percorrer( const vector<vector<int>> &matriz ){
    int tam = matriz.size();
    char metodo;
    int inicio=0;
    cout << "Qual será o método para percorrer?(b ou B -> BFS, qualquer outro caractere -> DFS)\n";
    cin >> metodo;

    cout << "Qual o vertice inicial para percorrer?(entre 1 e " << tam << ")\n";
    cin >> inicio;

    while ( inicio <= 0 || inicio >= tam ) {
        cout << "Vertice inválido (válido de 1 até " << tam << ")\n";
        cin >> inicio;
    }
    
    cout << endl;

    if( metodo == 66 || metodo == 98 ) {
        vector<int> exemploBfs = busca_largura_completa(matriz, inicio-1);
        for (int i = 0; i < exemploBfs.size(); i++) {
            cout << exemploBfs[i]+1 << endl;
        }
    } else {
        vector<int> exemploDfs = busca_profundidade_completa(matriz, inicio-1);
        for (int i = 0; i < exemploDfs.size(); i++) {
            cout << exemploDfs[i]+1 << endl;
        }
    }
}

vector<vector<int>> transpor ( const vector<vector<int>> &matriz ) {
    vector<vector<int>> matriz_t(matriz.size(), vector<int>(matriz.size(), 0));
    for (int i = 0; i < matriz.size(); ++i) {
        for (int j = 0; j < matriz.size(); ++j) {
            matriz_t[j][i] = matriz[i][j];
        }
    }
    return matriz_t; // retorna a matriz transposta
}

bool verifica_conectividade ( const vector<vector<int>> &matriz, bool dirigido ) {
    vector<int> visitados;
    visitados = busca_profundidade(matriz, 0);

    if( !dirigido ) {
        if ( visitados.size() == matriz.size() ) { // Se o grafo é não dirigido e percorre todos os vertices durante a DFS, o grafo é conexo
            return true;
        } else {
            return false;
        }
    }

    if ( visitados.size() != matriz.size() ) {
        return false; // Se o grafo é dirigido e não percorre todos os vertices durante a DFS, o grafo é não conexo
    }
    vector<vector<int>> matriz_t = transpor(matriz);
    visitados = busca_profundidade(matriz_t, 0);
    if ( visitados.size() != matriz_t.size() ) {
        return false; // Se o grafo é dirigido e não percorre todos os vertices durante a DFS, o grafo é não conexo
    }

    
    return true;
}

vector<vector<int>> fecho_transitivo_distancia( const vector<vector<int>> &matriz ) {
    int tam = matriz.size();
    vector<vector<int>> matriz_ftd(tam, vector<int>(tam, -1));

    for (int i = 0; i < tam; ++i) {
        for (int j = 0; j < tam; ++j) {
            if(i==j){
                matriz_ftd[i][j] = 0;
            } else if ( matriz [i][j] == 1 ) {
                matriz_ftd[i][j] = 1;
            }
        }
    }

    for (int k = 0; k < tam; ++k) {
        for (int i = 0; i < tam; ++i) {
            for (int j = 0; j < tam; ++j) {
                if (matriz_ftd[i][k] != -1 && matriz_ftd[k][j] != -1) {
                    int nova_dist = matriz_ftd[i][k] + matriz_ftd[k][j];
                    if (matriz_ftd[i][j] == -1 || nova_dist < matriz_ftd[i][j]) {
                        matriz_ftd[i][j] = nova_dist;
                    }
                }
            }
        }
    }

    return matriz_ftd;
}

vector<vector<int>> subgrafos_forte ( const Grafo &grafo ) {
    vector<vector<int>> matriz_ftd = fecho_transitivo_distancia(grafo.matriz);
    vector<vector<int>> subgrafos;

    int vertices_em_subgrafos = 0;
    int ciclo = 0;

    while ( vertices_em_subgrafos < grafo.numVertices ) {
        subgrafos.push_back(vector<int>());

        for (int j = 0; j < matriz_ftd.size(); j++) {
            if ( matriz_ftd[ciclo][j] != -1 && matriz_ftd[j][ciclo]!= -1 && count(subgrafos[ciclo].begin(), subgrafos[ciclo].end(), j) == 0 ) {
                subgrafos[ciclo].push_back(j);
            }
        }

        vertices_em_subgrafos += subgrafos[ciclo].size();
        // vertices_em_subgrafos = 0;
        // for (size_t i = 0; i < subgrafos.size(); i++) {
        //     vertices_em_subgrafos += subgrafos[ciclo].size();
        // }

        ciclo++;
    }
    
    return subgrafos;
}

void imprimir_matriz ( vector<vector<int>> matriz )  {
    int tam = matriz.size();
    cout << "Matriz " << tam << "x" << tam << ":" << endl;
    cout << "  ";
    for (int i = 0; i < tam+1; ++i) {
        cout << setw(4) << i;
    }
    cout << endl;
    for (int i = 0; i < tam; ++i) {
        cout << setw(4) << i+1 <<"->";
        for (int j = 0; j < tam; ++j) {
            if (matriz[i][j] == -1)
                cout << setw(4) << ".";
            else
                cout << setw(4) << matriz[i][j];
        }
        cout << endl;
    }
}

void adicionarArestas ( Grafo &grafo ) {
    cout << "Digite pares de vertices (origem destino) entre 1 e " << grafo.numVertices << ".\n";
    cout << "Digite um valor invalido para encerrar.\n";

    while (true) {
        int origem, destino;

        cout << "Aresta (origem destino): ";
        if (!(cin >> origem >> destino)) {
            cout << "Entrada invalida detectada. Encerrando...\n";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        if (origem < 1 || origem > grafo.numVertices || destino < 1 || destino > grafo.numVertices) {
            cout << "Vertice fora do intervalo permitido. Encerrando...\n";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        grafo.adicionarAresta(origem-1, destino-1);
    }
}

int main() {
    char dirigidoResposta;
    bool dirigido;
    int numVertices;
    vector<vector<int>> sub;
    vector<vector<int>> matriz_ft;

    cout << "Digite numero de vertices:\n";
    cin >> numVertices;

    if( cin.fail() || numVertices <= 0 ) {
        cout << "Erro. Digite um número valido.\n";
        return 0;
    }

    cout << "O arco será Dirigido?(s ou S -> sim, qualquer outro caractere -> nao):\n";
    cin >> dirigidoResposta;

    if( dirigidoResposta == 83 || dirigidoResposta == 115 ) {
        dirigido = true;
    } else {
        dirigido = false;
    }

    Grafo grafo(numVertices, dirigido); // Iniciamos o grafo com o número de vertices especificado, e a matriz zerada(pelo construtor)

    cout << "Digite pares de vertices (origem destino) entre 1 e " << numVertices << ".\n";
    cout << "Digite um valor invalido para encerrar.\n";

    while (true) {
        int origem, destino;

        cout << "Aresta (origem destino): ";
        if (!(cin >> origem >> destino)) {
            cout << "Entrada invalida detectada. Encerrando...\n";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        if (origem < 1 || origem > numVertices || destino < 1 || destino > numVertices) {
            cout << "Vertice fora do intervalo permitido. Encerrando...\n";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        grafo.adicionarAresta(origem-1, destino-1);
    }

    int menu=1;
    system("clear");

    while(true){
        cout << "------------------------------------------------------------------------------------------------------\n";
        cout << "Escolha uma opcao de interacao com o grafo:\n\n";
        cout << "0. limpar o console\n";
        cout << "1. mostrar a matriz de adjacencia\n";
        cout << "2. percorrer e printar os vertices em ordem de visitado\n";
        cout << "3. Imprimir Matriz FTD completa\n";
        cout << "4. Imprimir Matriz FTD/FTI Vertice\n";
        cout << "5. Verificar conectividade(Ainda nao retorna subgrafos)\n";
        cout << "6. Adicionar Vertice\n";
        cout << "7. Remover Vertice\n";
        cout << "8. Adicionar Arestas\n";
        cout << "99. sair\n";
        cout << "------------------------------------------------------------------------------------------------------\n";
        if (!(cin >> menu)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            menu=-1;
        }
        switch(menu){
        case 0:
            system("clear");
            break;
        case 1:
            grafo.imprimirMatriz();
            break;
        case 2:
            percorrer(grafo.matriz);
            break;
        case 3:
            imprimir_matriz( fecho_transitivo_distancia(grafo.matriz) );
            break;
        case 4:
            cout << "Digite um Vertice entre 1 e " << grafo.numVertices << ".\n";
            cout << "Digite um valor invalido para voltar.\n";

            int vertice_ft;

            cout << "Vertice: ";
            if (!( cin >> vertice_ft )) {
                cout << "Entrada invalida detectada.\n";
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }

            if ( vertice_ft < 1 || vertice_ft > grafo.numVertices ) {
                cout << "Vertice fora do intervalo permitido.\n";
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }

            matriz_ft = fecho_transitivo_distancia(grafo.matriz);

            char diretoResposta;

            cout << "Você quer um FTD?(s ou S -> sim, qualquer outro caractere -> nao, FTI):\n";
            cin >> diretoResposta;

            if( diretoResposta == 83 || diretoResposta == 115 ) {
                for (int i=0; i<matriz_ft.size(); i++) {
                    cout << matriz_ft[vertice_ft-1][i] << " ";
                }
                cout << endl;
            } else {
                for (int i=0; i<matriz_ft.size(); i++) {
                    cout << matriz_ft[i][vertice_ft-1] << " ";
                }
                cout << endl;
            }

            break;
        case 5:
            if( verifica_conectividade(grafo.matriz, grafo.dirigido) )
                cout << "Conexo\n";
            else
                cout << "Não Conexo\n";
                sub = subgrafos_forte(grafo);
                for (int i=0; i<sub.size(); i++) {
                    cout << "{ ";
                    for (int j=0; j<sub[i].size(); j++) {
                        cout << sub[i][j]+1;
                        if(j<sub[i].size()-1)cout << ", ";
                    }
                    cout << " }\n";
                }
            break;
        case 6:
            grafo.adicionarVertice();
            break;
        case 7:
            cout << "Digite um Vertice entre 1 e " << grafo.numVertices << ".\n";
            cout << "Digite um valor invalido para voltar.\n";

            int vertice;

            cout << "Vertice: ";
            if (!( cin >> vertice )) {
                cout << "Entrada invalida detectada.\n";
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }

            if ( vertice < 1 || vertice > grafo.numVertices ) {
                cout << "Vertice fora do intervalo permitido.\n";
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            grafo.removerVertice(vertice);
            break;
        case 8:
            adicionarArestas(grafo);
            break;
        case 99:
            cout << "Saindo.\n\n\n";
            return 0;
        default:
            cout << "entrada invalida\n\n\n\n";
        }
    }

}