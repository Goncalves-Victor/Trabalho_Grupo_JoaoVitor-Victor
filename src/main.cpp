#include <iostream>
#include "Gerenciador.h"

using namespace std;
int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Uso correto: ./execGrupoX <arquivo_entrada>" << endl;
        return 1;
    }

    Grafo* grafo = new Grafo();

    // FUNÇÃO FALTANDO: carrega o grafo do arquivo
    grafo->ler_arquivo(argv[1]);

    Gerenciador::comandos(grafo);

    delete grafo;
    return 0;
}