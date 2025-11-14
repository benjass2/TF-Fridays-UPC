#pragma once
#include "Nodo.hpp"
#include "Funcionalidades.hpp"
#include <vector>
#include <functional>

using namespace std;

template<typename T>
class GestorOrdenamiento {
public:
    enum Algoritmo {
        MERGE_SORT,
        QUICK_SORT,
        HEAP_SORT
    };
    //Con esta funcion se ordena segun el algoritmo que se le pasemos
    static void ordenar(Nodo<T>*& cabeza, Nodo<T>*& cola,
        function<bool(const T&, const T&)> comparador,
        Algoritmo algoritmo = MERGE_SORT) {

        if (!cabeza || !cabeza->getSiguiente()) return;

        switch (algoritmo) {
        case MERGE_SORT:
            cabeza = mergeSort(cabeza, comparador);
            break;
        case QUICK_SORT:
            quickSortWrapper(cabeza, cola, comparador);
            break;
        case HEAP_SORT:
            heapSort(cabeza, comparador);
            break;
        }

        actualizarCola(cabeza, cola);
    }

private:

    static void actualizarCola(Nodo<T>* cabeza, Nodo<T>*& cola) {
        if (!cabeza) {
            cola = nullptr;
            return;
        }

        Nodo<T>* temp = cabeza;
        while (temp->getSiguiente() != nullptr) {
            temp = temp->getSiguiente();
        }
        cola = temp;
    }

    static Nodo<T>* mergeSort(Nodo<T>* head, function<bool(const T&, const T&)> comparador) {
        if (!head || !head->getSiguiente()) return head;

        Nodo<T>* middle = dividir(head);
        Nodo<T>* mitad_derecha = middle->getSiguiente();
        middle->setSiguiente(nullptr);
        if (mitad_derecha) mitad_derecha->setAnterior(nullptr);

        Nodo<T>* izquierda = mergeSort(head, comparador);
        Nodo<T>* derecha = mergeSort(mitad_derecha, comparador);

        return merge(izquierda, derecha, comparador);
    }

    static Nodo<T>* merge(Nodo<T>* izquierda, Nodo<T>* derecha,
        function<bool(const T&, const T&)> comparador) {
        if (!izquierda) return derecha;
        if (!derecha) return izquierda;

        Nodo<T>* resultado = nullptr;

        if (comparador(izquierda->getData(), derecha->getData())) {
            resultado = izquierda;
            resultado->setSiguiente(merge(izquierda->getSiguiente(), derecha, comparador));
            if (resultado->getSiguiente()) {
                resultado->getSiguiente()->setAnterior(resultado);
            }
            resultado->setAnterior(nullptr);
        }
        else {
            resultado = derecha;
            resultado->setSiguiente(merge(izquierda, derecha->getSiguiente(), comparador));
            if (resultado->getSiguiente()) {
                resultado->getSiguiente()->setAnterior(resultado);
            }
            resultado->setAnterior(nullptr);
        }
        return resultado;
    }

    static Nodo<T>* dividir(Nodo<T>* head) {
        Nodo<T>* lento = head;
        Nodo<T>* rapido = head->getSiguiente();

        while (rapido && rapido->getSiguiente()) {
            lento = lento->getSiguiente();
            rapido = rapido->getSiguiente()->getSiguiente();
        }
        return lento;
    }

    static void quickSortWrapper(Nodo<T>*& cabeza, Nodo<T>*& cola,
        function<bool(const T&, const T&)> comparador) {
        quickSort(cabeza, cola, comparador);
    }

    static void quickSort(Nodo<T>* cabeza, Nodo<T>* cola,
        function<bool(const T&, const T&)> comparador) {
        if (!cabeza || cabeza == cola || !cola) return;

        Nodo<T>* pivot = particion(cabeza, cola, comparador);

        if (pivot != nullptr && pivot->getAnterior() != nullptr) {
            quickSort(cabeza, pivot->getAnterior(), comparador);
        }

        if (pivot != nullptr && pivot->getSiguiente() != nullptr) {
            quickSort(pivot->getSiguiente(), cola, comparador);
        }
    }

    static Nodo<T>* particion(Nodo<T>* cabeza, Nodo<T>* cola,
        function<bool(const T&, const T&)> comparador) {
        T pivotData = cola->getData();
        Nodo<T>* i = nullptr;

        for (Nodo<T>* j = cabeza; j != cola; j = j->getSiguiente()) {
            if (comparador(j->getData(), pivotData) || j->getData() == pivotData) {
                if (i == nullptr) {
                    i = cabeza;
                }
                else {
                    i = i->getSiguiente();
                }

                T temp = i->getData();
                i->setData(j->getData());
                j->setData(temp);
            }
        }

        if (i == nullptr) {
            i = cabeza;
        }
        else {
            i = i->getSiguiente();
        }

        T temp = i->getData();
        i->setData(cola->getData());
        cola->setData(temp);

        return i;
    }

    static void heapify(vector<T>& datos, int n, int i, function<bool(const T&, const T&)> comparador) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && comparador(datos[largest], datos[left])) {
            largest = left;
        }

        if (right < n && comparador(datos[largest], datos[right])) {
            largest = right;
        }

        if (largest != i) {
            T temp = datos[i];
            datos[i] = datos[largest];
            datos[largest] = temp;

            heapify(datos, n, largest, comparador);
        }
    }

    static void construirHeapManual(vector<T>& datos, function<bool(const T&, const T&)> comparador) {
        int n = datos.size();
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(datos, n, i, comparador);
        }
    }

    static void heapSort(Nodo<T>* cabeza, function<bool(const T&, const T&)> comparador) {
        if (!cabeza) return;

        vector<T> datos;
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            datos.push_back(actual->getData());
            actual = actual->getSiguiente();
        }

        int n = datos.size();

        construirHeapManual(datos, comparador);

        for (int i = n - 1; i > 0; i--) {
            T temp = datos[0];
            datos[0] = datos[i];
            datos[i] = temp;

            heapify(datos, i, 0, comparador);
        }

        actual = cabeza;
        for (const T& datoOrdenado : datos) {
            actual->setData(datoOrdenado);
            actual = actual->getSiguiente();
        }
    }
};