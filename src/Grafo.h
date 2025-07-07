//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <iostream>
#include <vector>


using namespace std;
class Grafo {
public:
    Grafo();
    ~Grafo();

    vector<char> fecho_transitivo_direto(char id_no); // a
    vector<char> fecho_transitivo_indireto(char id_no); // b
    vector<char> caminho_minimo_dijkstra(char id_no_a, char id_no_b); // c
    vector<char> caminho_minimo_floyd(char id_no, char id_no_b); // d
    Grafo* arvore_geradora_minima_prim(vector<char> ids_nos); // e
    Grafo* arvore_geradora_minima_kruskal(vector<char> ids_nos); // f
    Grafo* arvore_caminhamento_profundidade(char id_no); // g
    int raio(); // h 1
    int diametro(); // h 2
    vector<char> centro(); // h 3
    vector<char> periferia(); // h 4
    void imprimir_vetor(const vector<char>& vetor);
    void salvar_vetor_em_arquivo(const vector<char>& vetor, const string& nome_arquivo);
    void ler_arquivo(string nome_arquivo);
    void imprimir_arestas();
    void imprimir_info_raio_diametro_centro_periferia();
    void imprimir_lista_adjacencia();

    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No*> lista_adj;
};



#endif //GRAFO_H
