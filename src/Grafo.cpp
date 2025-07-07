#include "Grafo.h"
#include <unordered_set>
#include <stack>
#include <fstream>
#include <sstream>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>  
#include <unordered_map>
#include <tuple>

char buscar(unordered_map<char, char>& pai, char x) {
    if (pai[x] != x)
        pai[x] = buscar(pai, pai[x]);
    return pai[x];
}

void unir(unordered_map<char, char>& pai, char a, char b) {
    pai[buscar(pai, a)] = buscar(pai, b);
}


Grafo::Grafo() {
    ordem = 0;
    in_direcionado = false;
    in_ponderado_aresta = false;
    in_ponderado_vertice = false;
    lista_adj = {};     // Inicializa a lista de adjacência como vazia
}

Grafo::~Grafo() {
    for (No* no : lista_adj) {
        delete no;
    }
    lista_adj.clear();
}

vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    unordered_set<char> visitados;
    stack<No*> pilha;

    // Encontra o nó de origem
    No* origem = nullptr;
    for (No* no : lista_adj) {
        if (no->id == id_no) {
            origem = no;
            break;
        }
    }

    if (!origem) {
        cout << "Nó com id '" << id_no << "' não encontrado." << endl;
        return {};
    }

    pilha.push(origem);
    visitados.insert(origem->id);

    while (!pilha.empty()) {
        No* atual = pilha.top();
        pilha.pop();

        for (Aresta* aresta : atual->arestas) {
            char id_alvo = aresta->id_no_alvo;
            if (visitados.find(id_alvo) == visitados.end()) {
                visitados.insert(id_alvo);
                // Encontra o nó alvo na lista de adjacência
                for (No* no : lista_adj) {
                    if (no->id == id_alvo) {
                        pilha.push(no);
                        break;
                    }
                }
            }
        }
    }

    // Converter unordered_set para vector<char>, removendo o nó de origem
    vector<char> resultado;
    for (char c : visitados) {
        if (c != id_no) {
            resultado.push_back(c);
        }
    }

    return resultado;
}

void Grafo::imprimir_vetor(const vector<char>& vetor) {
    for (size_t i = 0; i < vetor.size(); ++i) {
        cout << vetor[i];
        if (i != vetor.size() - 1)
            cout << ",";
    }
    cout << endl << endl;
}

void Grafo::salvar_vetor_em_arquivo(const vector<char>& vetor, const string& nome_arquivo) {
    ofstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo " << nome_arquivo << endl;
        return;
    }

    for (size_t i = 0; i < vetor.size(); ++i) {
        arquivo << vetor[i];
        if (i != vetor.size() - 1)
            arquivo << ",";
    }
    arquivo << endl;

    arquivo.close();
    cout << "Resultado salvo em " << nome_arquivo << endl << endl;
}


void Grafo::ler_arquivo(string nome_arquivo) {
    ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo: " << nome_arquivo << endl;
        return;
    }

    int flag_dir, flag_arestas, flag_vertices;
    arquivo >> flag_dir >> flag_arestas >> flag_vertices;

    in_direcionado = flag_dir == 1;
    in_ponderado_aresta = flag_arestas == 1;
    in_ponderado_vertice = flag_vertices == 1;

    arquivo >> ordem;

    for (int i = 0; i < ordem; ++i) {
        char id;
        arquivo >> id;
        No* no = new No();
        no->id = id;
        lista_adj.push_back(no);
    }

    string linha;
    getline(arquivo, linha); // consome '\n' depois dos vértices

    while (getline(arquivo, linha)) {
        istringstream iss(linha);
        char origem, destino;
        int peso = 1;

        if (!(iss >> origem >> destino)) continue;
        if (in_ponderado_aresta) iss >> peso;

        No* no_origem = nullptr;
        for (No* no : lista_adj) {
            if (no->id == origem) {
                no_origem = no;
                break;
            }
        }


        if (!no_origem) continue;

        Aresta* aresta = new Aresta();
        aresta->id_no_alvo = destino;
        aresta->peso = peso;
        no_origem->arestas.push_back(aresta);

        if (!in_direcionado) {
            for (No* no : lista_adj) {
                if (no->id == destino) {
                    Aresta* aresta_rev = new Aresta();
                    aresta_rev->id_no_alvo = origem;
                    aresta_rev->peso = peso;
                    no->arestas.push_back(aresta_rev);
                    break;
                }
            }
        }

    
    }

    cout << "DEBUG: leitura finalizada com " << lista_adj.size() << " nós." << endl;
    arquivo.close();
    cout << "Grafo carregado com sucesso a partir de " << nome_arquivo << endl;
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no) {
    unordered_set<char> visitados;
    stack<No*> pilha;

    // Encontra o nó de destino
    No* destino = nullptr;
    for (No* no : lista_adj) {
        if (no->id == id_no) {
            destino = no;
            break;
        }
    }

    if (!destino) {
        cout << "Nó com id '" << id_no << "' não encontrado." << endl;
        return {};
    }

    // Para cada nó, faz uma DFS a partir dele e vê se alcança o destino
    for (No* origem : lista_adj) {
        if (origem->id == id_no) continue;

        unordered_set<char> alcancaveis;
        stack<No*> p;
        p.push(origem);
        alcancaveis.insert(origem->id);

        while (!p.empty()) {
            No* atual = p.top();
            p.pop();

            for (Aresta* aresta : atual->arestas) {
                char id_alvo = aresta->id_no_alvo;
                if (alcancaveis.find(id_alvo) == alcancaveis.end()) {
                    alcancaveis.insert(id_alvo);
                    for (No* no : lista_adj) {
                        if (no->id == id_alvo) {
                            p.push(no);
                            break;
                        }
                    }
                }
            }
        }

        // Se o destino estiver nos alcançáveis, adiciona origem ao resultado
        if (alcancaveis.find(id_no) != alcancaveis.end()) {
            visitados.insert(origem->id);
        }
    }

    vector<char> resultado(visitados.begin(), visitados.end());
    return resultado;
}

vector<char> Grafo::caminho_minimo_dijkstra(char origem_id, char destino_id) {
    map<char, int> dist;
    map<char, char> anterior;
    unordered_set<char> visitados;

    for (No* no : lista_adj) {
        dist[no->id] = numeric_limits<int>::max();
    }

    dist[origem_id] = 0;
    priority_queue<pair<int, char>, vector<pair<int, char>>, greater<>> fila;
    fila.push({0, origem_id});

    while (!fila.empty()) {
        char atual_id = fila.top().second;
        fila.pop();

        if (visitados.count(atual_id)) continue;
        visitados.insert(atual_id);

        No* atual = nullptr;
        for (No* no : lista_adj) {
            if (no->id == atual_id) {
                atual = no;
                break;
            }
        }

        if (!atual) continue;

        for (Aresta* aresta : atual->arestas) {
            char vizinho = aresta->id_no_alvo;
            int peso = aresta->peso;
            int nova_dist = dist[atual_id] + peso;

            if (nova_dist < dist[vizinho]) {
                dist[vizinho] = nova_dist;
                anterior[vizinho] = atual_id;
                fila.push({nova_dist, vizinho});
            }
        }
    }

    // Reconstrói caminho
    vector<char> caminho;
    if (dist[destino_id] == numeric_limits<int>::max()) {
        cout << "Não há caminho entre " << origem_id << " e " << destino_id << "." << endl;
        return {};
    }

    for (char at = destino_id; at != origem_id; at = anterior[at]) {
        caminho.push_back(at);
    }
    caminho.push_back(origem_id);
    reverse(caminho.begin(), caminho.end());
    return caminho;
}

vector<char> Grafo::caminho_minimo_floyd(char origem, char destino) {
    const int INF = 1e9;
    map<char, map<char, int>> dist;
    map<char, map<char, char>> pred;

    // Inicialização das distâncias e predecessores
    for (No* u : lista_adj) {
        for (No* v : lista_adj) {
            if (u->id == v->id)
                dist[u->id][v->id] = 0;
            else
                dist[u->id][v->id] = INF;
        }

        for (Aresta* aresta : u->arestas) {
            dist[u->id][aresta->id_no_alvo] = aresta->peso;
            pred[u->id][aresta->id_no_alvo] = u->id;
        }
    }

    // Algoritmo de Floyd-Warshall
    for (No* k : lista_adj) {
        for (No* i : lista_adj) {
            for (No* j : lista_adj) {
                if (dist[i->id][k->id] + dist[k->id][j->id] < dist[i->id][j->id]) {
                    dist[i->id][j->id] = dist[i->id][k->id] + dist[k->id][j->id];
                    pred[i->id][j->id] = pred[k->id][j->id];
                }
            }
        }
    }

    // Reconstruir caminho
    vector<char> caminho;
    if (dist[origem][destino] == INF) {
        cout << "Não há caminho entre " << origem << " e " << destino << "." << endl;
        return {};
    }

    char atual = destino;
    while (atual != origem) {
        caminho.push_back(atual);
        if (pred[origem].find(atual) == pred[origem].end()) {
            cout << "Erro ao reconstruir caminho." << endl;
            return {};
        }
        atual = pred[origem][atual];
    }
    caminho.push_back(origem);
    reverse(caminho.begin(), caminho.end());
    return caminho;
}

Grafo* Grafo::arvore_geradora_minima_prim(vector<char> ids) {
    const int INF = 1e9;

    unordered_set<char> conjunto_ids(ids.begin(), ids.end());
    unordered_map<char, No*> mapa_nos;

    // Cria novo grafo AGM
    Grafo* agm = new Grafo();
    agm->in_direcionado = false;
    agm->in_ponderado_aresta = true;
    agm->ordem = ids.size();

    // Adiciona nós ao novo grafo
    for (char id : ids) {
        No* no = new No();
        no->id = id;
        agm->lista_adj.push_back(no);
        mapa_nos[id] = no;
    }

    // Algoritmo de Prim
    unordered_set<char> visitados;
    auto compare = [](Aresta* a, Aresta* b) {
        return a->peso > b->peso;
    };
    priority_queue<Aresta*, vector<Aresta*>, decltype(compare)> fila(compare);

    // Começa por qualquer nó do subconjunto
    char inicial = ids[0];
    visitados.insert(inicial);

    // Adiciona arestas iniciais
    for (No* no : lista_adj) {
        if (no->id == inicial) {
            for (Aresta* aresta : no->arestas) {
                if (conjunto_ids.count(aresta->id_no_alvo)) {
                    Aresta* copia = new Aresta();
                    copia->id_no_alvo = aresta->id_no_alvo;
                    copia->peso = aresta->peso;
                    fila.push(copia);
                }
            }
            break;
        }
    }

    while (!fila.empty()) {
        Aresta* menor = fila.top();
        fila.pop();

        char u = '\0';  // origem
        char v = menor->id_no_alvo;

        // encontra origem (já visitado) que aponta para v
        for (No* no : lista_adj) {
            if (visitados.count(no->id) == 0) continue;
            for (Aresta* aresta : no->arestas) {
                if (aresta->id_no_alvo == v && conjunto_ids.count(v)) {
                    u = no->id;
                    menor->peso = aresta->peso;
                    break;
                }
            }
            if (u != '\0') break;
        }

        if (!visitados.count(v)) {
            visitados.insert(v);

            // adiciona aresta ao novo grafo
            Aresta* a1 = new Aresta();
            a1->id_no_alvo = v;
            a1->peso = menor->peso;
            mapa_nos[u]->arestas.push_back(a1);

            Aresta* a2 = new Aresta();
            a2->id_no_alvo = u;
            a2->peso = menor->peso;
            mapa_nos[v]->arestas.push_back(a2);

            // insere novas arestas de v
            for (No* no : lista_adj) {
                if (no->id == v) {
                    for (Aresta* aresta : no->arestas) {
                        if (conjunto_ids.count(aresta->id_no_alvo) && !visitados.count(aresta->id_no_alvo)) {
                            Aresta* nova = new Aresta();
                            nova->id_no_alvo = aresta->id_no_alvo;
                            nova->peso = aresta->peso;
                            fila.push(nova);
                        }
                    }
                    break;
                }
            }
        }
    }

    return agm;
}

void Grafo::imprimir_arestas() {
    unordered_set<string> exibidas;

    cout << "Arestas da AGM:" << endl;

    for (No* no : lista_adj) {
        for (Aresta* aresta : no->arestas) {
            string par1 = string(1, no->id) + "-" + string(1, aresta->id_no_alvo);
            string par2 = string(1, aresta->id_no_alvo) + "-" + string(1, no->id);

            // evita imprimir duplicadas
            if (exibidas.count(par1) == 0 && exibidas.count(par2) == 0) {
                cout << no->id << " -- " << aresta->id_no_alvo
                     << " (peso: " << aresta->peso << ")" << endl;
                exibidas.insert(par1);
                exibidas.insert(par2);
            }
        }
    }

    cout << endl;
}


Grafo* Grafo::arvore_geradora_minima_kruskal(vector<char> ids) {
    unordered_set<char> conjunto_ids(ids.begin(), ids.end());
    unordered_map<char, No*> mapa_nos;
    vector<tuple<int, char, char>> arestas;

    // Novo grafo AGM
    Grafo* agm = new Grafo();
    agm->in_direcionado = false;
    agm->in_ponderado_aresta = true;
    agm->ordem = ids.size();

    // Adiciona vértices ao novo grafo
    for (char id : ids) {
        No* no = new No();
        no->id = id;
        agm->lista_adj.push_back(no);
        mapa_nos[id] = no;
    }

    // Coleta arestas entre os vértices do subconjunto
    for (No* no : lista_adj) {
        if (!conjunto_ids.count(no->id)) continue;
        for (Aresta* aresta : no->arestas) {
            char u = no->id;
            char v = aresta->id_no_alvo;
            int peso = aresta->peso;

            if (conjunto_ids.count(v) && u < v) {
                arestas.push_back({peso, u, v});
            }
        }
    }

    // Ordena as arestas por peso crescente
    sort(arestas.begin(), arestas.end());

    // Inicializa Union-Find
    unordered_map<char, char> pai;
    for (char id : ids)
        pai[id] = id;

    // Kruskal: adiciona arestas sem formar ciclos
    for (auto [peso, u, v] : arestas) {
        if (buscar(pai, u) != buscar(pai, v)) {
            unir(pai, u, v);

            Aresta* a1 = new Aresta();
            a1->id_no_alvo = v;
            a1->peso = peso;
            mapa_nos[u]->arestas.push_back(a1);

            Aresta* a2 = new Aresta();
            a2->id_no_alvo = u;
            a2->peso = peso;
            mapa_nos[v]->arestas.push_back(a2);
        }
    }

    return agm;
}



Grafo* Grafo::arvore_caminhamento_profundidade(char id_no) {
    unordered_map<char, No*> mapa_nos;
    unordered_set<char> visitados;
    Grafo* arvore = new Grafo();

    arvore->in_direcionado = in_direcionado;
    arvore->in_ponderado_aresta = in_ponderado_aresta;
    arvore->in_ponderado_vertice = in_ponderado_vertice;

    // Adiciona todos os nós no mapa da árvore
    for (No* no : lista_adj) {
        No* novo = new No();
        novo->id = no->id;
        arvore->lista_adj.push_back(novo);
        mapa_nos[no->id] = novo;
    }

    // DFS com pilha
    stack<char> pilha;
    pilha.push(id_no);
    visitados.insert(id_no);

    while (!pilha.empty()) {
        char atual_id = pilha.top();
        pilha.pop();

        No* atual = nullptr;
        for (No* no : lista_adj) {
            if (no->id == atual_id) {
                atual = no;
                break;
            }
        }

        if (!atual) continue;

        for (Aresta* aresta : atual->arestas) {
            char vizinho = aresta->id_no_alvo;
            if (!visitados.count(vizinho)) {
                visitados.insert(vizinho);
                pilha.push(vizinho);

                // adiciona aresta à árvore
                Aresta* a1 = new Aresta();
                a1->id_no_alvo = vizinho;
                a1->peso = aresta->peso;
                mapa_nos[atual_id]->arestas.push_back(a1);

                if (!in_direcionado) {
                    Aresta* a2 = new Aresta();
                    a2->id_no_alvo = atual_id;
                    a2->peso = aresta->peso;
                    mapa_nos[vizinho]->arestas.push_back(a2);
                }
            }
        }
    }

    arvore->ordem = visitados.size();
    return arvore;
}

int calcular_excentricidade(Grafo* grafo, char origem) {
    const int INF = 1e9;
    unordered_map<char, int> dist;

    for (No* no : grafo->lista_adj) {
        dist[no->id] = INF;
    }

    dist[origem] = 0;
    priority_queue<pair<int, char>, vector<pair<int, char>>, greater<>> fila;
    fila.push({0, origem});

    while (!fila.empty()) {
        auto [d, atual_id] = fila.top();
        fila.pop();

        No* atual = nullptr;
        for (No* no : grafo->lista_adj) {
            if (no->id == atual_id) {
                atual = no;
                break;
            }
        }

        if (!atual) continue;

        for (Aresta* aresta : atual->arestas) {
            int nova_dist = dist[atual_id] + aresta->peso;
            if (nova_dist < dist[aresta->id_no_alvo]) {
                dist[aresta->id_no_alvo] = nova_dist;
                fila.push({nova_dist, aresta->id_no_alvo});
            }
        }
    }

    int excentricidade = 0;
    for (auto [id, d] : dist) {
        if (d != INF && d > excentricidade)
            excentricidade = d;
    }

    return excentricidade;
}

int Grafo::raio() {
    int min_ex = 1e9;
    for (No* no : lista_adj) {
        int ex = calcular_excentricidade(this, no->id);
        if (ex < min_ex) min_ex = ex;
    }
    return min_ex;
}

int Grafo::diametro() {
    int max_ex = 0;
    for (No* no : lista_adj) {
        int ex = calcular_excentricidade(this, no->id);
        if (ex > max_ex) max_ex = ex;
    }
    return max_ex;
}

vector<char> Grafo::centro() {
    int r = raio();
    vector<char> resultado;
    for (No* no : lista_adj) {
        if (calcular_excentricidade(this, no->id) == r)
            resultado.push_back(no->id);
    }
    return resultado;
}

vector<char> Grafo::periferia() {
    int d = diametro();
    vector<char> resultado;
    for (No* no : lista_adj) {
        if (calcular_excentricidade(this, no->id) == d)
            resultado.push_back(no->id);
    }
    return resultado;
}

void Grafo::imprimir_info_raio_diametro_centro_periferia() {
    cout << raio() << endl;
    cout << diametro() << endl;

    vector<char> c = centro();
    for (size_t i = 0; i < c.size(); ++i) {
        cout << c[i];
        if (i != c.size() - 1) cout << ",";
    }
    cout << endl;

    vector<char> p = periferia();
    for (size_t i = 0; i < p.size(); ++i) {
        cout << p[i];
        if (i != p.size() - 1) cout << ",";
    }
    cout << endl;
}

void Grafo::imprimir_lista_adjacencia() {
    unordered_map<char, vector<char>> adj;
    for (No* no : lista_adj) {
        for (Aresta* a : no->arestas) {
            adj[no->id].push_back(a->id_no_alvo);
        }
    }

    for (No* no : lista_adj) {
        cout << no->id << ": ";
        vector<char>& vizinhos = adj[no->id];
        for (size_t i = 0; i < vizinhos.size(); ++i) {
            cout << vizinhos[i];
            if (i != vizinhos.size() - 1)
                cout << " -> ";
        }
        cout << endl;
    }

    cout << endl;
}