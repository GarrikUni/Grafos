#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include <limits>
#include <iomanip> // para setw, melhora a amostragem do console
#include <cstdlib> // Para system()
#include <fstream> // << NOVO: Para manipulação de arquivos
#include "json.hpp" // << NOVO: A biblioteca para JSON


// Usando o namespace para a biblioteca JSON
using json = nlohmann::json;
using namespace std;

/*
-COLORAÇÃO
1-PASSO / 1ºVERTICE A COLORIR
    escolher o vertice com o maior numero de ligações

2-PASSO 
    escolher o vertice que, no momento, for adjacente ao maior número de cores diferentes(grau de saturação)
    REPETIR ESSE PASSO ATÉ COLORIR TODOS OS VÉRTICES

Nota:
Numerar as cores por ordem crescente (tabela de cores)
Escolhido um vértice para colorir, selecionar sempre a cor admissível com número mais baixo.

*/

struct Grafo {
    int numVertices;
    bool dirigido;
    bool geradoraMinima;
    vector<char> identificadores;
    vector<vector<int>> matriz; // matriz de adjacência
    vector<string> cores; // << GARANTA QUE ESTA LINHA EXISTA

    Grafo(int n, bool d = false) { // Construtor recebe numero de vertices, e se é dirigido
        
        numVertices = n;
        dirigido = d;
        geradoraMinima = false; // Inicia como falso por padrão
        matriz.assign(n, vector<int>(n, 0)); // inicializa com zeros
        identificadores.resize(n);
        cores.assign(n, ""); // << GARANTA QUE ESTA LINHA EXISTA NO CONSTRUTOR
        for (int i = 0; i < n; ++i) {
            cout << "Digite o identificador do vertice " << i+1 << ": ";
            cin >> identificadores[i];
        }
    }

    int indiceDoVertice (char c) const {
        for ( int i=0; i<numVertices; i++ ) {
            if( identificadores[i] == c )
                return i;
        }
        return -1;
    }

    void imprimirIdentificadores() const {
        for (int i = 0; i < identificadores.size(); ++i) {
            cout << identificadores[i] << " ";
        }
    }

    void adicionarAresta(int origem, int destino) {
        if ( origem!=destino ) {
            matriz[origem][destino] = 1;
            if (!dirigido) {
                matriz[destino][origem] = 1;
            }
        }
    }

    void adicionarAresta(int origem, int destino, int peso) {
        if ( origem!=destino ) {
            matriz[origem][destino] = peso;
            if (!dirigido) {
                matriz[destino][origem] = peso;
            }
        }
    }

    void removerAresta(int origem, int destino) {
        if ( origem!=destino ) {
            matriz[origem][destino] = 0;
            if (!dirigido) {
                matriz[destino][origem] = 0;
            }
        }
    }

    void adicionarVertice( char c ) {
        for ( int i = 0; i < numVertices; i++ ) {
            matriz[i].push_back(0);
        }
        numVertices++;
        matriz.push_back( vector<int>(numVertices, 0) );
        identificadores.push_back(c);
        cores.push_back(""); 
    }

    bool removerVertice( char c ) { // c é passado pelo usuário
        int v = indiceDoVertice(c); // pegamos o indice baseado no char passado pelo usuario
        if (v < 0 || v >= numVertices) return false;
        matriz.erase( matriz.begin()+v ); // apaga a linha do vertice na matriz
        
        // Corrige o loop para o novo número de vértices
        for ( int i = 0; i < numVertices - 1; i++ ) {
            matriz[i].erase( matriz[i].begin()+v ); // apaga a coluna equivalente do vertice de todos os outros vertices
        }
        identificadores.erase(identificadores.begin()+v);
        
        numVertices--;
        return true;
    }

    void imprimirMatriz() {
        cout << "Matriz " << numVertices << "x" << numVertices << ":\n";
        cout << "      ";
        for (int j = 0; j < numVertices; j++) cout << setw(4) << identificadores[j];
        cout << "\n";
        for (int i = 0; i < numVertices; i++) {
            cout << setw(3) << identificadores[i] << " ->";
            for (int j = 0; j < numVertices; j++) {
                cout << setw(4) << matriz[i][j];
            }
            cout << "\n";
        }
    }
};


void exportarParaJson(const Grafo& grafo) { // Crucial: const Grafo& (passado por referência)
    json json_obj; 

    cout << "\n--- [DEBUG] LENDO CORES DO OBJETO GRAFO PARA EXPORTAR ---" << endl;
    for (int i = 0; i < grafo.numVertices; ++i) {
        string idStr = string(1, grafo.identificadores[i]);
        json no_json;
        no_json["id"] = idStr;
        no_json["label"] = idStr;
        
        // MENSAGEM DE DEBUG CRÍTICA:
        cout << "[DEBUG exportar] Lendo cor '" << grafo.cores[i] << "' do indice " << i << "." << endl;

        if (!grafo.cores[i].empty()) {
            no_json["color"] = grafo.cores[i];
        }
        
        json_obj["nodes"].push_back(no_json);
    }
    cout << "---------------------------------------------------------\n" << endl;

    for (int i = 0; i < grafo.numVertices; ++i) {
        for (int j = 0; j < grafo.numVertices; ++j) {
            if (grafo.matriz[i][j] > 0) {
                if (!grafo.dirigido && i > j) { continue; }
                string origemStr = string(1, grafo.identificadores[i]);
                string destinoStr = string(1, grafo.identificadores[j]);
                string pesoStr = to_string(grafo.matriz[i][j]);
                json_obj["edges"].push_back({
                    {"from", origemStr},
                    {"to", destinoStr},
                    {"label", pesoStr}
                });
            }
        }
    }

    ofstream arquivo("grafo.json");
    if (arquivo.is_open()) {
        arquivo << json_obj.dump(4);
        arquivo.close();
        cout << "\n>>> Arquivo 'grafo.json' exportado com sucesso! <<<\n" << endl;
    } else {
        cerr << "\n>>> Erro ao abrir o arquivo 'grafo.json' para escrita. <<<\n" << endl;
    }
}


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
                if ( matriz[vertice][i] >= 1 ) {
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
                if ( matriz[vertice][i] >= 1 ) {
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
                            if ( matriz[vertice][i] >= 1 ) {
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
                if ( matriz[vertice][i] >= 1 ) {
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
                            if ( matriz[vertice][i] >= 1 ) {
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

void percorrer( const Grafo &grafo ){
    int tam = grafo.matriz.size();
    char metodo, char_inicio;
    int inicio=0;
    cout << "Qual será o método para percorrer?(b ou B -> BFS, qualquer outro caractere -> DFS)\n";
    cin >> metodo;

    cout << "Qual o vertice inicial para percorrer?( ";
    grafo.imprimirIdentificadores();
    cout << ").\n";
    cin >> char_inicio;
    inicio = grafo.indiceDoVertice(char_inicio);

    while ( inicio < 0 || inicio > tam ) {
        cout << "Vertice inválido (válidos: ";
        grafo.imprimirIdentificadores();
        cout << ").\n";
        cin >> char_inicio;
        inicio = grafo.indiceDoVertice(char_inicio);
    }
    
    cout << endl;

    if( metodo == 66 || metodo == 98 ) {
        vector<int> exemploBfs = busca_largura_completa(grafo.matriz, inicio);
        for (int i = 0; i < exemploBfs.size(); i++) {
            cout << grafo.identificadores[ exemploBfs[i] ] << endl;
        }
    } else {
        vector<int> exemploDfs = busca_profundidade_completa(grafo.matriz, inicio);
        for (int i = 0; i < exemploDfs.size(); i++) {
            cout << grafo.identificadores[ exemploDfs[i] ] << endl;
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
            } else if ( matriz [i][j] >= 1 ) {
                matriz_ftd[i][j] = matriz[i][j];
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

bool ja_esta_em_subgrafo ( vector<vector<int>> &subgrafos, int valor ) {
    for (int i = 0; i < subgrafos.size(); i++) {
        for (int j = 0; j < subgrafos[i].size(); j++) {
            if ( subgrafos[i][j] == valor ) {
                return true;
            }
        }
    }
    return false;
}

vector<vector<int>> subgrafos_forte ( const Grafo &grafo ) {
    vector<vector<int>> matriz_ftd = fecho_transitivo_distancia(grafo.matriz);
    vector<vector<int>> subgrafos;

    int vertices_em_subgrafos = 0;
    int vertice_percorrido = 0;
    int ciclo = 0;

    while ( vertices_em_subgrafos < grafo.numVertices ) {
        subgrafos.push_back(vector<int>());

        for (int j = 0; j < matriz_ftd.size(); j++) {
            if ( matriz_ftd[vertice_percorrido][j] != -1 && matriz_ftd[j][vertice_percorrido]!= -1 && !ja_esta_em_subgrafo(subgrafos, j) ) {
                subgrafos[ciclo].push_back(j);
            }
        }

        vertices_em_subgrafos += subgrafos[ciclo].size();
        // vertices_em_subgrafos = 0;
        // for (int i = 0; i < subgrafos.size(); i++) {
        //     cout << "Tamanho do Subgrafo = " << subgrafos[i].size() << endl;
        //     vertices_em_subgrafos += subgrafos[i].size();
        // }    
        for (int i=0; i<grafo.numVertices; i++){
            if( !ja_esta_em_subgrafo(subgrafos, i) ){
                vertice_percorrido = i;
                break;
            }
        }
        ciclo++;
    }
    
    return subgrafos;
}

void imprimir_matriz ( vector<vector<int>> matriz, vector<char> &identificadores )  {
    int tam = matriz.size();
    cout << "Matriz " << tam << "x" << tam << ":" << endl;
    cout << setw(6) << "";
    for (int i = 0; i < tam; ++i) {
        cout << setw(4) << identificadores[i];
    }
    cout << endl;
    for (int i = 0; i < tam; ++i) {
        cout << setw(4) << identificadores[i] <<"->";
        for (int j = 0; j < tam; ++j) {
            if (matriz[i][j] == -1)
                cout << setw(4) << ".";
            else
                cout << setw(4) << matriz[i][j];
        }
        cout << endl;
    }
}

int perguntaPeso() {
    int peso;
    while(true){
    cout << "Qual sera o peso desta aresta(o peso deve ser maior que zero)? ";
        if (!(cin >> peso)) {
            cout << "Entrada invalida detectada. Tente novamente.\n";
            cin.clear(); // limpa o erro de cin
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta a linha
        } else if (peso < 1) {
            cout << "Peso deve ser maior que zero. Tente novamente.\n";
        } else {
            return peso;// entrada válida
        }
    }
}

void adicionarArestas ( Grafo &grafo ) {
    cout << "Digite um par de vertices (Char do vertice origem e Char do vertice destino) entre os vertices existentes ( ";
    grafo.imprimirIdentificadores();
    cout << ").\n";
    cout << "Digite um valor invalido para encerrar.\n";

    while (true) {
        int origem, destino;
        char origemNome, destinoNome;

        cout << "Aresta (Char da origem Char do destino): ";
        cin >> origemNome >> destinoNome;
        origem = grafo.indiceDoVertice(origemNome);
        destino = grafo.indiceDoVertice(destinoNome);
        if (origem == -1 || destino == -1) {
            cout << "Um dos identificadores não existe. \nVoltando para o menu.\n";
            break;
        }

        // cout << "Aresta (origem destino): ";
        // if (!(cin >> origem >> destino)) {
        //     cout << "Entrada invalida detectada. Encerrando...\n";
        //     cin.clear(); 
        //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
        //     break;
        // }
        
        if (origem < 0 || origem > grafo.numVertices || destino < 0 || destino > grafo.numVertices || origem==destino) {
            cout << "Vertice fora do intervalo permitido. Encerrando...\n";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        if(!grafo.geradoraMinima){
            grafo.adicionarAresta(origem, destino);
        } else {
            grafo.adicionarAresta(origem, destino, perguntaPeso());
        }
            
    }
}

void removerArestas ( Grafo &grafo ) {
    cout << "Digite um par de vertices (Char do vertice origem e Char do vertice destino) entre os vertices existentes ( ";
        grafo.imprimirIdentificadores();
    cout << ").\n";
    cout << "Digite um valor invalido para voltar para o menu.\n";

    int origem, destino;
    char origemNome, destinoNome;

    cin >> origemNome >> destinoNome;
    origem = grafo.indiceDoVertice(origemNome);
    destino = grafo.indiceDoVertice(destinoNome);
    if (origem == -1 || destino == -1) {
        cout << "Um dos identificadores não existe.\nVoltando ao menu.\n";
        return;
    }

    // cout << "Aresta (origem destino): ";
    // if (!(cin >> origem >> destino)) {
    //     cout << "Entrada invalida detectada. Encerrando...\n";
    //     cin.clear(); 
    //     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // }

    if (origem < 0 || origem > grafo.numVertices || destino < 0 || destino > grafo.numVertices || origem==destino) {
        cout << "Vertice fora do intervalo permitido. Encerrando...\n";
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    grafo.removerAresta(origem, destino);
}

void lerMatrizCompleta(Grafo& grafo) {
    cout << "Digite a matriz de adjacencia completa, linha por linha, separando os valores por espaco.\n";
    cout << "A matriz deve ser " << grafo.numVertices << "x" << grafo.numVertices << ".\n";
    
    for (int i = 0; i < grafo.numVertices; i++) {
        cout << "Linha para o vertice " << grafo.identificadores[i] << ": ";
        for (int j = 0; j < grafo.numVertices; j++) {
            int valor;
            if (!(cin >> valor)) {
                cout << "Entrada invalida. Voltando ao menu...\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }
            if (valor < 0) {
                cout << "Valores negativos nao sao permitidos. Voltando ao menu...\n";
                return;
            }
            grafo.matriz[i][j] = valor;
        }
    }
    cout << "Matriz lida com sucesso.\n";
}

bool parExiste(const vector<pair<int, int> >& vetor, pair<int, int> alvo) {
    for (vector<pair<int, int> >::const_iterator it = vetor.begin(); it != vetor.end(); ++it) {
        if ( ( it->first == alvo.first && it->second == alvo.second ) || (it->second == alvo.first && it->first == alvo.second) ) {
            return true;
        }
    }
    return false;
}

vector<pair<int, int>> AGV ( Grafo &grafo ) {
    vector<vector<int>> matriz = grafo.matriz;
    vector<pair<int, int>> agv = {};
    vector<int> vertices_conectados;
    pair<int, int> pos_menor_custo;
    int menor_custo = numeric_limits<int>::max();

    for (int i = 0; i < grafo.numVertices; i++) {
        for (int j = 1; j < grafo.numVertices; j++) {
            if (j > i) { // Lidar somente com itens acima da diagonal principal, evita duplica
                if ( matriz[i][j] < menor_custo && matriz[i][j] > 0 ) {
                    pos_menor_custo = make_pair (i, j);
                    menor_custo = matriz[i][j];
                }
            }
        }
    }
    agv.push_back(pos_menor_custo);
    vertices_conectados.push_back(pos_menor_custo.first);
    vertices_conectados.push_back(pos_menor_custo.second);

    while ( agv.size() < grafo.numVertices-1 ) { // Faz até o numero de arestas ser numVertices-1;
        menor_custo = numeric_limits<int>::max(); // Reseta o menor custo
        for (int i = 0; i < vertices_conectados.size(); i++) {
            for (int j = 0; j < matriz.size(); j++) {
                if ( vertices_conectados[i] != j ) { // não inclui o vetor principal
                    if( matriz[vertices_conectados[i]][j] > 0 ) { // o valor da matriz deve ser maior que 0 para ter uma conexao
                        if ( matriz[vertices_conectados[i]][j] < menor_custo && !parExiste( agv, {vertices_conectados[i],j} ) ) {
                            pos_menor_custo = make_pair (vertices_conectados[i], j);
                            menor_custo = matriz[vertices_conectados[i]][j];
                        }
                    }
                }
            }
        }
        agv.push_back(pos_menor_custo);
        if ( count(vertices_conectados.begin(), vertices_conectados.end(), pos_menor_custo.first) == 0 ){
            vertices_conectados.push_back(pos_menor_custo.first);
        } else {
            vertices_conectados.push_back(pos_menor_custo.second);
        }
    }

    return agv;
}

void coloracao(Grafo &grafo) {// Função de coloração ATUALIZADA para salvar as cores no grafo

    grafo.cores.assign(grafo.numVertices, "");

    // --- Configuração das Cores ---
    vector<string> cores_portugues = {
        "Vermelho", "Azul", "Amarelo", "Verde", "Laranja", "Roxo",
        "Rosa", "Preto", "Branco", "Cinza", "Ciano", "Magenta"};

    map<string, string> mapa_cores;
    mapa_cores["Vermelho"] = "red";
    mapa_cores["Azul"] = "blue";
    mapa_cores["Amarelo"] = "yellow";
    mapa_cores["Verde"] = "green";
    mapa_cores["Laranja"] = "orange";
    mapa_cores["Roxo"] = "purple";
    mapa_cores["Rosa"] = "pink";
    mapa_cores["Preto"] = "black";
    mapa_cores["Branco"] = "white";
    mapa_cores["Cinza"] = "gray";
    mapa_cores["Ciano"] = "cyan";
    mapa_cores["Magenta"] = "magenta";

    if (cores_portugues.empty()) {
        cout << "Erro: nenhuma cor disponível." << endl;
        return;
    }

    // --- Inicialização ---
    int n = grafo.numVertices;
    if (n == 0) return; // Evita erros se o grafo estiver vazio
    
    vector<string> cores_vertices(n, "");
    vector<int> grau(n, 0);

    // --- Lógica Principal do Algoritmo de Coloração ---

    // 1. Calcula o grau de cada vértice
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && (grafo.matriz[i][j] > 0 || grafo.matriz[j][i] > 0)) {
                grau[i]++;
            }
        }
    }

    // 2. Colore o vértice de maior grau com a primeira cor
    int inicial = max_element(grau.begin(), grau.end()) - grau.begin();
    cores_vertices[inicial] = cores_portugues[0];

    // 3. Loop principal: colore os vértices restantes
    while (count(cores_vertices.begin(), cores_vertices.end(), "") > 0) {
        int melhor_indice = -1;
        int maior_saturacao = -1;
        int maior_grau_desempate = -1;

        // Encontra o próximo vértice a ser colorido (maior saturação, depois maior grau)
        for (int i = 0; i < n; i++) {
            if (cores_vertices[i] != "") {
                continue; // Já está colorido, pula
            }

            set<string> cores_vizinhos;
            for (int j = 0; j < n; j++) {
                if (grafo.matriz[i][j] > 0 || grafo.matriz[j][i] > 0) {
                    if (cores_vertices[j] != "") {
                        cores_vizinhos.insert(cores_vertices[j]);
                    }
                }
            }

            int saturacao_atual = cores_vizinhos.size();

            // Critério de desempate: maior saturação, seguido por maior grau
            if (saturacao_atual > maior_saturacao) {
                maior_saturacao = saturacao_atual;
                maior_grau_desempate = grau[i];
                melhor_indice = i;
            } else if (saturacao_atual == maior_saturacao && grau[i] > maior_grau_desempate) {
                maior_grau_desempate = grau[i];
                melhor_indice = i;
            }
        }

        if (melhor_indice == -1) {
            cout << "Erro: não foi possível encontrar o próximo vértice para colorir." << endl;
            return;
        }

        // Encontra a menor cor disponível para o vértice escolhido
        set<string> cores_proibidas;
        for (int j = 0; j < n; j++) {
            if (grafo.matriz[melhor_indice][j] > 0 || grafo.matriz[j][melhor_indice] > 0) {
                if (cores_vertices[j] != "") {
                    cores_proibidas.insert(cores_vertices[j]);
                }
            }
        }

        string cor_escolhida = "";
        for (const string &cor : cores_portugues) {
            if (cores_proibidas.find(cor) == cores_proibidas.end()) {
                cor_escolhida = cor;
                break;
            }
        }

        if (cor_escolhida == "") {
            cout << "Erro: não há cores disponíveis para o vértice " << grafo.identificadores[melhor_indice] << endl;
            return;
        }

        cores_vertices[melhor_indice] = cor_escolhida;
    }

    // --- Finalização: Salva as cores no objeto grafo e imprime no console ---
    cout << "\n--- Cores Atribuídas ---\n";
    for (int i = 0; i < n; i++) {
        string cor_pt = cores_vertices[i];
        string cor_web = "";

        if (mapa_cores.count(cor_pt)) {
            cor_web = mapa_cores[cor_pt];
        }

        grafo.cores[i] = cor_web;

        cout << grafo.identificadores[i] << ": " << cor_pt
             << " (-> " << cor_web << " para web)" << endl;
    }
    cout << "--- Fim das Cores ---\n\n";
}



int main() {
    char dirigidoResposta, formaEntrada;
    bool dirigido;
    int numVertices;
    vector<vector<int>> sub;
    vector<vector<int>> matriz_ft;
    vector<pair<int, int> > ex;

    cout << "Digite o numero de vertices:\n";
    cin >> numVertices;

    if( cin.fail() || numVertices <= 0 ) {
        cout << "Erro. Digite um numero valido.\n";
        return 0;
    }

    cout << "O grafo sera Dirigido? (s/S para sim, qualquer outro caractere para nao):\n";
    cin >> dirigidoResposta;
    dirigido = (dirigidoResposta == 'S' || dirigidoResposta == 's');

    Grafo grafo(numVertices, dirigido);
    if ( !grafo.dirigido ) {
        cout << "O grafo sera para Arvore Geradora Minima (com pesos)? (s/S para sim, qualquer outro caractere para nao):\n";
        cin >> dirigidoResposta;
        grafo.geradoraMinima = (dirigidoResposta == 'S' || dirigidoResposta == 's');
    }

    cout << "\nComo voce deseja inserir as arestas?\n";
    cout << "1. Aresta por aresta\n";
    cout << "2. Matriz de adjacencia completa\n";
    cin >> formaEntrada;

    if (formaEntrada == '1') {
        adicionarArestas(grafo);
    } else if (formaEntrada == '2') {
        lerMatrizCompleta(grafo);
    } else {
        cout << "Opcao invalida. Encerrando o programa.\n";
        return 0;
    }

    int menu=1;

    while(true){
        cout << "------------------------------------------------------------------------------------------------------\n";
        cout << "Escolha uma opcao de interacao com o grafo:\n\n";
        cout << "0. Limpar o console\n";
        cout << "1. Mostrar a matriz de adjacencia\n";
        cout << "2. Percorrer e printar os vertices em ordem de visitado\n";
        cout << "3. Imprimir Matriz FTD completa\n";
        cout << "4. Imprimir Matriz FTD/FTI Vertice\n";
        cout << "5. Verificar conectividade\n";
        cout << "6. Adicionar Vertice\n";
        cout << "7. Remover Vertice\n";
        cout << "8. Adicionar Arestas\n";
        cout << "9. Remover Arestas\n";
        cout << "10. Exibe as conexoes de AVG\n";
        cout << "11. COLORAÇÃO\n";
        cout << "12. EXPORTAR PARA JSON (Visualizacao Web)\n"; //NOVO
        cout << "99. Sair\n";
        cout << "------------------------------------------------------------------------------------------------------\n";
        if (!(cin >> menu)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            menu=-1;
        }
        switch(menu){
        case 0:
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
            break;
        case 1:
            grafo.imprimirMatriz();
            break;
        case 2:
            percorrer(grafo);
            break;
        case 3:
            imprimir_matriz( fecho_transitivo_distancia(grafo.matriz), grafo.identificadores );
            break;
        case 4:
            cout << "Digite um vertice entre os vertices existentes ( ";
            grafo.imprimirIdentificadores();
            cout << ").\n";
            cout << "Digite um valor invalido para voltar.\n";

            char char_vertice_ft;
            int vertice_ft;

            cin >> char_vertice_ft;
            vertice_ft = grafo.indiceDoVertice(char_vertice_ft);

            if ( vertice_ft == -1 ){
                cout << "Entrada invalida detectada.\n";
                break;
            }

            matriz_ft = fecho_transitivo_distancia(grafo.matriz);

            char diretoResposta;

            cout << "Você quer um FTD?(s ou S -> sim, qualquer outro caractere -> nao, FTI):\n";
            cin >> diretoResposta;

            if( diretoResposta == 83 || diretoResposta == 115 ) {
                cout << "\t";
                for (int i=0; i<matriz_ft.size(); i++) {
                    cout << setw(4) << grafo.identificadores[i];
                }
                cout << endl;
                cout << setw(4) << grafo.identificadores[vertice_ft] << " -> ";
                for (int i=0; i<matriz_ft.size(); i++) {
                    cout << setw(4) << matriz_ft[vertice_ft][i];
                }
                cout << endl;
            } else {
                cout << "\t";
                for (int i=0; i<matriz_ft.size(); i++) {
                    cout << setw(4) << grafo.identificadores[i];
                }
                cout << endl;
                cout << setw(4) << grafo.identificadores[vertice_ft] << " -> ";
                for (int i=0; i<matriz_ft.size(); i++) {
                    cout << setw(4) <<matriz_ft[i][vertice_ft];
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
                        cout << grafo.identificadores[ sub[i][j] ];
                        if(j<sub[i].size()-1)cout << ", ";
                    }
                    cout << " }\n";
                }
            break;
        case 6:
            cout << "Digite um CARACTERE DIFERENTE dos caracteres dos vertices existentes ( ";
            grafo.imprimirIdentificadores();
            cout << ").\n";
            cout << "Digite um caractere já utilizado para voltar.\n";

            char c_novo;
            cin >> c_novo;
            if ( grafo.indiceDoVertice(c_novo) == -1 )
                grafo.adicionarVertice(c_novo);
            else
                cout << "Caractere digitado já está em uso. Voltando ao menu.\n";
            break;
        case 7:
            cout << "Digite um vertice entre os vertices existentes ( ";
            grafo.imprimirIdentificadores();
            cout << ").\n";
            cout << "Digite um valor invalido para voltar.\n";

            char c;
            cin >> c;
            if( grafo.removerVertice(c) )
                cout << "Vertice " << c << " Removido.\n";
            else
                cout << "Vertice " << c << " Não encontrado, nenhum vertice foi removido.\n";
            break;
        case 8:
            adicionarArestas(grafo);
            break;
        case 9:
            removerArestas(grafo);
            break;
        case 10:
            ex = AGV(grafo);
            for (vector<pair<int, int> >::const_iterator it = ex.begin(); it != ex.end(); ++it) {
                cout << "(" << grafo.identificadores[it->first] << ", " << grafo.identificadores[it->second] << ")" << endl;
            }
            break;
        case 11:
            coloracao ( grafo );
            break;
        case 12: // NOVO
            exportarParaJson(grafo);
            break;
        case 99:
            cout << "Saindo.\n\n\n";
            return 0;
        default:
            cout << "entrada invalida\n\n\n\n";
        }
    }

}