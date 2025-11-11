#pragma once
#include "Nodo.hpp"
#include <iostream>
#include <functional>
using namespace std;

template<typename T>
class Pila {
private:
    Nodo<T>* tope;
    int tamaño;

public:

    Pila() : tope(nullptr), tamaño(0) {}


    ~Pila() {
        limpiar();
    }


    void apilar(T elemento) {
        Nodo<T>* nuevo = new Nodo<T>(elemento);
        nuevo->setSiguiente(tope);
        tope = nuevo;
        tamaño++;
    }


    T desapilar() {
        if (estaVacia()) {
            throw runtime_error("Pila vacia");
        }

        Nodo<T>* temp = tope;
        T elemento = temp->getData();
        tope = tope->getSiguiente();

        delete temp;
        tamaño--;
        return elemento;
    }


    T verTope() const {
        if (estaVacia()) {
            throw runtime_error("Pila vacia");
        }
        return tope->getData();
    }


    bool estaVacia() const {
        return tope == nullptr;
    }


    int getTamaño() const {
        return tamaño;
    }


    void mostrar() const {
        if (estaVacia()) {
            cout << "Pila vacia" << endl;
            return;
        }

        cout << "Tope:" << endl;
        Nodo<T>* actual = tope;
        while (actual != nullptr) {
            cout << "  | " << actual->getData() << " |" << endl;
            actual = actual->getSiguiente();
        }
        cout << "  +-----+" << endl;
    }


    bool buscar(T elemento) const {
        Nodo<T>* actual = tope;
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
            throw out_of_range("Posicion fuera de rango");
        }

        Nodo<T>* actual = tope;
        for (int i = 0; i < posicion; i++) {
            actual = actual->getSiguiente();
        }
        return actual->getData();
    }


    Pila<T> filtrar(function<bool(const T&)> condicion) const {
        Pila<T> resultado;
        Pila<T> temporal;

        Nodo<T>* actual = tope;
        while (actual != nullptr) {
            if (condicion(actual->getData())) {
                temporal.apilar(actual->getData());
            }
            actual = actual->getSiguiente();
        }


        while (!temporal.estaVacia()) {
            resultado.apilar(temporal.desapilar());
        }

        return resultado;
    }

private:
    void limpiar() {
        while (!estaVacia()) {
            desapilar();
        }
    }
};
