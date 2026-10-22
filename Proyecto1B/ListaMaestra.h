#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

typedef struct TVotante { //Se define la estructura para los votantes
    char cedula[10];
    char codelec[7];
    char sexo;
    char fecha[9];
    char numjun[6];
    char nombre[31];
    char papellido[27];
    char sapellido[27];
    TVotante* Siguiente;
}*PtrTVotante;


void DestruirInventario(PtrTVotante& Lista)
{
    PtrTVotante Aux;
    Aux = Lista;
    while (Aux != NULL)
    {
        Lista = Lista->Siguiente;
        delete(Aux);
        Aux = Lista;
    }
}

void agregarvotante(
    PtrTVotante& Lista,
    const string& cedula,
    const string& codelec,
    const string& sexo,
    const string& fecha,
    const string& numjun,
    const string& nombre,
    const string& papellido,
    const string& sapellido
) {
    PtrTVotante Aux = new TVotante;

    strncpy_s(Aux->cedula, cedula.c_str(), sizeof(Aux->cedula) - 1);
    strncpy_s(Aux->codelec, codelec.c_str(), sizeof(Aux->codelec) - 1);
    Aux->sexo = sexo.empty() ? ' ' : sexo[0];
    strncpy_s(Aux->fecha, fecha.c_str(), sizeof(Aux->fecha) - 1);
    strncpy_s(Aux->numjun, numjun.c_str(), sizeof(Aux->numjun) - 1);
    strncpy_s(Aux->nombre, nombre.c_str(), sizeof(Aux->nombre) - 1);
    strncpy_s(Aux->papellido, papellido.c_str(), sizeof(Aux->papellido) - 1);
    strncpy_s(Aux->sapellido, sapellido.c_str(), sizeof(Aux->sapellido) - 1);

    Aux->Siguiente = Lista;
    Lista = Aux;
}


//Funcion para cargar los votantes, se encarga de tomar cada una de las lineas
//directo del archivo de texto y enviarlo a la funci�n encargada de extraer cad uno
// de los datos e incorporarlos en un nodo de la lista.
void CargarVotantes(PtrTVotante& Lista) {
    ifstream archivo("PADRON_COMPLETO--.txt");

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return;
    }

    string linea;
    int cont = 0;

    auto inicio = chrono::steady_clock::now();

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campos[8];

        for (int i = 0; i < 8; i++) {
            if (!getline(ss, campos[i], ',')) {
                break;
            }
        }

        if (ss.fail() && campos[7].empty()) {
            // Esta condición no debe utilizarse como validación
            // definitiva; conviene contar los campos correctamente.
        }

        agregarvotante(
            Lista,
            campos[0], campos[1], campos[2], campos[3],
            campos[4], campos[5], campos[6], campos[7]
        );

        cont++;
    }

    auto fin = chrono::steady_clock::now();
    chrono::duration<double> duracion = fin - inicio;

    cout << "Registros cargados: " << cont << endl;
    cout << "Tiempo de carga: " << duracion.count()
        << " segundos." << endl;
}

//Funcion para liberar los votantes en estructuras de memoria dinamica de la lista enlazada hasta dejar la lista en NULL

void LiberarVotantes(PtrTVotante& Lista) {
    PtrTVotante Aux = Lista;

    int cont = 0;
    auto inicio = chrono::steady_clock::now();

    while (Aux != NULL) {
        Lista = Lista->Siguiente;
        delete(Aux);
        //Globalfree(Aux);
        Aux = Lista;
    }

    auto fin = chrono::steady_clock::now();
    chrono::duration<double> duracion = fin - inicio;

    cout << "Registros cargados: " << cont << endl;
    cout << "Tiempo de liberación: " << duracion.count()
        << " segundos." << endl;
}


//Funci�n para inicializar la lista de votantes
void inicializarvotantes(PtrTVotante& Lista) {
    Lista = NULL;
}

PtrTVotante BuscarVotante(PtrTVotante& Lista, char cual[9])
{
    bool encontro = false;
    PtrTVotante Aux;
    Aux = Lista;

    while ((!encontro == true) && (Aux != NULL))
    {
        if (strncmp(Aux->cedula, cual, 9) == 0)
            encontro = true;
        else
            Aux = Aux->Siguiente;
    }
    return Aux;
}

PtrTVotante BuscarArticulo(PtrTVotante& Lista, int cual)
{
    return Lista;
}