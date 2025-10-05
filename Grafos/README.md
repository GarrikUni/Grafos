# Visualizador de Grafos em C++

Um programa de console em C++ para manipulação e análise de grafos, com uma funcionalidade de exportação para visualização interativa em uma interface web.

## Pré-requisitos e Configuração

Este projeto requer um compilador C++ (padrão C++11 ou superior) e Python para o servidor web local.

#### 1) Garanta que os seguintes arquivos estejam na mesma pasta:
```bash
- main.cpp #(seu código fonte)

- json.hpp #(a biblioteca nlohmann/json)

- index.html #(o arquivo do visualizador web)
```

#### 2) Instale um compilador C++, como o g++ (padrão no Linux, disponível via MinGW no Windows).

#### 3) Instale o [Python](https://www.python.org/) (versão 3 recomendada).



## Utilização
O fluxo de trabalho é dividido em três partes: compilar o código, iniciar o servidor web e, finalmente, executar o programa para gerar a visualização.

#### 1) Compile o programa C++
Abra um terminal no diretório do projeto e compile o código-fonte.
```bash
#(main.cpp é o nome do programa ex: grafos1005.cpp)
g++ -o programa main.cpp
````
#### 2) Inicie o Servidor Web
Em um novo terminal (deixe-o aberto em segundo plano), inicie o servidor local.

```bash
# Requer Python 3
python -m http.server 8000
````
#### 3) Execute e Visualize

No primeiro terminal, execute o programa compilado. Siga os passos no console para criar seu grafo e, em seguida, exporte-o.

#### Executa o programa de grafos
````bash
# Executa o programa de grafos
./programa 

# Dentro do programa, siga os passos no menu:
# 1. Crie seu grafo (adicionando vértices e arestas).
# 2. Use a Opção 11 para executar a coloração.
# 3. Use a Opção 12 para exportar o grafo para 'grafo.json'.

# 4. Abra seu navegador e acesse: http://localhost:8000
# 5. Seu grafo interativo aparecerá na tela.
`````
Para atualizar a visualização após qualquer alteração, simplesmente execute a Opção 12 novamente no programa C++ e atualize a página do navegador (F5).


## Contribuições

Pull requests são bem-vindos. Para mudanças maiores, por favor, abra uma issue primeiro para discutir o que você gostaria de mudar.

Por favor, certifique-se de atualizar os testes conforme apropriado.

## Licença

[MIT](https://choosealicense.com/licenses/mit/)
