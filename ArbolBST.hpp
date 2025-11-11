#pragma once
#include "Funcionalidades.hpp"

// Nodo del árbol binario
template<typename T>
class NodoBST {
public:
    T dato;
    NodoBST<T>* izquierda;
    NodoBST<T>* derecha;

    NodoBST(T valor) : dato(valor), izquierda(nullptr), derecha(nullptr) {}
};

// Árbol Binario de Búsqueda
template<typename T>
class ArbolBST {
private:
    NodoBST<T>* raiz;
    int tamaño;


    // Insertar recursivo
    NodoBST<T>* insertarRec(NodoBST<T>* nodo, T valor) {
        if (nodo == nullptr) {
            tamaño++;
            return new NodoBST<T>(valor);
        }

        if (valor < nodo->dato) {
            nodo->izquierda = insertarRec(nodo->izquierda, valor);
        }
        else if (valor > nodo->dato) {
            nodo->derecha = insertarRec(nodo->derecha, valor);
        }

        return nodo;
    }

    // Buscar recursivo
    NodoBST<T>* buscarRec(NodoBST<T>* nodo, T valor) const {
        if (nodo == nullptr || nodo->dato == valor) {
            return nodo;
        }

        if (valor < nodo->dato) {
            return buscarRec(nodo->izquierda, valor);
        }
        return buscarRec(nodo->derecha, valor);
    }

    // Encontrar mínimo
    NodoBST<T>* encontrarMinimo(NodoBST<T>* nodo) const {
        while (nodo->izquierda != nullptr) {
            nodo = nodo->izquierda;
        }
        return nodo;
    }

    // Eliminar recursivo
    NodoBST<T>* eliminarRec(NodoBST<T>* nodo, T valor) {
        if (nodo == nullptr) return nullptr;

        if (valor < nodo->dato) {
            nodo->izquierda = eliminarRec(nodo->izquierda, valor);
        }
        else if (valor > nodo->dato) {
            nodo->derecha = eliminarRec(nodo->derecha, valor);
        }
        else {
            // Nodo encontrado
            if (nodo->izquierda == nullptr) {
                NodoBST<T>* temp = nodo->derecha;
                delete nodo;
                tamaño--;
                return temp;
            }
            else if (nodo->derecha == nullptr) {
                NodoBST<T>* temp = nodo->izquierda;
                delete nodo;
                tamaño--;
                return temp;
            }

            // Nodo con dos hijos
            NodoBST<T>* temp = encontrarMinimo(nodo->derecha);
            nodo->dato = temp->dato;
            nodo->derecha = eliminarRec(nodo->derecha, temp->dato);
        }
        return nodo;
    }

    // Recorrido in-order
    void inOrderRec(NodoBST<T>* nodo, function<void(T)> funcion) const {
        if (nodo != nullptr) {
            inOrderRec(nodo->izquierda, funcion);
            funcion(nodo->dato);
            inOrderRec(nodo->derecha, funcion);
        }
    }

    // Recorrido pre-order
    void preOrderRec(NodoBST<T>* nodo, function<void(T)> funcion) const {
        if (nodo != nullptr) {
            funcion(nodo->dato);
            preOrderRec(nodo->izquierda, funcion);
            preOrderRec(nodo->derecha, funcion);
        }
    }

    // Recorrido post-order
    void postOrderRec(NodoBST<T>* nodo, function<void(T)> funcion) const {
        if (nodo != nullptr) {
            postOrderRec(nodo->izquierda, funcion);
            postOrderRec(nodo->derecha, funcion);
            funcion(nodo->dato);
        }
    }

    // Destruir árbol
    void destruirArbol(NodoBST<T>* nodo) {
        if (nodo != nullptr) {
            destruirArbol(nodo->izquierda);
            destruirArbol(nodo->derecha);
            delete nodo;
        }
    }

    // Calcular altura
    int alturaRec(NodoBST<T>* nodo) const {
        if (nodo == nullptr) return 0;

        int altIzq = alturaRec(nodo->izquierda);
        int altDer = alturaRec(nodo->derecha);

        return 1 + max(altIzq, altDer);
    }

public:

    ArbolBST() : raiz(nullptr), tamaño(0) {}

    ~ArbolBST() {
        destruirArbol(raiz);
    }



    // Insertar elemento
    void insertar(T valor) {
        raiz = insertarRec(raiz, valor);
    }

    // Buscar elemento
    bool buscar(T valor) const {
        return buscarRec(raiz, valor) != nullptr;
    }

    // Eliminar elemento
    void eliminar(T valor) {
        raiz = eliminarRec(raiz, valor);
    }

    // Recorrido in-order (izquierda -> raíz -> derecha)
    void inOrder(function<void(T)> funcion) const {
        inOrderRec(raiz, funcion);
    }

    // Recorrido pre-order (raíz -> izquierda -> derecha)
    void preOrder(function<void(T)> funcion) const {
        preOrderRec(raiz, funcion);
    }

    // Recorrido post-order (izquierda -> derecha -> raíz)
    void postOrder(function<void(T)> funcion) const {
        postOrderRec(raiz, funcion);
    }

    // Obtener tamaño
    int getTamaño() const {
        return tamaño;
    }

    // Verificar si está vacío
    bool estaVacio() const {
        return raiz == nullptr;
    }

    // Obtener altura del árbol
    int getAltura() const {
        return alturaRec(raiz);
    }

    // Limpiar árbol
    void limpiar() {
        destruirArbol(raiz);
        raiz = nullptr;
        tamaño = 0;
    }
};