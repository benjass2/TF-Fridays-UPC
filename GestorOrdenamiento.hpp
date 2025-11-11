#pragma once
#include "Nodo.hpp"
#include "Funcionalidades.hpp"
using namespace std;

template<typename T>
class GestorOrdenamiento {
public:
    enum Algoritmo {
        MERGE_SORT,
        QUICK_SORT,
        BUBBLE_SORT
    };

    // Método principal para ordenar
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
        case BUBBLE_SORT:
            bubbleSort(cabeza, comparador);
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

    //Algoritmo Merge Sort
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

	//Algoritmo Quick Sort
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

                // Intercambiar datos
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

        // Intercambiar con el pivote
        T temp = i->getData();
        i->setData(cola->getData());
        cola->setData(temp);

        return i;
    }

	//Algoritmo Bubble Sort
    static void bubbleSort(Nodo<T>* cabeza, std::function<bool(const T&, const T&)> comparador) {
        if (!cabeza) return;

        bool intercambiado;
        Nodo<T>* actual;
        Nodo<T>* ultimo = nullptr;

        do {
            intercambiado = false;
            actual = cabeza;

            while (actual->getSiguiente() != ultimo) {
                if (!comparador(actual->getData(), actual->getSiguiente()->getData())) {
                    // Intercambiar datos
                    T temp = actual->getData();
                    actual->setData(actual->getSiguiente()->getData());
                    actual->getSiguiente()->setData(temp);
                    intercambiado = true;
                }
                actual = actual->getSiguiente();
            }
            ultimo = actual;
        } while (intercambiado);
    }
};