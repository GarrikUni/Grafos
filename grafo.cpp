#include <iostream>
#include <vector>
using namespace std;

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
    vector<pair<int, int>> arestas; // Pares de arestas que serão digitados pelo usuario

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

        arestas.push_back({origem, destino});
    }

    if ( dirigido ) {
        for (const auto& aresta : arestas) {
            cout << "(" << aresta.first << ", " << aresta.second << ")\n";
            matriz[aresta.first][aresta.second] = 1;
        }
    } else {
        for (const auto& aresta : arestas) {
            cout << "(" << aresta.first << ", " << aresta.second << ")\n";
            matriz[aresta.first][aresta.second] = 1;
            matriz[aresta.second][aresta.first] = 1;
        }
    }

    cout << "Matriz " << numVertices << "x" << numVertices << ":" << endl;
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}