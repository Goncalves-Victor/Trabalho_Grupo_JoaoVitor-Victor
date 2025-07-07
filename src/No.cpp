#include "No.h"

using namespace std;

No::No() {
    id = ' ';           // Inicializa com caractere em branco
    peso = 0;           // Peso padrão
    arestas = {};       // Vetor vazio de arestas
}

No::~No() {
    for (Aresta* aresta : arestas) {
        delete aresta;
    }
    arestas.clear();
}
