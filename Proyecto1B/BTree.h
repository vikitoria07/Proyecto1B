
#include <iostream>
#include <cstdlib>
#include "ListaMaestra.h"

using namespace std;

#define MAX 4
#define MIN 2

struct NodoBtree {
    long long val[MAX + 2];
    PtrTVotante votante[MAX + 2];
    int count;
    NodoBtree* link[MAX + 2];
};

// Crea un nodo con una sola clave.
NodoBtree* crearNodo(
    long long val,
    PtrTVotante votante,
    NodoBtree* hijoIzq = nullptr,
    NodoBtree* hijoDer = nullptr
) {
    NodoBtree* nuevo = new NodoBtree{};

    nuevo->val[1] = val;
    nuevo->votante[1] = votante;
    nuevo->count = 1;
    nuevo->link[0] = hijoIzq;
    nuevo->link[1] = hijoDer;

    return nuevo;
}

// Inserta una clave en un nodo que recibió una clave promovida.
void colocarNodo(
    long long val,
    PtrTVotante votante,
    int pos,
    NodoBtree* nodo,
    NodoBtree* hijoDer
) {
    int j = nodo->count;

    while (j > pos) {
        nodo->val[j + 1] = nodo->val[j];
        nodo->votante[j + 1] = nodo->votante[j];
        nodo->link[j + 1] = nodo->link[j];
        j--;
    }

    nodo->val[pos + 1] = val;
    nodo->votante[pos + 1] = votante;
    nodo->link[pos + 1] = hijoDer;
    nodo->count++;
}

// Divide un nodo que acaba de recibir su quinta clave.
// Promueve la clave central al padre.
void partirNodo(
    NodoBtree* nodo,
    long long& clavePromovida,
    PtrTVotante& votantePromovido,
    NodoBtree*& nuevoHijoDer
) {
    const int mediana = MIN + 1; // Posición 3

    nuevoHijoDer = new NodoBtree{};

    clavePromovida = nodo->val[mediana];
    votantePromovido = nodo->votante[mediana];

    nuevoHijoDer->count = nodo->count - mediana;

    for (int i = 1; i <= nuevoHijoDer->count; i++) {
        nuevoHijoDer->val[i] = nodo->val[mediana + i];
        nuevoHijoDer->votante[i] =
            nodo->votante[mediana + i];
    }

    for (int i = 0; i <= nuevoHijoDer->count; i++) {
        nuevoHijoDer->link[i] = nodo->link[mediana + i];
    }

    nodo->count = mediana - 1;
}

// Devuelve true cuando una clave debe ser promovida al padre.
bool insertarRec(
    NodoBtree* nodo,
    long long val,
    PtrTVotante votante,
    long long& clavePromovida,
    PtrTVotante& votantePromovido,
    NodoBtree*& nuevoHijoDer
) {
    if (nodo == nullptr) {
        clavePromovida = val;
        votantePromovido = votante;
        nuevoHijoDer = nullptr;
        return true;
    }

    int pos = nodo->count;

    while (pos >= 1 && val < nodo->val[pos]) {
        pos--;
    }

    // No insertar cédulas duplicadas.
    if (pos >= 1 && val == nodo->val[pos]) {
        return false;
    }

    long long claveHija;
    PtrTVotante votanteHijo = nullptr;
    NodoBtree* nuevoHijo = nullptr;

    bool debePromoverse = insertarRec(
        nodo->link[pos],
        val,
        votante,
        claveHija,
        votanteHijo,
        nuevoHijo
    );

    if (!debePromoverse) {
        return false;
    }

    colocarNodo(
        claveHija,
        votanteHijo,
        pos,
        nodo,
        nuevoHijo
    );

    if (nodo->count <= MAX) {
        return false;
    }

    partirNodo(
        nodo,
        clavePromovida,
        votantePromovido,
        nuevoHijoDer
    );

    return true;
}

// Inserta una cédula en el árbol y actualiza la raíz si hace falta.
void insertar(
    NodoBtree*& raiz,
    long long val,
    PtrTVotante votante
) {
    if (raiz == nullptr) {
        raiz = crearNodo(val, votante);
        return;
    }

    long long clavePromovida;
    PtrTVotante votantePromovido = nullptr;
    NodoBtree* nuevoHijoDer = nullptr;

    bool seDivide = insertarRec(
        raiz,
        val,
        votante,
        clavePromovida,
        votantePromovido,
        nuevoHijoDer
    );

    if (seDivide) {
        raiz = crearNodo(
            clavePromovida,
            votantePromovido,
            raiz,
            nuevoHijoDer
        );
    }
}

// Construye el árbol a partir de la lista maestra.
// Los registros originales no se copian.
NodoBtree* ConstruirBtree(PtrTVotante lista) {
    NodoBtree* raiz = nullptr;

    while (lista != nullptr) {
        char* fin = nullptr;
        long long cedula = strtoll(lista->cedula, &fin, 10);

        // Ignora claves vacías o no numéricas.
        if (fin != lista->cedula && *fin == '\0') {
            insertar(raiz, cedula, lista);
        }

        lista = lista->Siguiente;
    }

    return raiz;
}

// Busca una cédula. Devuelve true si la encuentra.
bool BuscarBtree(
    NodoBtree* nodo,
    long long val,
    PtrTVotante* encontrado = nullptr
) {
    if (nodo == nullptr) {
        return false;
    }

    int pos = 1;

    while (pos <= nodo->count && val > nodo->val[pos]) {
        pos++;
    }

    if (pos <= nodo->count && val == nodo->val[pos]) {
        if (encontrado != nullptr) {
            *encontrado = nodo->votante[pos];
        }
        return true;
    }

    return BuscarBtree(nodo->link[pos - 1], val, encontrado);
}

// Imprime las claves en orden ascendente.
void desplegar(NodoBtree* nodo) {
    if (nodo == nullptr) {
        return;
    }

    for (int i = 1; i <= nodo->count; i++) {
        desplegar(nodo->link[i - 1]);
        cout << nodo->val[i] << ' ';
    }

    desplegar(nodo->link[nodo->count]);
}

// Libera los nodos del árbol, no los registros de la lista maestra.
void destruirBtree(NodoBtree* nodo) {
    if (nodo == nullptr) {
        return;
    }

    for (int i = 0; i <= nodo->count; i++) {
        destruirBtree(nodo->link[i]);
    }

    delete nodo;
}
