#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <limits>
#include <iomanip> // para setw, melhora a amostragem do console
#include <cstdlib> // Para system()
using namespace std;

vector<int> busca_profundidade ( vector<vector<int>> matriz, int comecoXY = 0 ) {
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

vector<int> busca_profundidade_completa ( vector<vector<int>> matriz, int comecoXY ) {
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

vector<int> busca_largura_completa ( vector<vector<int>> matriz, int comecoXY ) {
    vector<int> visitados;
    queue<int> fila;
    int vertice;
    
    fila.push(comecoXY);
    while ( !fila.empty() ) {
        vertice = fila.front(); // front() retorna o elemento no topo, mas não remove
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

vector<vector<int>> transpor ( vector<vector<int>> matriz ) {
    vector<vector<int>> matriz_t(matriz.size(), vector<int>(matriz.size(), 0));
    for (int i = 0; i < matriz.size(); ++i) {
        for (int j = 0; j < matriz.size(); ++j) {
            matriz_t[j][i] = matriz[i][j];
        }
    }
    return matriz_t; // retorna a matriz transposta
}

bool verifica_conectividade ( vector<vector<int>> matriz, bool dirigido ) {
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

vector<vector<int>> fecho_transitivo_distancia(vector<vector<int>> &matriz, int tamanho) {
    vector<vector<int>> matriz_ftd(tamanho, vector<int>(tamanho, -1));

    for (int i = 0; i < tamanho; ++i) {
        for (int j = 0; j < tamanho; ++j) {
            if(i==j){
                matriz_ftd[i][j] = 0;
            } else if ( matriz [i][j] == 1 ) {
                matriz_ftd[i][j] = 1;
            }
        }
    }

    for (int k = 0; k < tamanho; ++k) {
        for (int i = 0; i < tamanho; ++i) {
            for (int j = 0; j < tamanho; ++j) {
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

void percorrer(vector<vector<int>> matriz, int numVertices){
    char metodo;
    int inicio=0;
    cout << "Qual será o método para percorrer?(b ou B -> BFS, qualquer outro caractere -> DFS)\n";
    cin >> metodo;

    cout << "Qual o vertice inicial para percorrer?(entre 1 e " << numVertices << ")\n";
    cin >> inicio;

    while ( inicio <= 0 || inicio >= numVertices ) {
        cout << "Vertice inválido (válido de 1 até " << numVertices << ")\n";
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

void imprimir_matriz ( vector<vector<int>> matriz, int tam )  {
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

int main() {
    char dirigidoResposta;
    bool dirigido;
    int numVertices;

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

    vector<vector<int>> matriz(numVertices, vector<int>(numVertices, 0)); // Inicia a matriz com zero em todas as posições

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

        if ( dirigido ) {
            matriz[origem-1][destino-1] = 1;
        } else {
            matriz[origem-1][destino-1] = 1;
            matriz[destino-1][origem-1] = 1;
        }
        
    }

    cout << "Matriz " << numVertices << "x" << numVertices << ":" << endl;
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
    int menu=1;
    system("clear");

        while(true){
        cout << "------------------------------------------------------------------------------------------------------\n";
        cout << "Escolha uma opcao de interacao com o grafo:\n\n";
        cout << "0. limpar o console\n";
        cout << "1. mostrar a matriz de adjacencia\n";
        cout << "2. percorrer e printar os vertices em ordem de visitado\n";
        cout << "3. TESTE FTD\n";
        cout << "4. TESTE Conectividade\n";
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
            imprimir_matriz(matriz, matriz.size());
            break;
        case 2:
            percorrer(matriz, numVertices);
            break;
        case 3:
            imprimir_matriz(fecho_transitivo_distancia(matriz, matriz.size()), numVertices);
            break;
        case 4:
            if(verifica_conectividade(matriz, dirigido))
                cout << "Conexo\n";
            else
                cout << "Não Conexo\n";
            break;
        case 99:
            cout << "Saindo.\n\n\n";
            return 0;
        default:
            cout << "entrada invalida\n\n\n\n";
        }
    }

}
