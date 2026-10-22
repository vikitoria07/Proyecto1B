#pragma once

#include "iostream"
#include <stdio.h>
#include "ListaMaestra.h"

typedef struct NodoBB
{
	int llave;
	PtrTVotante votante;
	NodoBB* izq;
	NodoBB* derecha;
}X;

NodoBB* CrearNodoABB(
	long long llave,
	PtrTVotante votante
) {
	NodoBB* nuevo = new NodoBB;

	nuevo->llave = llave;
	nuevo->votante = votante;
	nuevo->izq = nullptr;
	nuevo->derecha = nullptr;

	return nuevo;
}
bool Insertar(NodoBB*& Raiz, int cedula, PtrTVotante votante)
{
	if (Raiz == NULL)
	{
		Raiz = CrearNodoABB(cedula, votante);
		return true;
	}
	else
	{
		if (cedula == Raiz->llave) return false;
		else
			if (cedula < Raiz->llave) return Insertar(Raiz->izq, cedula, votante);
			else return Insertar(Raiz->derecha, cedula, votante);
	}
}



void PodarHojas(NodoBB*& Raiz)
{
	if (Raiz != NULL)
	{
		PodarHojas(Raiz->izq);
		PodarHojas(Raiz->derecha);
		delete(Raiz); Raiz = NULL;
	}
}

NodoBB* Buscar(NodoBB* Raiz, int cualllave)
{
	if (Raiz == NULL)
	{
		return NULL;
	}
	else
	{
		if (cualllave == Raiz->llave) return Raiz;
		else
			if (cualllave < Raiz->llave) return Buscar(Raiz->izq, cualllave);
			else return Buscar(Raiz->derecha, cualllave);
	}
}

void PreOrdenRID(NodoBB* Raiz)
{
	if (Raiz != NULL)
	{
		printf("%i  ", Raiz->llave);
		PreOrdenRID(Raiz->izq);
		PreOrdenRID(Raiz->derecha);
	}
}

void EnOrdenIRD(NodoBB* Raiz)
{
	if (Raiz != NULL)
	{
		EnOrdenIRD(Raiz->izq);
		printf("%i  ", Raiz->llave);
		EnOrdenIRD(Raiz->derecha);
	}
}
void PosOrdenIDR(NodoBB* Raiz)
{
	if (Raiz != NULL)
	{
		PosOrdenIDR(Raiz->izq);
		PosOrdenIDR(Raiz->derecha);
		printf("%i  ", Raiz->llave);
	}
}



// Construye el AVL recorriendo la lista maestra.
// La lista debe seguir existiendo mientras se use el árbol.
NodoBB* ConstruirABB(PtrTVotante lista) {
	NodoBB* raiz = nullptr;

	while (lista != nullptr) {
		char* fin = nullptr;

		long long cedula = strtoll(
			lista->cedula, &fin, 10
		);

		// Verifica que toda la cadena sea numerica.
		if (fin != lista->cedula && *fin == '\0') {
			Insertar(raiz, cedula, lista);
		}

		lista = lista->Siguiente;
	}

	return raiz;
}