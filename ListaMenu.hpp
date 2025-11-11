#pragma once
#include "Nodo.hpp"
#include <iostream>
#include <functional>
using namespace std;

template<typename T>
class ListaMenu {
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    int tamaño;

public:
    ListaMenu() : cabeza(nullptr), cola(nullptr), tamaño(0) {}

    ListaMenu(const ListaMenu<T>& otra) : cabeza(nullptr), cola(nullptr), tamaño(0) {
        Nodo<T>* actual = otra.cabeza;
        while (actual != nullptr) {
            insertarFinal(actual->getData());
            actual = actual->getSiguiente();
        }
    }

    ~ListaMenu() {
        limpiar();
    }

    ListaMenu<T>& operator=(const ListaMenu<T>& otra) {
        if (this != &otra) {
            limpiar();
            Nodo<T>* actual = otra.cabeza;
            while (actual != nullptr) {
                insertarFinal(actual->getData());
                actual = actual->getSiguiente();
            }
        }
        return *this;
    }

    void insertarInicio(T elemento) {
        Nodo<T>* nuevo = new Nodo<T>(elemento);

        if (cabeza == nullptr) {
            cabeza = cola = nuevo;
        }
        else {
            nuevo->setSiguiente(cabeza);
            cabeza->setAnterior(nuevo);
            cabeza = nuevo;
        }
        tamaño++;
    }

    void insertarFinal(T elemento) {
        Nodo<T>* nuevo = new Nodo<T>(elemento);

        if (cabeza == nullptr) {
            cabeza = cola = nuevo;
        }
        else {
            cola->setSiguiente(nuevo);
            nuevo->setAnterior(cola);
            cola = nuevo;
        }
        tamaño++;
    }

    void insertarEnPosicion(int posicion, T elemento) {
        if (posicion < 0 || posicion > tamaño) {
            cout << "Posicion fuera de rango" << endl;
        }

        if (posicion == 0) {
            insertarInicio(elemento);
            return;
        }

        if (posicion == tamaño) {
            insertarFinal(elemento);
            return;
        }

        Nodo<T>* nuevo = new Nodo<T>(elemento);
        Nodo<T>* actual = cabeza;

        for (int i = 0; i < posicion; i++) {
            actual = actual->getSiguiente();
        }

        nuevo->setAnterior(actual->getAnterior());
        nuevo->setSiguiente(actual);
        actual->getAnterior()->setSiguiente(nuevo);
        actual->setAnterior(nuevo);
        tamaño++;
    }

    bool eliminar(T elemento) {
        if (cabeza == nullptr) return false;

        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            if (actual->getData() == elemento) {
                // Caso: es el único nodo
                if (actual == cabeza && actual == cola) {
                    cabeza = cola = nullptr;
                }
                // Caso: es el primer nodo
                else if (actual == cabeza) {
                    cabeza = cabeza->getSiguiente();
                    cabeza->setAnterior(nullptr);
                }
                // Caso: es el último nodo
                else if (actual == cola) {
                    cola = cola->getAnterior();
                    cola->setSiguiente(nullptr);
                }
                // Caso: está en el medio
                else {
                    actual->getAnterior()->setSiguiente(actual->getSiguiente());
                    actual->getSiguiente()->setAnterior(actual->getAnterior());
                }

                delete actual;
                tamaño--;
                return true;
            }
            actual = actual->getSiguiente();
        }
        return false;
    }

    bool eliminarEnPosicion(int posicion) {
        if (posicion < 0 || posicion >= tamaño) {
            cout << "Posición fuera de rango" << endl;
            return false;
        }

        Nodo<T>* actual;

        if (posicion < tamaño / 2) {
            actual = cabeza;
            for (int i = 0; i < posicion; i++) {
                actual = actual->getSiguiente();
            }
        }
        else {
            actual = cola;
            for (int i = tamaño - 1; i > posicion; i--) {
                actual = actual->getAnterior();
            }
        }


        if (actual == cabeza && actual == cola) {
            cabeza = cola = nullptr;
        }

        else if (actual == cabeza) {
            cabeza = cabeza->getSiguiente();
            cabeza->setAnterior(nullptr);
        }

        else if (actual == cola) {
            cola = cola->getAnterior();
            cola->setSiguiente(nullptr);
        }

        else {
            actual->getAnterior()->setSiguiente(actual->getSiguiente());
            actual->getSiguiente()->setAnterior(actual->getAnterior());
        }

        delete actual;
        tamaño--;
        return true;
    }

    bool eliminarSi(function<bool(const T&)> condicion) {
        if (cabeza == nullptr) return false;

        Nodo<T>* actual = cabeza;
        bool eliminado = false;

        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->getSiguiente();

            if (condicion(actual->getData())) {
                // Caso: es el único nodo
                if (actual == cabeza && actual == cola) {
                    cabeza = cola = nullptr;
                }
                // Caso: es el primer nodo
                else if (actual == cabeza) {
                    cabeza = cabeza->getSiguiente();
                    cabeza->setAnterior(nullptr);
                }
                // Caso: es el último nodo
                else if (actual == cola) {
                    cola = cola->getAnterior();
                    cola->setSiguiente(nullptr);
                }
                // Caso: está en el medio
                else {
                    actual->getAnterior()->setSiguiente(actual->getSiguiente());
                    actual->getSiguiente()->setAnterior(actual->getAnterior());
                }

                delete actual;
                tamaño--;
                eliminado = true;
            }

            actual = siguiente;
        }

        return eliminado;
    }

    bool buscar(T elemento) const {
        Nodo<T>* actual = cabeza;
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
            cout << "Posicion fuera de rango" << endl;
        }

        Nodo<T>* actual;


        if (posicion < tamaño / 2) {
            actual = cabeza;
            for (int i = 0; i < posicion; i++) {
                actual = actual->getSiguiente();
            }
        }
        else {
            actual = cola;
            for (int i = tamaño - 1; i > posicion; i--) {
                actual = actual->getAnterior();
            }
        }

        return actual->getData();
    }

    T obtenerPrimero() const {
        if (cabeza == nullptr) {
            cout << "Lista vacia" << endl;
        }
        return cabeza->getData();
    }

    T obtenerUltimo() const {
        if (cola == nullptr) {
            cout << "Lista vacia" << endl;
        }
        return cola->getData();
    }

    int getTamaño() const { return tamaño; }

    bool estaVacia() const { return cabeza == nullptr; }

    void mostrar() const {
        Nodo<T>* actual = cabeza;
        cout << "Lista: ";
        while (actual != nullptr) {
            cout << actual->getData();
            if (actual->getSiguiente() != nullptr) {
                cout << " <-> ";
            }
            actual = actual->getSiguiente();
        }
        cout << " -> NULL" << endl;
    }

    void mostrarReversa() const {
        Nodo<T>* actual = cola;
        cout << "Lista (reversa): ";
        while (actual != nullptr) {
            cout << actual->getData();
            if (actual->getAnterior() != nullptr) {
                cout << " <-> ";
            }
            actual = actual->getAnterior();
        }
        cout << " -> NULL" << endl;
    }

    ListaMenu<T> filtrar(function<bool(const T&)> condicion) const {
        ListaMenu<T> resultado;
        Nodo<T>* actual = cabeza;

        while (actual != nullptr) {
            if (condicion(actual->getData())) {
                resultado.insertarFinal(actual->getData());
            }
            actual = actual->getSiguiente();
        }
        return resultado;
    }
    void ordenar(function<bool(const T&, const T&)> comparador) {
        if (tamaño <= 1) return;

        cabeza = mergeSort(cabeza, comparador);

        // Actualizar cola
        Nodo<T>* temp = cabeza;
        while (temp->getSiguiente() != nullptr) {
            temp = temp->getSiguiente();
        }
        cola = temp;
    }

    Nodo<T>* mergeSort(Nodo<T>* head, function<bool(const T&, const T&)> comparador) {
        if (!head || !head->getSiguiente()) return head;

        Nodo<T>* middle = dividir(head);
        Nodo<T>* mitad_derecha = middle->getSiguiente();
        middle->setSiguiente(nullptr);

        Nodo<T>* izquierda = mergeSort(head, comparador);
        Nodo<T>* derecha = mergeSort(mitad_derecha, comparador);

        return merge(izquierda, derecha, comparador);
    }

    Nodo<T>* merge(Nodo<T>* izquierda, Nodo<T>* derecha, function<bool(const T&, const T&)> comparador) {
        if (!izquierda) return derecha;
        if (!derecha) return izquierda;

        Nodo<T>* resultado = nullptr;

        if (comparador(izquierda->getData(), derecha->getData())) {
            resultado = izquierda;
            resultado->setSiguiente(merge(izquierda->getSiguiente(), derecha, comparador));
            if (resultado->getSiguiente()) resultado->getSiguiente()->setAnterior(resultado);
        }
        else {
            resultado = derecha;
            resultado->setSiguiente(merge(izquierda, derecha->getSiguiente(), comparador));
            if (resultado->getSiguiente()) resultado->getSiguiente()->setAnterior(resultado);
        }
        return resultado;
    }

    Nodo<T>* dividir(Nodo<T>* head) {
        Nodo<T>* lento = head;
        Nodo<T>* rapido = head->getSiguiente();

        while (rapido && rapido->getSiguiente()) {
            lento = lento->getSiguiente();
            rapido = rapido->getSiguiente()->getSiguiente();
        }
        return lento;
    }

private:
    void limpiar() {
        while (cabeza != nullptr) {
            Nodo<T>* temp = cabeza;
            cabeza = cabeza->getSiguiente();
            delete temp;
        }
        cola = nullptr;
        tamaño = 0;
    }
};