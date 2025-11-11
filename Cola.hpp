#pragma once
#include "Nodo.hpp"
#include <iostream>
#include <functional>
using namespace std;

template<typename T>
class Cola {
private:
    Nodo<T>* frente;
    Nodo<T>* final;
    int tamaño;

public:

    Cola() : frente(nullptr), final(nullptr), tamaño(0) {}


    ~Cola() {
        limpiar();
    }


    void encolar(T elemento) {
        Nodo<T>* nuevo = new Nodo<T>(elemento);

        if (estaVacia()) {
            frente = nuevo;
            final = nuevo;
        }
        else {
            final->setSiguiente(nuevo);
            final = nuevo;
        }
        tamaño++;
    }


    T desencolar() {
        if (estaVacia()) {
            cout << "Cola vacia" << endl;
        }

        Nodo<T>* temp = frente;
        T elemento = temp->getData();
        frente = frente->getSiguiente();

        if (frente == nullptr) {
            final = nullptr;
        }

        delete temp;
        tamaño--;
        return elemento;
    }

    bool eliminarEnPosicion(int posicion) {
        if (posicion < 0 || posicion >= tamaño) {
            cout << "Posicion invalida" << endl;
            return false;
        }


        if (posicion == 0) {
            desencolar();
            return true;
        }


        Nodo<T>* actual = frente;
        for (int i = 0; i < posicion - 1; i++) {
            actual = actual->getSiguiente();
        }


        Nodo<T>* aEliminar = actual->getSiguiente();


        actual->setSiguiente(aEliminar->getSiguiente());


        if (aEliminar == final) {
            final = actual;
        }

        delete aEliminar;
        tamaño--;
        return true;
    }


    T verFrente() const {
        if (estaVacia()) {
            cout << "Cola vacia" << endl;
        }
        return frente->getData();
    }


    bool estaVacia() const {
        return frente == nullptr;
    }


    int getTamaño() const {
        return tamaño;
    }


    void mostrar() const {
        if (estaVacia()) {
            cout << "Cola vacia" << endl;
            return;
        }

        cout << "Frente -> ";
        Nodo<T>* actual = frente;
        while (actual != nullptr) {
            cout << actual->getData() << " -> ";
            actual = actual->getSiguiente();
        }
        cout << "Final" << endl;
    }


    bool buscar(T elemento) const {
        Nodo<T>* actual = frente;
        while (actual != nullptr) {
            if (actual->getData() == elemento) {
                return true;
            }
            actual = actual->getSiguiente();
        }
        return false;
    }


    T obtenerEnPosicion(int posicion) const {
        if (posicion < 0 || posicion >= tamaño) {
            cout << "Posicion invalida" << endl;
        }

        Nodo<T>* actual = frente;
        for (int i = 0; i < posicion; i++) {
            actual = actual->getSiguiente();
        }
        return actual->getData();
    }


    int contarSi(function<bool(const T&)> condicion) const {
        int contador = 0;
        Nodo<T>* actual = frente;

        while (actual != nullptr) {
            if (condicion(actual->getData())) {
                contador++;
            }
            actual = actual->getSiguiente();
        }
        return contador;
    }

private:
    void limpiar() {
        while (!estaVacia()) {
            desencolar();
        }
    }
};

