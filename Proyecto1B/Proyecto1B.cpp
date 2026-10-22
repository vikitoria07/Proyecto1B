
#include <algorithm>
#include <chrono>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#include "ListaMaestra.h"
#include "AVL.h"
#include "BTree.h"
#include "ABB.h"

using namespace std;
using namespace chrono;

// Cuenta los registros de la lista maestra.
long long ContarVotantes(PtrTVotante lista) {
    long long total = 0;

    while (lista != nullptr) {
        total++;
        lista = lista->Siguiente;
    }

    return total;
}

// Busca en el ABB y cuenta los nodos examinados.
NodoBB* BuscarABBMenu(
    NodoBB* raiz,
    long long cedula,
    long long& examinados
) {
    while (raiz != nullptr) {
        examinados++;

        if (cedula == raiz->llave)
            return raiz;

        if (cedula < raiz->llave)
            raiz = raiz->izq;
        else
            raiz = raiz->derecha;
    }

    return nullptr;
}

// Busca en el B-Tree y cuenta las claves examinadas.
PtrTVotante BuscarBTreeMenu(
    NodoBtree* nodo,
    long long cedula,
    long long& examinados
) {
    if (nodo == nullptr)
        return nullptr;

    int pos = 1;

    while (pos <= nodo->count) {
        examinados++;

        if (cedula == nodo->val[pos])
            return nodo->votante[pos];

        if (cedula < nodo->val[pos])
            break;

        pos++;
    }

    return BuscarBTreeMenu(
        nodo->link[pos - 1],
        cedula,
        examinados
    );
}

// Calcula la altura del B-Tree.
int AlturaBTreeMenu(NodoBtree* nodo) {
    if (nodo == nullptr)
        return 0;

    return 1 + AlturaBTreeMenu(nodo->link[0]);
}

// Elimina espacios al final de un campo.
string LimpiarCampo(const char* campo) {
    string texto(campo);

    while (!texto.empty() && texto.back() == ' ')
        texto.pop_back();

    return texto;
}

// Muestra los datos del votante.
void MostrarVotante(PtrTVotante votante) {
    if (votante == nullptr) {
        cout << "No se encontro el votante.\n";
        return;
    }

    cout << "\n========== DATOS DEL VOTANTE ==========\n";
    cout << "Cedula:          " << votante->cedula << '\n';
    cout << "Codigo electoral:" << votante->codelec << '\n';
    cout << "Sexo:            " << votante->sexo << '\n';
    cout << "Fecha:           " << votante->fecha << '\n';
    cout << "Numero de junta: " << votante->numjun << '\n';
    cout << "Nombre:          "
        << LimpiarCampo(votante->nombre) << '\n';
    cout << "Primer apellido: "
        << LimpiarCampo(votante->papellido) << '\n';
    cout << "Segundo apellido:"
        << LimpiarCampo(votante->sapellido) << '\n';
    cout << "=======================================\n";
}

void ListarTodo(NodoBB* raizABB, NodoAVL* raizAVL, NodoBtree* raizBtree) {
    int opcion2;

    cout << fixed << setprecision(3);
    cout << "\n======= LISTAR DATOS =======\n";
    cout << "1. Listar con ABB\n";
    cout << "2. Listar con AVL\n";
    cout << "3. Lista con BTREE\n";
    cout << "------------------------------\n";
    cout << "Seleccione una opcion: ";
    cout << "------------------------------\n";

    cin >> opcion2;

    if (opcion2 == 1) {
        auto inicio = steady_clock::now();
        EnOrdenIRD(raizABB);
        auto fin = steady_clock::now();
        double tiempo = duration<double, milli>(fin - inicio).count();

        cout << "  Resultado: ";
        cout << "  Tiempo: " << tiempo << " ms\n";
        
    }
    else if (opcion2 == 2) {
        auto inicio = steady_clock::now();
        PreOrdenAVL(raizAVL);
        auto fin = steady_clock::now();
        double tiempo = duration<double, milli>(fin - inicio).count();

        cout << "  Resultado: ";
        cout << "  Tiempo: " << tiempo << " ms\n";
    }
    else if (opcion2 == 3) {
        auto inicio = steady_clock::now();
        desplegar(raizBtree);
        auto fin = steady_clock::now();
        double tiempo = duration<double, milli>(fin - inicio).count();

        cout << "  Resultado: ";
        cout << "  Tiempo: " << tiempo << " ms\n";
    }
}
// Compara las tres estructuras usando la misma cedula.
void ConsultarCedula(
    NodoBB* raizABB,
    NodoAVL* raizAVL,
    NodoBtree* raizBtree
) {
    string entrada;

    cout << "\nIngrese la cedula: ";
    cin >> entrada;

    bool numerica = !entrada.empty() &&
        all_of(entrada.begin(), entrada.end(),
            [](unsigned char c) {
                return isdigit(c);
            });

    if (!numerica) {
        cout << "Error: la cedula debe contener solo digitos.\n";
        return;
    }

    long long cedula;

    try {
        cedula = stoll(entrada);
    }
    catch (...) {
        cout << "La cedula ingresada no es valida.\n";
        return;
    }

    // ABB
    long long examinadosABB = 0;
    auto inicioABB = steady_clock::now();

    NodoBB* resultadoABB = BuscarABBMenu(
        raizABB, cedula, examinadosABB
    );

    auto finABB = steady_clock::now();

    double tiempoABB =
        duration<double, milli>(finABB - inicioABB).count();

    // AVL
    long long examinadosAVL = 0;
    auto inicioAVL = steady_clock::now();

    PtrTVotante resultadoAVL = BuscarAVL(
        raizAVL, cedula, examinadosAVL
    );

    auto finAVL = steady_clock::now();

    double tiempoAVL =
        duration<double, milli>(finAVL - inicioAVL).count();

    // B-Tree
    long long examinadosBTree = 0;
    auto inicioBTree = steady_clock::now();

    PtrTVotante resultadoBTree = BuscarBTreeMenu(
        raizBtree, cedula, examinadosBTree
    );

    auto finBTree = steady_clock::now();

    double tiempoBTree =
        duration<double, milli>(finBTree - inicioBTree).count();

    cout << fixed << setprecision(4);
    cout << "\n========== COMPARACION DE BUSQUEDA ==========\n";

    cout << "ABB:\n";
    cout << "  Resultado: "
        << (resultadoABB ? "Encontrado" : "No encontrado") << '\n';
    cout << "  Nodos examinados: " << examinadosABB << '\n';
    cout << "  Tiempo: " << tiempoABB << " ms\n";

    cout << "AVL:\n";
    cout << "  Resultado: "
        << (resultadoAVL ? "Encontrado" : "No encontrado") << '\n';
    cout << "  Nodos examinados: " << examinadosAVL << '\n';
    cout << "  Tiempo: " << tiempoAVL << " ms\n";

    cout << "B-Tree:\n";
    cout << "  Resultado: "
        << (resultadoBTree ? "Encontrado" : "No encontrado") << '\n';
    cout << "  Claves examinadas: " << examinadosBTree << '\n';
    cout << "  Tiempo: " << tiempoBTree << " ms\n";

    cout << "=============================================\n";

    // Muestra los datos si las estructuras encontraron el registro.
    if (resultadoAVL != nullptr) {
        MostrarVotante(resultadoAVL);
    }
    else if (resultadoABB != nullptr) {
        MostrarVotante(resultadoABB->votante);
    }
    else if (resultadoBTree != nullptr) {
        MostrarVotante(resultadoBTree);
    }
    else {
        cout << "\nNo existe un registro con esa cedula.\n";
    }

    // Verifica que las estructuras coincidan.
    bool coinciden =
        (resultadoABB != nullptr) == (resultadoAVL != nullptr) &&
        (resultadoAVL != nullptr) == (resultadoBTree != nullptr);

    if (!coinciden) {
        cout << "\nADVERTENCIA: las estructuras no coinciden "
            << "en el resultado de la busqueda.\n";
    }
}

void MostrarMenu() {
    cout << "\n";
    cout << "=============================================\n";
    cout << "       SISTEMA DE CONSULTA ELECTORAL\n";
    cout << "             PADRON DE COSTA RICA\n";
    cout << "=============================================\n";
    cout << "1. Consultar cedula en las tres estructuras\n";
    cout << "2. Ver estadisticas del padron\n";
    cout << "3. Ver tiempos de construccion\n";
    cout << "4. Listar padrón\n";
    cout << "0. Salir\n";
    cout << "---------------------------------------------\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    PtrTVotante ListaV = nullptr;

    cout << "Por favor espere mientras se carga el padron...\n";

    auto inicioCarga = steady_clock::now();
    inicializarvotantes(ListaV);
    CargarVotantes(ListaV);
    auto finCarga = steady_clock::now();

    if (ListaV == nullptr) {
        cout << "\nNo se pudo cargar el padron o la lista esta vacia.\n";
        cout << "Revise el archivo PADRON_COMPLETO--.txt.\n";
        return 1;
    }

    long long totalVotantes = ContarVotantes(ListaV);

    double tiempoCarga =
        duration<double>(finCarga - inicioCarga).count();

    cout << "\nConstruyendo las estructuras. Esto puede tardar...\n";

    auto inicioAVL = steady_clock::now();
    NodoAVL* raizAVL = ConstruirAVL(ListaV);
    auto finAVL = steady_clock::now();

    double tiempoAVL =
        duration<double>(finAVL - inicioAVL).count();

    auto inicioBTree = steady_clock::now();
    NodoBtree* raizBtree = ConstruirBtree(ListaV);
    auto finBTree = steady_clock::now();

    double tiempoBTree =
        duration<double>(finBTree - inicioBTree).count();

    auto inicioABB = steady_clock::now();
    NodoBB* raizABB = ConstruirABB(ListaV);
    auto finABB = steady_clock::now();

    double tiempoABB =
        duration<double>(finABB - inicioABB).count();

    cout << fixed << setprecision(3);
    cout << "\nEstructuras construidas.\n";
    cout << "Registros en la lista: " << totalVotantes << '\n';
    cout << "Carga:      " << tiempoCarga << " segundos\n";
    cout << "AVL:        " << tiempoAVL << " segundos\n";
    cout << "B-Tree:     " << tiempoBTree << " segundos\n";
    cout << "ABB:        " << tiempoABB << " segundos\n";

    int opcion = -1;

    do {
        MostrarMenu();

        if (!(cin >> opcion)) {
            cout << "\nEntrada invalida. Ingrese un numero.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = -1;
            continue;
        }

        switch (opcion) {
        case 1:
            ConsultarCedula(raizABB, raizAVL, raizBtree);
            break;

        case 2:
            cout << "\n========== ESTADISTICAS ==========\n";
            cout << "Registros cargados: " << totalVotantes << '\n';
            cout << "Altura AVL: " << AlturaAVL(raizAVL) << '\n';
            cout << "Altura B-Tree: " << AlturaBTreeMenu(raizBtree) << '\n';
            cout << "==================================\n";
            break;

        case 3:
            cout << fixed << setprecision(3);
            cout << "\n======= TIEMPOS DE EJECUCION =======\n";
            cout << "Carga:      " << tiempoCarga << " s\n";
            cout << "AVL:        " << tiempoAVL << " s\n";
            cout << "B-Tree:     " << tiempoBTree << " s\n";
            cout << "ABB:        " << tiempoABB << " s\n";
            cout << "===================================\n";
            break;
        case 4:
            
            ListarTodo(raizABB, raizAVL, raizBtree);
            break;
        case 0:
            cout << "\nCerrando el sistema...\n";
            break;

        default:
            cout << "\nOpcion no valida. Intente de nuevo.\n";
            break;
        }

    } while (opcion != 0);

    // Los arboles guardan punteros a los registros de la lista.
    // Por eso se destruyen primero los arboles.
    DestruirAVL(raizAVL);
    destruirBtree(raizBtree);
    PodarHojas(raizABB);

    LiberarVotantes(ListaV);

    cout << "Memoria liberada. Programa finalizado.\n";
    return 0;
}
