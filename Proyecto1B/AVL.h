
#include <cstdlib>
#include "ListaMaestra.h"

// Se supone que PtrTVotante y TVotante
// ya están definidos en ListaMaestra.h.

struct NodoAVL {
    long long llave;
    PtrTVotante votante;
    NodoAVL* izq;
    NodoAVL* derecha;
    int altura;
};

int AlturaAVL(NodoAVL* nodo) {
    if (nodo == nullptr)
        return 0;

    return nodo->altura;
}

int Maximo(int a, int b) {
    return (a > b) ? a : b;
}

int BalanceAVL(NodoAVL* nodo) {
    if (nodo == nullptr)
        return 0;

    return AlturaAVL(nodo->izq) -
        AlturaAVL(nodo->derecha);
}

NodoAVL* CrearNodoAVL(
    long long llave,
    PtrTVotante votante
) {
    NodoAVL* nuevo = new NodoAVL;

    nuevo->llave = llave;
    nuevo->votante = votante;
    nuevo->izq = nullptr;
    nuevo->derecha = nullptr;
    nuevo->altura = 1;

    return nuevo;
}

NodoAVL* RotarDerechaAVL(NodoAVL* y) {
    NodoAVL* x = y->izq;
    NodoAVL* temporal = x->derecha;

    x->derecha = y;
    y->izq = temporal;

    y->altura = 1 + Maximo(
        AlturaAVL(y->izq),
        AlturaAVL(y->derecha)
    );

    x->altura = 1 + Maximo(
        AlturaAVL(x->izq),
        AlturaAVL(x->derecha)
    );

    return x;
}

NodoAVL* RotarIzquierdaAVL(NodoAVL* x) {
    NodoAVL* y = x->derecha;
    NodoAVL* temporal = y->izq;

    y->izq = x;
    x->derecha = temporal;

    x->altura = 1 + Maximo(
        AlturaAVL(x->izq),
        AlturaAVL(x->derecha)
    );

    y->altura = 1 + Maximo(
        AlturaAVL(y->izq),
        AlturaAVL(y->derecha)
    );

    return y;
}

NodoAVL* InsertarAVL(
    NodoAVL* nodo,
    long long llave,
    PtrTVotante votante
) {
    if (nodo == nullptr)
        return CrearNodoAVL(llave, votante);

    if (llave < nodo->llave) {
        nodo->izq = InsertarAVL(
            nodo->izq, llave, votante
        );
    }
    else if (llave > nodo->llave) {
        nodo->derecha = InsertarAVL(
            nodo->derecha, llave, votante
        );
    }
    else {
        // No se insertan cédulas duplicadas.
        return nodo;
    }

    nodo->altura = 1 + Maximo(
        AlturaAVL(nodo->izq),
        AlturaAVL(nodo->derecha)
    );

    int balance = BalanceAVL(nodo);

    // Caso izquierda-izquierda
    if (balance > 1 &&
        llave < nodo->izq->llave) {
        return RotarDerechaAVL(nodo);
    }

    // Caso derecha-derecha
    if (balance < -1 &&
        llave > nodo->derecha->llave) {
        return RotarIzquierdaAVL(nodo);
    }

    // Caso izquierda-derecha
    if (balance > 1 &&
        llave > nodo->izq->llave) {
        nodo->izq = RotarIzquierdaAVL(nodo->izq);
        return RotarDerechaAVL(nodo);
    }

    // Caso derecha-izquierda
    if (balance < -1 &&
        llave < nodo->derecha->llave) {
        nodo->derecha =
            RotarDerechaAVL(nodo->derecha);
        return RotarIzquierdaAVL(nodo);
    }

    return nodo;
}

// Construye el AVL recorriendo la lista maestra.
// La lista debe seguir existiendo mientras se use el árbol.
NodoAVL* ConstruirAVL(PtrTVotante lista) {
    NodoAVL* raiz = nullptr;

    while (lista != nullptr) {
        long long cedula = strtoll(
            lista->cedula, nullptr, 10
        );

        raiz = InsertarAVL(
            raiz, cedula, lista
        );

        lista = lista->Siguiente;
    }

    return raiz;
}

void PreOrdenAVL(NodoAVL* Raiz)
{
    if (Raiz != NULL)
    {
        printf("%i  ", Raiz->llave);
        PreOrdenAVL(Raiz->izq);
        PreOrdenAVL(Raiz->derecha);
    }
}

// Devuelve el votante encontrado, o nullptr.
PtrTVotante BuscarAVL(
    NodoAVL* raiz,
    long long cedula,
    long long& comparaciones
) {
    while (raiz != nullptr) {
        comparaciones++;

        if (cedula == raiz->llave)
            return raiz->votante;

        if (cedula < raiz->llave)
            raiz = raiz->izq;
        else
            raiz = raiz->derecha;
    }

    return nullptr;
}

// Destruye los nodos del AVL, no los votantes.
void DestruirAVL(NodoAVL* raiz) {
    if (raiz == nullptr)
        return;

    DestruirAVL(raiz->izq);
    DestruirAVL(raiz->derecha);

    delete raiz;
}
