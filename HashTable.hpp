#pragma once
#include "Funcionalidades.hpp"

template<typename K, typename V>
class NodoHash {
private:
    K clave;
    V valor;
    NodoHash<K, V>* siguiente;

public:
    NodoHash(K k, V v) : clave(k), valor(v), siguiente(nullptr) {}

    K getClave() const { return clave; }
    V getValor() const { return valor; }
    NodoHash<K, V>* getSiguiente() const { return siguiente; }

    void setValor(V v) { valor = v; }
    void setSiguiente(NodoHash<K, V>* sig) { siguiente = sig; }
};

template<typename K, typename V>
class HashTable {
private:
    NodoHash<K, V>** tabla;
    int capacidad;
    int tamaño;
    float factorCarga;

    // Función hash para enteros (ID de productos)
    int funcionHash(int clave) const {
        // Método de división con número primo
        return abs(clave) % capacidad;
    }

    // Función hash para strings (nombre de productos)
    int funcionHash(const string& clave) const {
        // Algoritmo djb2
        unsigned long hash = 5381;
        for (char c : clave) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % capacidad;
    }

    // Sobrecarga para usar con cualquier tipo
    int hash(const K& clave) const {
        if constexpr (is_same<K, int>::value) {
            return funcionHash(clave);
        }
        else if constexpr (is_same<K, string>::value) {
            return funcionHash(clave);
        }
        return 0;
    }

    // Redimensionar cuando el factor de carga es alto
    void redimensionar() {
        int nuevaCapacidad = capacidad * 2;
        NodoHash<K, V>** nuevaTabla = new NodoHash<K, V>* [nuevaCapacidad];

        // Inicializar nueva tabla
        for (int i = 0; i < nuevaCapacidad; i++) {
            nuevaTabla[i] = nullptr;
        }

        int capacidadAnterior = capacidad;
        NodoHash<K, V>** tablaAnterior = tabla;

        capacidad = nuevaCapacidad;
        tabla = nuevaTabla;
        tamaño = 0;

        // Reinsertar todos los elementos
        for (int i = 0; i < capacidadAnterior; i++) {
            NodoHash<K, V>* actual = tablaAnterior[i];
            while (actual != nullptr) {
                insertar(actual->getClave(), actual->getValor());
                NodoHash<K, V>* temp = actual;
                actual = actual->getSiguiente();
                delete temp;
            }
        }

        delete[] tablaAnterior;
    }

public:
    HashTable(int cap = 53) : capacidad(cap), tamaño(0), factorCarga(0.75) {
        tabla = new NodoHash<K, V>* [capacidad];
        for (int i = 0; i < capacidad; i++) {
            tabla[i] = nullptr;
        }
    }

    ~HashTable() {
        limpiar();
        delete[] tabla;
    }

    // Insertar o actualizar un elemento
    void insertar(const K& clave, const V& valor) {
        // Verificar factor de carga
        if ((float)tamaño / capacidad >= factorCarga) {
            redimensionar();
        }

        int indice = hash(clave);
        NodoHash<K, V>* actual = tabla[indice];

        // Buscar si la clave ya existe
        while (actual != nullptr) {
            if (actual->getClave() == clave) {
                actual->setValor(valor); // Actualizar valor existente
                return;
            }
            actual = actual->getSiguiente();
        }

        // Insertar nuevo nodo al inicio de la lista
        NodoHash<K, V>* nuevo = new NodoHash<K, V>(clave, valor);
        nuevo->setSiguiente(tabla[indice]);
        tabla[indice] = nuevo;
        tamaño++;
    }

    // Buscar un elemento por clave
    V* buscar(const K& clave) {
        int indice = hash(clave);
        NodoHash<K, V>* actual = tabla[indice];

        while (actual != nullptr) {
            if (actual->getClave() == clave) {
                return new V(actual->getValor()); // Retornar puntero al valor
            }
            actual = actual->getSiguiente();
        }

        return nullptr; // No encontrado
    }

    // Verificar si existe una clave
    bool contiene(const K& clave) const {
        int indice = hash(clave);
        NodoHash<K, V>* actual = tabla[indice];

        while (actual != nullptr) {
            if (actual->getClave() == clave) {
                return true;
            }
            actual = actual->getSiguiente();
        }

        return false;
    }

    // Eliminar un elemento
    bool eliminar(const K& clave) {
        int indice = hash(clave);
        NodoHash<K, V>* actual = tabla[indice];
        NodoHash<K, V>* anterior = nullptr;

        while (actual != nullptr) {
            if (actual->getClave() == clave) {
                if (anterior == nullptr) {
                    // Es el primer nodo
                    tabla[indice] = actual->getSiguiente();
                }
                else {
                    anterior->setSiguiente(actual->getSiguiente());
                }
                delete actual;
                tamaño--;
                return true;
            }
            anterior = actual;
            actual = actual->getSiguiente();
        }

        return false; // No encontrado
    }

    // Obtener todas las claves
    vector<K> obtenerClaves() const {
        vector<K> claves;
        for (int i = 0; i < capacidad; i++) {
            NodoHash<K, V>* actual = tabla[i];
            while (actual != nullptr) {
                claves.push_back(actual->getClave());
                actual = actual->getSiguiente();
            }
        }
        return claves;
    }

    // Obtener todos los valores
    vector<V> obtenerValores() const {
        vector<V> valores;
        for (int i = 0; i < capacidad; i++) {
            NodoHash<K, V>* actual = tabla[i];
            while (actual != nullptr) {
                valores.push_back(actual->getValor());
                actual = actual->getSiguiente();
            }
        }
        return valores;
    }

    // Limpiar la tabla
    void limpiar() {
        for (int i = 0; i < capacidad; i++) {
            NodoHash<K, V>* actual = tabla[i];
            while (actual != nullptr) {
                NodoHash<K, V>* temp = actual;
                actual = actual->getSiguiente();
                delete temp;
            }
            tabla[i] = nullptr;
        }
        tamaño = 0;
    }

    // Mostrar estadísticas de la tabla
    void mostrarEstadisticas() const {
        cout << "\n=== ESTADISTICAS HASHTABLE ===" << endl;
        cout << "Capacidad: " << capacidad << endl;
        cout << "Elementos: " << tamaño << endl;
        cout << "Factor de carga: " << fixed << setprecision(2)
            << (float)tamaño / capacidad << endl;

        int colisiones = 0;
        int bucketVacios = 0;
        int maxLongitud = 0;

        for (int i = 0; i < capacidad; i++) {
            int longitud = 0;
            NodoHash<K, V>* actual = tabla[i];

            if (actual == nullptr) {
                bucketVacios++;
            }
            else {
                while (actual != nullptr) {
                    longitud++;
                    actual = actual->getSiguiente();
                }
                if (longitud > 1) colisiones += (longitud - 1);
                if (longitud > maxLongitud) maxLongitud = longitud;
            }
        }

        cout << "Buckets vacios: " << bucketVacios << endl;
        cout << "Colisiones totales: " << colisiones << endl;
        cout << "Longitud maxima de cadena: " << maxLongitud << endl;
        cout << "==============================\n" << endl;
    }

    // Getters
    int getTamaño() const { return tamaño; }
    int getCapacidad() const { return capacidad; }
    bool estaVacia() const { return tamaño == 0; }
};