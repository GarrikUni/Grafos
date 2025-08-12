#include <iostream>
#include <vector>
#include <stack>
#include <bits/stdc++.h>
using namespace std;

vector<int> depth_search ( vector<vector<int>> matriz, int startXY ) {
    vector<int> visitados;
    stack<int> pilha;
    int vertice;
    
    pilha.push(startXY);
    while ( !pilha.empty() ) {
        vertice = pilha.top();
        pilha.pop();
        if( count(visitados.begin(), visitados.end(), vertice) == 0 ) {
            visitados.push_back(vertice);

            for (int i = 0; i < matriz[vertice].size(); ++i) {
                if (matriz[vertice][i] == 1) {
                    pilha.push(i);
                }
            }
        }

    }

    return visitados;
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

    cout << "O arco será Dirigido(S -> sim, qualquer outra letra -> nao):\n";
    cin >> dirigidoResposta;

    if( dirigidoResposta == 83 || dirigidoResposta == 115 ) {
        dirigido = true;
    } else {
        dirigido = false;
    }

    vector<vector<int>> matriz(numVertices, vector<int>(numVertices, 0)); // Inicia a matriz com zero em todas as posições

    cout << "Digite pares de vertices (origem destino) entre 0 e " << numVertices - 1 << ".\n";
    cout << "Digite um valor invalido para encerrar.\n";

    while (true) {
        int origem, destino;

        cout << "Aresta (origem destino): ";
        if (!(cin >> origem >> destino)) {
            cout << "Entrada invalida detectada. Encerrando...\n";
            break;
        }

        if (origem < 0 || origem >= numVertices || destino < 0 || destino >= numVertices) {
            cout << "Vertice fora do intervalo permitido. Encerrando...\n";
            break;
        }

        if ( dirigido ) {
            matriz[origem][destino] = 1;
        } else {
            matriz[origem][destino] = 1;
            matriz[destino][origem] = 1;
        }
        
    }

    cout << "Matriz " << numVertices << "x" << numVertices << ":" << endl;
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }

    vector<int> exemplo = depth_search(matriz, 0);
    for (int i = 0; i < exemplo.size(); ++i) {
        cout << exemplo[i] << endl;
    }

    return 0;
}
